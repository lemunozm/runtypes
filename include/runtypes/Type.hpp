#ifndef RT__TYPE_HPP_
#define RT__TYPE_HPP_

#include <cinttypes>
#include <string>

namespace rt
{

//=========================== Kind =============================
enum class Kind
{
    Undefined, CType, Struct,
};

//=========================== Type =============================
class Type
{
public:
    virtual ~Type() = default;

    virtual Type* clone() const = 0;

    virtual void build_object_at(uint8_t* location) const = 0;
    virtual void destroy_object_at(uint8_t* location) const = 0;
    virtual void copy_object(uint8_t* dest_location, uint8_t* src_location) const = 0;

    Kind kind() const { return kind_; };
    const std::string& name() const { return name_; };
    size_t memory_size() const { return memory_size_; }

protected:
    Type(Kind kind, const std::string& name, size_t memory_size)
        : kind_(kind)
        , name_(name)
        , memory_size_(memory_size)
    {}

private:
    Kind kind_;
    std::string name_;

protected:
    size_t memory_size_;
};

} //namespace rt

#endif //RT__TYPE_HPP_

