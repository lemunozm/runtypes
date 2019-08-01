#ifndef RT__STRUCT_HPP_
#define RT__STRUCT_HPP_

#include <runtypes/CType.hpp>
#include <memory>
#include <map>
#include <vector>

namespace rt
{

//=========================== MEMBER =============================
class Member
{
public:
    Member(const Type& type, size_t offset)
        : type_(type), offset_(offset)
    {}

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
        : Type(Kind::Struct, name, 0u)
    {};

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
        for(auto&& it: members_)
        {
            it.second.type().build_object_at(location + it.second.offset());
        }
    }

private:
    std::vector<std::shared_ptr<Type>> built_members_;
    std::map<std::string, Member> members_;
};

} //namespace rt

#endif //RT_STRUCT_HPP
