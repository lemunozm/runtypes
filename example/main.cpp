#include <memory>
#include <cinttypes>
#include <map>

namespace rt
{

//=========================== COMMON =============================
enum class Kind
{
    None,
    Bool, Char,
    Int8, Int16, Int32, Int64,
    Uint8, Uint16, Uint32, Uint64,
    Float, Double,
    Array, Sequence, Map, Struct,
};

//=========================== TYPE =============================
class TypeImpl;
template<typename Impl>
class TypeRef
{
public:
    Kind kind() const { return this->impl->kind; }
    size_t memory_size() const { return this->impl->memory_size; }

    operator TypeRef<TypeImpl>&()
    {
        return reinterpret_cast<TypeRef<TypeImpl>&>(*this);
    }

    operator const TypeRef<TypeImpl>&() const
    {
        return reinterpret_cast<const TypeRef<TypeImpl>&>(*this);
    }

protected:
    template<typename... Args>
    TypeRef(Kind kind, size_t memory_size, Args&&... args)
        : impl(std::make_shared<Impl>(kind, memory_size, std::forward<Args>(args)...)) {}

    std::shared_ptr<Impl> impl;

private:
    void* operator new(size_t);
};

struct TypeImpl
{
    TypeImpl(Kind kind, size_t memory_size)
        : kind(kind), memory_size(memory_size) {}

    Kind kind;
    size_t memory_size;
};

template class TypeRef<TypeImpl>;
typedef TypeRef<TypeImpl> Type;


//=========================== PRIMITIVES =============================
template<typename T>
struct PrimitiveKind { static constexpr Kind kind = Kind::None; };

template<typename T>
class Primitive : public Type
{
public:
    Primitive() : Type(PrimitiveKind<T>::kind, sizeof(T)) {};
};

#define RT_CREATE_PRIMITIVE_TYPE(CTYPE, KIND) \
    template<> \
    struct PrimitiveKind<CTYPE> { static constexpr Kind kind = Kind::KIND; };

RT_CREATE_PRIMITIVE_TYPE(uint8_t, Uint8);
RT_CREATE_PRIMITIVE_TYPE(uint16_t, Uint16);
RT_CREATE_PRIMITIVE_TYPE(uint32_t, Uint32);

//=========================== Collection  =============================
template<typename Impl>
class CollectionRef : public TypeRef<Impl>
{
protected:
    CollectionRef(Kind kind, size_t memory_size, const Type& inner_type, size_t size)
        : TypeRef<Impl>(kind, memory_size, inner_type, size) {};

public:
    const Type& inner_type() const { return this->impl->inner_type; }
    size_t size() const { return this->impl->size; }
};

struct CollectionImpl : TypeImpl
{
    CollectionImpl(Kind kind, size_t memory_size, const Type& inner_type, size_t size)
        : TypeImpl(kind, memory_size)
        , inner_type(inner_type)
        , size(size) {}

    Type inner_type;
    size_t size;
};

template class CollectionRef<CollectionImpl>;
typedef CollectionRef<CollectionImpl> Collection;


//=========================== ARRAY =============================

class Array : public Collection
{
public:
    Array(const Type& inner_type, size_t size)
        : Collection(Kind::Array, inner_type.memory_size() * size, inner_type, size) {};
};


//=========================== SEQUENCE =============================
class Sequence : public Collection
{
public:
    Sequence(const Type& inner_type, size_t size)
        : Collection(Kind::Sequence, inner_type.memory_size() * size + sizeof(size_t), inner_type, size) {};
};


//=========================== MEMBER =============================
struct Member
{
    Member(const Type& type, size_t offset)
        : type(type), offset(offset) {}

    Type type;
    size_t offset;
};


//=========================== STRUCT =============================
template<typename Impl>
class StructRef : public TypeRef<Impl>
{
protected:
    template<typename... Args>
    StructRef(const std::string& name, Args&&... args)
        : TypeRef<Impl>(Kind::Struct, 0u, name, std::forward<Args>(args)...) {};

public:
    StructRef(const std::string& name = "")
        : TypeRef<Impl>(Kind::Struct, 0u, name) {};

    const std::string& name() const { return this->impl->name; }

    void add_member(const std::string& name, const Type& type)
    {
        this->impl->members.emplace(name, Member(type, 0u)); //Modify 0 value with the offset
        this->impl->memory_size += type.memory_size();
    }

    const Type& operator [](const std::string& name) const
    {
        return this->impl->members.at(name).type;
    }
};

struct StructImpl : TypeImpl
{
    StructImpl(Kind kind, size_t memory_size, const std::string& name)
        : TypeImpl(kind, memory_size)
        , name(name) {}

    std::string name;
    std::map<std::string, Member> members;
};

template class StructRef<StructImpl>;
typedef StructRef<StructImpl> Struct;


}

#include <iostream>

int main()
{
    rt::Type p_uint32 = rt::Primitive<uint32_t>();
    rt::Array a_uint32 = rt::Array(p_uint32, 4);
    rt::Sequence s_s_uint32 = rt::Sequence(a_uint32, 8);
    rt::Struct t_ps = rt::Struct("t_ps");
    t_ps.add_member("p_uint32", p_uint32);
    t_ps.add_member("a_uint32", a_uint32);
    rt::Struct t_pt = rt::Struct("t_pt");
    t_pt.add_member("p_uint32", p_uint32);
    t_pt.add_member("t_ps", t_ps);

    std::cout << p_uint32.memory_size() << std::endl;
    std::cout << a_uint32.memory_size() << std::endl;
    std::cout << s_s_uint32.memory_size() << std::endl;
    std::cout << t_ps.memory_size() << std::endl;
    std::cout << t_pt.memory_size() << std::endl;

    return 0;
}

