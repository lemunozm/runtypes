#ifndef RT__TYPE_HPP_
#define RT__TYPE_HPP_

#include <cinttypes>
#include <cstring>
#include <memory>
#include <string>
#include <map>
#include <vector>

namespace rt
{

enum class Kind
{
    Undefined, CType, Struct,
};

//=========================== TYPE =============================
class Type
{
public:
    Kind kind() const { return kind_; };
    const std::string& name() const { return name_; };
    size_t memory_size() const { return memory_size_; }

    virtual void build_object_at(uint8_t* location) const = 0;

protected:
    Type(Kind kind, const std::string& name, size_t memory_size)
        : kind_(kind)
        , name_(name)
        , memory_size_(memory_size)
    {}

private:
    Kind kind_;
    std::string name_;

protected:
    size_t memory_size_;
};

//=========================== CType =============================
template <typename T>
class CType : public Type
{
public:
    CType()
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
    {};

    CType(const T& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , instance_(t)
    {};

    CType(T&& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , instance_(t)
    {};

    virtual void build_object_at(uint8_t* location) const
    {
        std::memcpy(location, &instance_, memory_size());
    }

private:
    T instance_;
};

//=========================== MEMBER =============================
class Member
{
public:
    Member(const Type& type, size_t offset)
        : type_(type), offset_(offset) {}

    const Type& type() const { return type_; }
    size_t offset() const { return offset_; }

private:
    const Type& type_;
    size_t offset_;
};

//=========================== STRUCT =============================
class Struct : public Type
{
public:
    Struct(const std::string& name = "")
        : Type(Kind::Struct, name, 0u) {};

    void add_member(const std::string& name, const Struct& type)
    {
        members_.emplace(name, Member(type, memory_size_));
        memory_size_ += type.memory_size();
    }

    template<typename T>
    void add_member(const std::string& name, T t)
    {
        built_members_.emplace_back(std::make_shared<CType<T>>(t));
        members_.emplace(name, Member(*built_members_.back(), memory_size_));
        memory_size_ += built_members_.back()->memory_size();
    }

    const Type& operator[](const std::string& name) const
    {
        return members_.at(name).type();
    }

    const Member* member(const std::string& name) const
    {
        auto it = members_.find(name);
        return it != members_.end() ? &it->second : nullptr;
    }

    virtual void build_object_at(uint8_t* location) const
    {
        for(auto&& member_it: members_)
        {
            member_it.second.type().build_object_at(location + member_it.second.offset());
        }
    }

private:
    std::vector<std::shared_ptr<Type>> built_members_;
    std::map<std::string, Member> members_;
};

}

#endif //RT__TYPE_HPP_

