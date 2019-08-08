#ifndef RT__DATA_HPP_
#define RT__DATA_HPP_

#include <runtypes/Struct.hpp>
#include <runtypes/Exception.hpp>

#define RT_NO_COPY_ASSIGNABLE_ERROR(TYPE) \
    RT_STATIC_ERROR_TAG \
    "Type '" #TYPE "' must be copy is_copy_assignable. " \
    "This feature is not mandatory, you can use instead the '" #TYPE "'& get()' way to access the data." \
    "See your '" #TYPE "' instantiation for more details."

namespace rt
{

//=========================== ReadableDataRef =============================
class ReadableDataRef
{
public:
    ReadableDataRef(const ReadableDataRef&) = default;
    virtual ~ReadableDataRef() = default;

    const Type& type() const { return type_; }
    const uint8_t* memory() const { return memory_; }

    ReadableDataRef operator[](const std::string& name) const
    {
        const Member* member = get_member(name);
        return ReadableDataRef(member->type(), memory_ + member->offset());
    }

    template <typename T>
    const T& get() const
    {
        validate_data_type<T>("get");
        return *reinterpret_cast<T*>(memory_);
    }

    template <typename T>
    void get(T& t) const
    {
        static_assert(std::is_copy_assignable<T>::value, RT_NO_COPY_ASSIGNABLE_ERROR(T));
        validate_data_type<T>("get");
        t = *reinterpret_cast<T*>(memory_);
    }

protected:
    ReadableDataRef(const Type& type, uint8_t* memory)
        : type_(type)
        , memory_(memory)
    {}

    const Member* get_member(const std::string& name) const
    {
        if(type_.kind() != Kind::Struct)
        {
            throw MemberAccessException("Type '" + type_.name() + "' has no members.");
        }

        const Member* member = static_cast<const Struct&>(type_).member(name);
        if(!member)
        {
            throw MemberAccessException("Type '" + type_.name() + "' has no member '" + name + "'.");
        }

        return member;
    }

    template <typename T>
    bool validate_data_type(const std::string& method) const
    {
        if(type_.kind() != Kind::CType)
        {
            throw DataAccessException("'" + method + "' can only be called from members with primitive types. "
                   "It was called from type '" + type_.name() + "'.");
        }

        const CType<T>& ctype = static_cast<const CType<T>&>(type_);
        if(typeid(T).hash_code() != ctype.hash_code())
        {
            throw DataAccessException("Type '" + type_.name() + "' differs from '" + typeid(T).name() + "'.");
        }

        return true;
    }

    const Type& type_;
    uint8_t* memory_;
};


//=========================== WritableDataRef =============================
class WritableDataRef : public ReadableDataRef
{
public:
    WritableDataRef(const WritableDataRef&) = default;
    virtual ~WritableDataRef() = default;

    uint8_t* memory() { return memory_; }

    WritableDataRef operator[](const std::string& name)
    {
        const Member* member = get_member(name);
        return WritableDataRef(member->type(), memory_ + member->offset());
    }

    template <typename T>
    T& get_mut()
    {
        validate_data_type<T>("get_mut");
        return *reinterpret_cast<T*>(memory_);
    }

    template <typename T>
    void set(const T& t)
    {
        validate_data_type<T>("set");
        reinterpret_cast<T*>(memory_)->~T();
        new (memory_) T(t);
    }

protected:
    WritableDataRef(const Type& type, uint8_t* memory)
        : ReadableDataRef(type, memory)
    {}
};


//=========================== Data =============================
class Data : public WritableDataRef
{
public:
    Data(const Type& type)
        : WritableDataRef(type, new uint8_t[type.memory_size()])
    {
        type_.build_object_at(memory_);
    }

    Data(const Data& other)
        : WritableDataRef(other.type_, new uint8_t[other.type_.memory_size()])
    {
        type_.copy_object(memory_, other.memory_);
    }

    virtual ~Data()
    {
        type_.destroy_object_at(memory_);
        delete[] memory_;
    }
};


} //namespace rt

#endif //RT__DATA_HPP_
