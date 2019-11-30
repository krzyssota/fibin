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
    
};

struct True { static constexpr bool value = true; };
struct False { static constexpr bool value = false; };

template<class T>
struct Lit<T> ...;

template<>
struct Lit<True> : std::bool_constant<true> {};

template<>
struct Lit<False> : std::bool_constant<false> {};