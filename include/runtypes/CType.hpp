#ifndef RT__CTYPE_HPP_
#define RT__CTYPE_HPP_

#include <runtypes/Type.hpp>

#include <cstring>
#include <typeinfo>

namespace rt
{

template <typename T>
class CType : public Type
{
public:
    CType()
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
    {};

    CType(const T& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , instance_(t)
        , hash_code_(typeid(T).hash_code())
    {};

    CType(T&& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , instance_(t)
        , hash_code_(typeid(T).hash_code())
    {};

    size_t hash_code() const { return hash_code_; }

    virtual void build_object_at(uint8_t* location) const
    {
        std::memcpy(location, &instance_, memory_size());
    }

private:
    T instance_;
    size_t hash_code_;
};

} //namespace rt

#endif //RT__CTYPE_HPP_
