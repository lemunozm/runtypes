#ifndef RT__EXCEPTION_HPP_
#define RT__EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace rt
{

#define DEFINE_STRING_BASED_EXCEPTION(TYPE, PARENT) \
    class TYPE##Exception : public PARENT \
    { \
    public: \
        TYPE##Exception(const std::string& message) \
            : PARENT(message) \
        {} \
    };

#define DEFINE_RUNTYPE_EXCEPTION(TYPE) \
    DEFINE_STRING_BASED_EXCEPTION(TYPE, RuntypeException)


DEFINE_STRING_BASED_EXCEPTION(Runtype, std::runtime_error)

DEFINE_RUNTYPE_EXCEPTION(DataAccess)
DEFINE_RUNTYPE_EXCEPTION(MemberAccess)
DEFINE_RUNTYPE_EXCEPTION(MemberAdd)

} //namespace rt

#endif //RT__EXCEPTION_HPP_
