#ifndef AOP_DEFINE_H
#define AOP_DEFINE_H

#include <functional>

#define HAS_MEMBER(member)\
    template<typename T, typename...Args> struct has_member_##member\
{\
    private:\
    template<typename U>static auto check(int) -> decltype(std::declval<U>().member(std::declval<Args>()...), std::true_type());\
    template<typename U> static std::false_type check(...);\
    public:\
    enum {value = std::is_same<decltype(check<T>(0)), std::true_type>::value};\
    };\

HAS_MEMBER(Foo)
HAS_MEMBER(Before)
HAS_MEMBER(After)

#endif // AOP_DEFINE_H
