#include "fibin.h"
#include <iostream>
#include <cstdint>
#include <functional>
#include <string>

/*
 WERSJA Z DZIEDZICZENIEM PO INTEGRAL_CONSTANT

template <int n>
struct Fib : std::integral_constant<int, Fib<n- 1>::value + Fib<n - 2>::value> {};

template<>
struct Fib<1> : std::integral_constant<int, 1> {};
template<>
struct Fib<0> : std::integral_constant<int, 0> {};

struct True {};
struct False {};

template<typename T>
struct Lit {};

template <int n>
struct Lit<Fib<n>> : Fib<n>{};

template<>
struct Lit<False> : std::false_type {} ;

template<>
struct Lit<True> : std::true_type {};

template<typename ... Args>
struct Sum {};

template<std::size_t N, std::size_t M, typename ... Args>
struct Sum<Lit<Fib<N>>, Lit<Fib<M>>, Args...> :
    std::integral_constant<std::size_t, Lit<Fib<N>>::value + Lit<Fib<M>>::value + Sum<Args...>()> {};

template<std::size_t N>
struct Sum< Lit<Fib<N>>> : std::integral_constant<std::size_t, Lit<Fib<N>>::value> {};

template<>
struct Sum<> : std::integral_constant<std::size_t, 0> {};

template<typename T>
struct Inc1 {};

template<std::size_t N>
struct Inc1<Lit<Fib<N>>> {
    //... tu chyba przyda się Var albo Ref
};

template<typename T>
struct Inc10 {};

template<std::size_t N>
struct Inc10<Lit<Fib<N>>> {
    //... tu chyba przyda się Var albo Ref
};



int main(){
    static_assert(1 == Lit<Fib<1>>());
    static_assert(1 == Fib<1>());
    static_assert(3 == Sum<Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>>());
    return 0;
}*/

//---------------------------------------------------------
// WERSJA Z POLEM TYPU SIZE_T

template<std::size_t N>
struct Fib {
	static constexpr std::size_t value = Fib<N-1>::value + Fib<N-2>::value;
};
template<>
struct Fib<0> {
	static constexpr std::size_t value = 0;
};
template<>
struct Fib<1> {
	static constexpr std::size_t value = 1;
};

struct True {};
struct False {};


template<typename T>
struct Lit {};

template<std::size_t N>
struct Lit<Fib<N>> {
	static constexpr std::size_t value = Fib<N>::value;
};

template<> struct Lit<True> :public std::true_type {};
template<> struct Lit<False> :public std::false_type {};

//FIXME Czy jesli zmienna ma wiecej niz 6 znakow to ma sie kompilowac czy mozna to sprawdzac w funkcji assertem?
template<std::size_t N>
constexpr unsigned long long Var(const char (&id)[N]) {
	static_assert(sizeof(unsigned long long) == 8);
	unsigned long long ret = 0;
	for(std::size_t i = 0; i < N; i++)
	{
		ret |= static_cast<std::size_t>(static_cast<unsigned char>(id[i])) << (i*8);
	}
	return ret;
}
// TODO var trzeba skonwertować
// template <ulong Var, typename Val, typename Body>
// struct Let {};

/* struct Eval< If<Cond, Then, Else>>
*        using result = Eval<if<Eval<Cond>::result>,Then, Else>>::result
  struct Eval< If<True, Then, Else>
  *     using result = typename Eval<Then>::result
  *struct Eval< If<False, Then, Else>
  *     using result = typename Eval<Else>::result

  *
  *eval<>()
  *
  * eval<T>::result::value
  * // struktura typu closure
  *
  *     struct Envirionment {};
  *
  *     template <Name , Value , Environment>
  *     struct Environment {}
  *
  *     <Environment<y, Lit<2>>, Environment<x, Lit<1>>, Empty>
  *
  *
  * */
/*
 template <typename T>
 class isFib : public std::false_type{};

 template <uint64 n>
 class ifFib<Fib<n>> : std::true_type {};

 template<typename T, typename = typename std::enable_if<is_same<isFib<T>::value || isBool<T>::value)::type



template<unsigned long long varId>
struct Ref {
	static std::size_t value;
};
*/

template< typename ... Args>
struct Eq {};

template<std::size_t N, std::size_t M>
struct Eq<Lit<Fib<N>>, Lit<Fib<M>>>{
	static constexpr bool equal = Lit<Fib<N>>::value == Lit<Fib<M>>::value;
};

template<>
struct Eq<Lit<True>, Lit<True>>{
    static constexpr bool equal = std::bool_constant<true>();
};
template<>
struct Eq<Lit<False>, Lit<False>>{
    static constexpr bool equal = std::bool_constant<true>();
};
template<>
struct Eq<Lit<True>, Lit<False>>{
    static constexpr bool equal = std::bool_constant<false>();
};
template<>
struct Eq<Lit<False>, Lit<True>>{
    static constexpr bool equal = std::bool_constant<false>();
};

template<typename T>
struct Inc1 {};

template<std::size_t N>
struct Inc1<Lit<Fib<N>>> {
	static constexpr std::size_t value = Lit<Fib<N>>::value + Lit<Fib<1>>::value;
};

template<typename T>
struct Inc10 {};

template<std::size_t N>
struct Inc10<Lit<Fib<N>>> {
	static constexpr std::size_t increased = Lit<Fib<N>>::value + Lit<Fib<10>>::value;
};

template< typename ... Args>
struct Sum {};

template<std::size_t N, std::size_t M, typename ... Args>
struct Sum<Lit<Fib<N>>, Lit<Fib<M>>, Args...>{
    static constexpr std::size_t sum =  Lit<Fib<N>>::value + Lit<Fib<M>>::value + Sum<Args...>::sum;
};

template<std::size_t N>
struct Sum<Lit<Fib<N>>> {
    static constexpr std::size_t sum = Lit<Fib<N>>::value;
};

template<>
struct Sum<> {
    static constexpr std::size_t sum = 0;
};



template<typename ValueType>
class Fibin {
public :
    template<typename Expr>
    static constexpr void eval() {
        std::cout << "Fibin doesn't support: PKc\n" << "Fibin works fine!\n";
    }
    template<typename Expr, typename = typename std::enable_if<std::is_integral<ValueType>::value, Expr>::type>
    static constexpr ValueType eval() {
        return 0;
    }


};

int main(){
    Fibin<const char*>::eval<Lit<Fib<0>>>();
    static_assert(1 == Lit<Fib<1>>::value);
    static_assert(3 == Sum<Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>>::sum);
    static_assert(true == Eq<Lit<Fib<1>>, Lit<Fib<1>>>::equal);
    static_assert(1 == Inc1<Lit<Fib<0>>>::value);
    return 0;
}

