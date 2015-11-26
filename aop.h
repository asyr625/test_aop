#ifndef AOP_H
#define AOP_H

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

#include "non_copyable.h"
template<typename Func, typename... Args>
class Aspect : public Non_Copyable
{
                          public:
                          Aspect(Func&& f) : m_func(std::forward<Func>(f))
{}
                          template<typename T>
                          typename std::enable_if<has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type invoke(Args&&... args,T&& aspect)
{
                          aspect.Before(std::forward<Args>(args)...);
                          m_func(std::forward<Args>(args)...);
                          aspect.After(std::forward<Args>(args)...);
                          }

                          template<typename T>
                          typename std::enable_if<has_member_Before<T, Args...>::value && !has_member_After<T, Args...>::value>::type invoke(Args&&... args,T&& aspect)
{
                          aspect.Before(std::forward<Args>(args)...);
                          m_func(std::forward<Args>(args)...);
                          }

                          template<typename T>
                          typename std::enable_if<!has_member_Before<T, Args...>::value && has_member_After<T, Args...>::value>::type invoke(Args&&... args,T&& aspect)
{
                          m_func(std::forward<Args>(args)...);
                          aspect.After(std::forward<Args>(args)...);
                          }

                          template<typename Head, typename... Tail>
                          void invoke(Args&&... args, Head&&headAspect, Tail&&... tailAspect)
{
                          headAspect.Before(std::forward<Args>(args)...);
                          invoke(std::forward<Args>(args)..., std::forward<Tail>(tailAspect)...);
                          headAspect.After(std::forward<Args>(args)...);
                          }
private:
Func    m_func;
};


template<typename T> using identity_t = T;

template<typename... AP, typename... Args, typename Func>
void invoke(Func&& f, Args&&... args)
{
    Aspect<Func, Args...> asp(std::forward<Func>(f));
    asp.invoke(std::forward<Args>(args)..., identity_t<AP>()...);
}


#endif //AOP_H
