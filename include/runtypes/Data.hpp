#ifndef RT__DATA_HPP_
#define RT__DATA_HPP_

#include <runtypes/Struct.hpp>
#include <runtypes/Exception.hpp>

#include <memory>

namespace rt
{

//=========================== ReadableDataRef =============================
class ReadableDataRef
{
public:
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

protected:
    ReadableDataRef(const Type& type, uint8_t* memory)
        : type_(type)
        , memory_(memory)
    {}

    const Member* get_member(const std::string& name) const
    {
        if(type_.kind() != Kind::Struct)
        {
            throw DataAccessException("Type '" + type_.name() + "' has no members.");
        }

        const Member* member = static_cast<const Struct&>(type_).member(name);
        if(!member)
        {
            throw DataAccessException("Type '" + type_.name() + "' has no member '" + name + "'.");
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
    uint8_t* memory() { return memory_; }

    WritableDataRef operator[](const std::string& name)
    {
        const Member* member = get_member(name);
        return WritableDataRef(member->type(), memory_ + member->offset());
    }

    template <typename T>
    void set(T&& t)
    {
        validate_data_type<T>("set");
        std::memcpy(memory_, &t, sizeof(T));
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
        type.build_object_at(memory_);
    }

    virtual ~Data()
    {
        delete[] memory_;
    }
};


} //namespace rt

#endif //RT__DATA_HPP_
