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
        return *reinterpret_cast<T*>(memory_);
    }

protected:
    ReadableDataRef(const Type& type, uint8_t* memory)
        : type_(type)
        , memory_(memory)
    {}

    const Member* get_member(const std::string& name) const
    {
        if(type_.kind() == Kind::Struct)
        {
            const Member* member = static_cast<const Struct&>(type_).member(name);
            if(!member)
            {
                throw NoMemberException("Type '" + type_.name() + "' has no member '" + name + "'");
            }
            return member;
        }
        else
        {
            throw NoMemberException("Type '" + type_.name() + "' has no members");
        }
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
        std::memcpy(memory_, &t, type_.memory_size());
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
