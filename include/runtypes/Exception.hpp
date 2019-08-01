#ifndef RT__EXCEPTION_HPP_
#define RT__EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace rt
{

class NoMemberException : public std::runtime_error
{
public:
    NoMemberException(const std::string& message)
        : std::runtime_error(message)
    {}
};

} //namespace rt

#endif //RT__EXCEPTION_HPP_
