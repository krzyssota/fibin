#ifndef JNP1_FIBIN_MASTER_FIBIN_H
#define JNP1_FIBIN_MASTER_FIBIN_H

#include "fibin.h"
#include <iostream>
#include <cstdint>
#include <type_traits>

template <uint64_t N>
struct Fib :public std::integral_constant<uint64_t, Fib<N-1>::value + Fib<N-2>::value> {};
template <>
struct Fib<0> : std::integral_constant<uint64_t, 0> {};
template <>
struct Fib<1> : std::integral_constant<uint64_t, 1> {};

struct True {};
struct False {};

template<typename T>
struct isFib : std::false_type {};
template<uint64_t N>
struct isFib<Fib<N>> : std::true_type {};
template<typename T>
struct isBool : std::false_type {};
template<>
struct isBool<False> : std::true_type {};
template<>
struct isBool<True> : std::true_type {};

template<typename Condition, typename Then, typename Else>
struct If {};

template<typename Left, typename Right>
struct Eq {};

template<typename ... Args>
struct Sum {};

template<typename T>
struct Inc1 {};

template<typename T>
struct Inc10 {};

template <uint64_t Var>
struct Ref {};

struct Let {};

struct Lambda {};

struct Invoke {};

struct Empty {}; // empty environment

template<typename T, typename = typename std::enable_if<(isBool<T>::value || isFib<T>::value)>::type>
struct Lit {
};
template<uint64_t N>
struct Lit<Fib<N>> {};
template<>
struct Lit<True> {};
template<>
struct Lit<False> {};

template<typename ValueType>
class Fibin {

public:
    template<typename Expr, typename V = ValueType,
            typename = typename std::enable_if<!(std::is_integral<V>::value)>::type>
    static constexpr void eval() {
        std::cout << "Fibin doesn't support: PKc\n";
    };

    template<typename Expr = ValueType, typename V = ValueType,
            typename = typename std::enable_if<std::is_integral<V>::value>::type>
    static constexpr ValueType eval() {
        return Eval<Expr, Empty>::result::value;
    };

    template<typename Expr, typename Env>
    struct Eval {
    };

    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        using result = Fib<0>;
    };
    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        using result = Fib<1>;
    };
    template<uint64_t N, typename Env>
    struct Eval<Lit<Fib<N>>, Env> {
        using result = Fib<N>;
    };

    // IF
    template<typename Condition, typename Then, typename Else, typename Env>
    struct Eval<If<Condition, Then, Else>, Env> {
        using result = typename Eval<If<Eval<Condition, Env>, Then, Else>, Env>::result;
    };
    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<True>, Then, Else>, Env> {
        using result = typename Eval<Then, Env>::result;
    };
    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<False>, Then, Else>, Env> {
        using result = typename Eval<Else, Env>::result ;
    };

    // EQ
    template<typename Left, typename Right, typename Env>
    struct Eval<Eq<Left, Right>, Env> {
        using result = Eval<Eq<Eval<Left, Env>, Eval<Right, Env>>, Env>;
    };
    template<typename Sth, typename SthElse, typename Env>
    struct Eval<Eq<Lit<Sth>, Lit<SthElse>>, Env> {
        using result = Lit<False>;
    };
    template<typename Same, typename Env>
    struct Eval<Eq<Lit<Same>, Lit<Same>>, Env> {
        using result = Lit<True>;
    };

    // SUM
    template<typename Env, typename A, typename B, typename ... Args>
    struct Eval<Sum<A, B, Args...>, Env> {
        using result = struct sumN {
            static const ValueType value = Eval<A, Env>::result::value + Eval<B, Env>::result::value + Eval<Sum<Env, Args...>, Env>::result::value;
        };
    };
    template<typename Env, typename A, typename B>
    struct Eval<Sum<Env, A, B>, Env> {
        using result = struct sum2 {
            static const ValueType value = Eval<A, Env>::result::value + Eval<B, Env>::result::value;
        };
    };
    template<typename Env, typename A>
    struct Eval<Sum<Env, A>, Env> {
        using result = struct sum1 {
            static const ValueType value = Eval<A, Env>::result::value;
        };
    };

    // INC
    template <typename T, typename Env>
    struct Eval<Inc1<T>, Env> {
        using result = typename Eval<Sum<Lit<Fib<1>>, T>, Env>::result;
    };

    template <typename T, typename Env>
    struct Eval<Inc10<T>, Env> {
        using result = typename Eval<Sum<Lit<Fib<10>>, T>, Env>::result ;
    };
};

int main() {
    static_assert(6 == Fibin<int>::eval<Sum< Lit<Fib<2>>, Sum<Lit<Fib<3>>, Lit<Fib<4>>> >>());
}

#endif //JNP1_FIBIN_MASTER_FIBIN_H
