#ifndef RT__CTYPE_HPP_
#define RT__CTYPE_HPP_

#include <runtypes/Type.hpp>

#include <typeinfo>
#include <type_traits>

#define RT_NO_COPY_CONSTRUCTIBLE_ERROR(TYPE) \
    "Runtype error: Type '" #TYPE "' must be copy constructible. " \
    "See your '" #TYPE "' instantiation for more details."

namespace rt
{

template <typename T>
class CType : public Type
{
public:
    CType(const T& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(t)
    {
        static_assert(std::is_copy_constructible<T>::value, RT_NO_COPY_CONSTRUCTIBLE_ERROR(T));
    };

    template<typename... Args>
    CType(Args... args)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(std::forward<Args>(args)...)
    {
        static_assert(std::is_copy_constructible<T>::value, RT_NO_COPY_CONSTRUCTIBLE_ERROR(T));
    };

    size_t hash_code() const { return hash_code_; }
    const T& base_instance() const { return base_instance_; }

    virtual void build_object_at(uint8_t* location) const
    {
        new (location) T(base_instance_);
    }

    virtual void destroy_object_at(uint8_t* location) const
    {
        reinterpret_cast<T*>(location)->~T();
    }

    virtual void copy_object(uint8_t* dest_location, uint8_t* src_location) const
    {
        new (dest_location) T(*reinterpret_cast<T*>(src_location));
    }

private:
    size_t hash_code_; //Must be placed before instance
    T base_instance_;
};

} //namespace rt

#endif //RT__CTYPE_HPP_
