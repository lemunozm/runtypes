#ifndef RT__STRUCT_HPP_
#define RT__STRUCT_HPP_

#include <runtypes/Exception.hpp>
#include <runtypes/CType.hpp>

#include <map>
#include <vector>

namespace rt
{

//=========================== MEMBER =============================
class Member
{
public:
    static Member ref(size_t offset, const Type& type)
    {
        return Member(offset, type, false);
    }

    static Member copy(size_t offset, const Type& type)
    {
        return Member(offset, type, false);
    }

    template<typename T, typename... Args>
    static Member create_ctype(size_t offset, Args&&... args)
    {
        return Member(offset, *new CType<T>(std::forward<Args>(args)...), true);
    }

    Member(const Member& other)
        : offset_(other.offset())
        , type_(other.managed_ ? *other.type_.clone().release() : other.type_)
        , managed_(other.managed_)
    {}

    Member(Member&& other)
        : offset_(std::move(other.offset_))
        , type_(std::move(other.type_))
        , managed_(std::move(other.managed_))
    {
        other.managed_ = false;
    }

    virtual ~Member()
    {
        if(managed_)
        {
            delete &type_;
        }
    }

    const Type& type() const { return type_; }
    size_t offset() const { return offset_; }
    bool managed() const { return managed_; }

private:
    Member(size_t offset, const Type& type, bool managed)
        : offset_(offset)
        , type_(type)
        , managed_(managed)
    {}

    size_t offset_;
    const Type& type_;
    bool managed_;
};

//=========================== STRUCT =============================
struct Builder
{
    Builder(const std::string& name, const Type& type)
        : name(name)
        , type(type)
    {}

    std::string name;
    const Type& type;
};

class Struct : public Type
{
public:
    Struct(const std::string& name = "")
        : Type(Kind::Struct, name, 0u)
    {};

    Struct(const std::string& name, std::initializer_list<Builder> member_list)
        : Type(Kind::Struct, name, 0u)
    {
        for(auto&& builder: member_list)
        {
            validate_member_creation(name);
            members_.emplace(builder.name, Member::copy(memory_size_, builder.type));
            memory_size_ += builder.type.memory_size();
        }
    };

    Struct(const Struct& other) = default;
    virtual ~Struct() = default;

    void add_member(const std::string& name, const Struct& type)
    {
        validate_member_creation(name);
        members_.emplace(name, Member::ref(memory_size_, type));
        memory_size_ += type.memory_size();
    }

    template<typename T>
    void add_member(const std::string& name, const T& t)
    {
        validate_member_creation(name);
        auto insertion = members_.emplace(name, Member::create_ctype<T>(memory_size_, t));
        memory_size_ += insertion.first->second.type().memory_size();
    }

    template<typename T, typename... Args>
    void add_member(const std::string& name, Args&&... args)
    {
        validate_member_creation(name);
        auto insertion = members_.emplace(name, Member::create_ctype<T>(memory_size_, std::forward<Args>(args)...));
        memory_size_ += insertion.first->second.type().memory_size();
    }

    virtual std::unique_ptr<Type> clone() const override
    {
        return std::unique_ptr<Struct>(new Struct(*this));
    }

    virtual void build_object_at(uint8_t* location) const override
    {
        for(auto&& it: members_)
        {
            it.second.type().build_object_at(location + it.second.offset());
        }
    }

    virtual void destroy_object_at(uint8_t* location) const override
    {
        for(auto&& it: members_)
        {
            it.second.type().destroy_object_at(location + it.second.offset());
        }
    }

    virtual void copy_object(uint8_t* dest_location, uint8_t* src_location) const override
    {
        for(auto&& it: members_)
        {
            it.second.type().copy_object(dest_location + it.second.offset(), src_location + it.second.offset());
        }
    }

    size_t member_size() const { return members_.size(); }


    const Type& operator[](const std::string& name) const
    {
        if(members_.find(name) == members_.end())
        {
            throw MemberAccessException("Struct type '" + this->name() + "' has no member '" + name + "'.");
        }

        return members_.at(name).type();
    }

    const Member* member(const std::string& name) const
    {
        auto it = members_.find(name);
        return it != members_.end() ? &it->second : nullptr;
    }

private:
    bool validate_member_creation(const std::string& name) const
    {
        if(members_.find(name) != members_.end())
        {
            throw MemberAddException("Struct type '" + this->name() + "' has already a member called '" + name + "'.");
        }

        return true;
    }

    std::map<std::string, Member> members_;
};

} //namespace rt

#endif //RT_STRUCT_HPP
