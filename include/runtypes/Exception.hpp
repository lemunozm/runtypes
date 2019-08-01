#ifndef RT__EXCEPTION_HPP_
#define RT__EXCEPTION_HPP_

#include <stdexcept>
#include <string>

namespace rt
{

#define RT_DEFINE_STRING_BASED_EXCEPTION(TYPE, PARENT) \
    class TYPE##Exception : public PARENT \
    { \
    public: \
        TYPE##Exception(const std::string& message) \
            : PARENT(message) \
        {} \
    };

#define RT_DEFINE_RUNTYPE_EXCEPTION(TYPE) \
    RT_DEFINE_STRING_BASED_EXCEPTION(TYPE, RuntypeException)


RT_DEFINE_STRING_BASED_EXCEPTION(Runtype, std::runtime_error)

RT_DEFINE_RUNTYPE_EXCEPTION(DataAccess)
RT_DEFINE_RUNTYPE_EXCEPTION(MemberAccess)
RT_DEFINE_RUNTYPE_EXCEPTION(MemberAdd)

} //namespace rt

#endif //RT__EXCEPTION_HPP_
