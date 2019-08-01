#ifndef RT__TYPE_HPP_
#define RT__TYPE_HPP_

#include <cinttypes>
#include <memory>
#include <cstring>
#include <map>
#include <vector>
#include <iostream> //Check


namespace rt
{

//=========================== COMMON =============================
enum class Kind
{
    None, CType, Struct,
};

//=========================== TYPE =============================
struct TypeImpl;
template<typename Impl>
class TypeRef
{
public:
    Kind kind() const { return impl_->kind_; }
    const std::string& name() const { return impl_->name_; };
    size_t memory_size() const { return impl_->memory_size_; }


    operator TypeRef<TypeImpl>&()
    {
        return reinterpret_cast<TypeRef<TypeImpl>&>(*this);
    }

    operator const TypeRef<TypeImpl>&() const
    {
        return reinterpret_cast<const TypeRef<TypeImpl>&>(*this);
    }

    virtual void build_object_at(uint8_t* location) const
    {
        std::cout << "build abstract" << std::endl;
        this->build_object_at(location);
        /*
        if(kind() == Kind::CType)
        {
            static_cast<CTypeRef<CTypeImpl>*>(this)->build_object_at(location);
        }
        */
    };

protected:
    template<typename... Args>
    TypeRef(Kind kind, const std::string& name, size_t memory_size, Args&&... args)
        : impl_(std::make_shared<Impl>(kind, name, memory_size, std::forward<Args>(args)...)) {}

    std::shared_ptr<Impl> impl_;

private:
    void* operator new(size_t);
};

struct TypeImpl
{
    TypeImpl(Kind kind, const std::string& name, size_t memory_size)
        : kind_(kind), name_(name), memory_size_(memory_size) {}

    Kind kind_;
    std::string name_;
    size_t memory_size_;
};

template class TypeRef<TypeImpl>;
typedef TypeRef<TypeImpl> Type;
/*
class Type
{
    TypeRef<TypeImpl>& operator->() { return core; }
    const TypeRef<TypeImpl>& operator->() const { return core; }

private:
    TypeRef<TypeImpl> core;
}
*/

//=========================== CType =============================
template<typename T, template<typename> class Impl>
class CTypeRef : public TypeRef<Impl<T>>
{
public:
    CTypeRef()
        : TypeRef<Impl<T>>(Kind::CType, typeid(T).name(), sizeof(T), T())
    {};

    virtual void build_object_at(uint8_t* location) const
    {
        std::cout << "build ctype" << std::endl;
        std::memcpy(location, &this->impl_->ctype_, this->memory_size());
    }
};

template<typename T>
struct CTypeImpl : TypeImpl
{
    CTypeImpl(Kind kind, const std::string& name, size_t memory_size, T ctype)
        : TypeImpl(kind, name, memory_size)
        , ctype_(ctype) {}

    T ctype_;
};

template<typename T>
class CType : public CTypeRef<T, CTypeImpl> { };

//=========================== MEMBER =============================
class Member
{
public:
    Member(const Type& type, size_t offset)
        : type_(type), offset_(offset) {}

    const Type& type() const { return type_; }
    size_t offset() const { return offset_; }

private:
    Type type_;
    size_t offset_;
};

//=========================== STRUCT =============================
template<typename Impl>
class StructRef : public TypeRef<Impl>
{
public:
    StructRef(const std::string& name = "")
        : TypeRef<Impl>(Kind::Struct, name, 0u) {};

    void add_member(const std::string& name, const Type& type)
    {
        this->impl_->members_.emplace(name, Member(type, this->impl_->memory_size_));
        this->impl_->memory_size_ += type.memory_size();
    }

    const Type& operator[](const std::string& name) const
    {
        return this->impl_->members_.at(name).type();
    }

    const Member& member(const std::string& name) const
    {
        return this->impl_->members_.at(name);
    }

    virtual void build_object_at(uint8_t* location) const
    {
        std::cout << "build struct" << std::endl;
        for(auto&& member_it: this->impl_->members_)
        {
            member_it.second.type().build_object_at(location + member_it.second.offset());
            std::cout << member_it.first << std::endl;
        }
    }
};

struct StructImpl : TypeImpl
{
    StructImpl(Kind kind, const std::string& name, size_t memory_size)
        : TypeImpl(kind, name, memory_size) {}

    std::map<std::string, Member> members_;
};

template class StructRef<StructImpl>;
typedef StructRef<StructImpl> Struct;

}

#endif //RT__TYPE_HPP_

