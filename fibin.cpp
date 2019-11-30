#include "fibin.h"
#include <iostream>
#include <cstdint>
#include <functional>


template <int n>
struct Fib : std::integral_constant<int, Fib<n- 1>::value + Fib<n - 2>::value> {};

template<>
struct Fib<1> : std::integral_constant<int, 1> {};
template<>
struct Fib<0> : std::integral_constant<int, 0> {};


template<typename T>
class Fibin{
    using type = T;
};

struct True { static constexpr bool value = true; };
struct False { static constexpr bool value = false; };


template<typename T>
struct Lit {};

template <int n>
struct Lit<Fib<n>> : Fib<n>{};

template<>
struct Lit<False> : std::false_type {} ;

template<>
struct Lit<True> : std::true_type {};

template<typename T, typename U>
struct Eq {

};

template<typename T>
struct Eq<T, T> {

};


int main(){
    static_assert(2 == Lit<Fib<3>>());
    return 0;
}


