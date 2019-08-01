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
        , hash_code_(typeid(T).hash_code())
        , base_instance_(t)
    {};

    CType(T&& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(t)
    {};

    size_t hash_code() const { return hash_code_; }

    virtual void build_object_at(uint8_t* location) const
    {
        T instance = base_instance_;
        std::memcpy(location, &instance, memory_size());
    }

private:
    size_t hash_code_; //Must be before instance
    T base_instance_;
};

} //namespace rt

#endif //RT__CTYPE_HPP_
