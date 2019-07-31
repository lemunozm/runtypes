#ifndef RT__DATA_HPP_
#define RT__DATA_HPP_

#include <cinttypes>
#include <memory>

#include <runtypes/Type.hpp>

namespace rt
{

class Data
{
public:
    Data(const Type& type)
        : type_(type)
        , memory_(new uint8_t[type_.memory_size()], std::default_delete<uint8_t[]>())
    {
        type.build_object_at(memory_.get());
    }

    Data(const Type& type, uint8_t* memory)
        : type_(type)
        , memory_(memory, [](uint8_t*){})
    {}

    const Type& type() const { return type_; }

    uint8_t* memory() { return memory_.get(); }
    const uint8_t* memory() const { return memory_.get(); }

    /*
    template<typename T>
    T operator[](const std::string& type)
    {
        //TODO
    }
    */

private:
    Type type_;
    std::shared_ptr<uint8_t> memory_;
};

}

#endif //RT__DATA_HPP_
