#ifndef RT__CTYPE_HPP_
#define RT__CTYPE_HPP_

#include <runtypes/Type.hpp>
#include <runtypes/Exception.hpp>

#include <typeinfo>
#include <type_traits>

#define RT_NO_COPY_CONSTRUCTIBLE_ERROR(TYPE) \
    RT_STATIC_ERROR_TAG \
    "Type '" #TYPE "' must be copy constructible. " \
    "See your '" #TYPE "' instantiation for more details."

namespace rt
{

template <typename T>
class CType : public Type
{
public:
    CType(const CType& other)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(other.base_instance_)
    {
        static_assert(std::is_copy_constructible<T>::value, RT_NO_COPY_CONSTRUCTIBLE_ERROR(T));
    };

    CType(const T& t)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(t)
    {
        static_assert(std::is_copy_constructible<T>::value, RT_NO_COPY_CONSTRUCTIBLE_ERROR(T));
    };

    template<typename... Args>
    CType(Args&&... args)
        : Type(Kind::CType, typeid(T).name(), sizeof(T))
        , hash_code_(typeid(T).hash_code())
        , base_instance_(std::forward<Args>(args)...)
    {
        static_assert(std::is_copy_constructible<T>::value, RT_NO_COPY_CONSTRUCTIBLE_ERROR(T));
    };

    virtual ~CType() = default;

    virtual std::unique_ptr<Type> clone() const override
    {
        return std::unique_ptr<Type>(new CType(*this));
    }

    virtual void build_object_at(uint8_t* location) const override
    {
        new (location) T(base_instance_);
    }

    virtual void destroy_object_at(uint8_t* location) const override
    {
        reinterpret_cast<T*>(location)->~T();
    }

    virtual void copy_object(uint8_t* dest_location, uint8_t* src_location) const override
    {
        new (dest_location) T(*reinterpret_cast<T*>(src_location));
    }

    size_t hash_code() const { return hash_code_; }
    const T& base_instance() const { return base_instance_; }

private:
    size_t hash_code_; //Must be placed before instance
    T base_instance_;
};

} //namespace rt

#endif //RT__CTYPE_HPP_
