#include <memory>
#include <cinttypes>
#include <map>

namespace rt
{

//=========================== COMMON =============================
enum class Kind
{
    Int8, Int16, Int32, Int64, Float, Double, Struct
};

template<typename I>
class Reference
{
protected:
    template<typename... Args>
    Reference(Args... args) : impl(std::make_shared<I>(args...)) {}

    std::shared_ptr<I> impl;
};


//=========================== TYPE =============================
struct TypeImpl
{
    TypeImpl(Kind kind, int32_t size) : kind(kind), size(size) {}

    Kind kind;
    int32_t size;
};

template<typename Impl>
class TypeRef : public Reference<Impl>
{
public:
    TypeRef(Kind kind, int32_t size) : Reference<Impl>(kind, size) {}

    Kind& kind() { return this->impl->kind; } //we I need this?
    uint32_t& size() { return this->impl->size; } //we I need this?
};

typedef TypeRef<TypeImpl> Type;


//=========================== MEMBER =============================
class Member
{
    Type type;
    int32_t offset;
};

//=========================== STRUCT =============================
struct StructTypeImpl : TypeImpl
{
    StructTypeImpl() : TypeImpl(Kind::Struct, 0u) {}
    std::map<std::string, Member> members;
};

template<typename Impl>
class StructTypeRef : public TypeRef<Impl>
{
public:
    StructTypeRef() : TypeRef<Impl>() {};

    void add_member(const std::string& name, Type type);
    Type operator [](const std::string& name);
};

typedef StructTypeRef<StructTypeImpl> StructType;

//===========================  =============================
}

int main()
{
    return 0;
}


/*
class StructType : public Type
{
public:
};

}

int main()
{
    TypeRef type1 = std::make_shared<rt::StructType>();
    type1->add_member("m_int8", rt::Kind::Int8);
    type1->add_member("m_int32", rt::Kind::Int32);
    type1->add_member("m_float", rt::Kind::Float);

    return 0;
}
*/
