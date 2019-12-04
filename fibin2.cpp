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
struct Lit<Fib<N>> {
};
template<>
struct Lit<True> {
};
template<>
struct Lit<False> {
};


/*constexpr uint64_t Var(const char* name) {
    uint64_t id = 0;
    for(std::size_t i = 0; i < 6; i++)
    {
        id |= static_cast<std::size_t>(static_cast<unsigned char>(id[i])) << (i * 8);
    }
    return id;
}*/


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

//private:

    template<typename Expr, typename Env>
    struct Eval {
    };

    /*template<uint64_t n, typename Env>
    struct Eval<Fib<n>, Env> {
        static constexpr ValueType value = Eval<Fib<n - 1>, Env>::value + Eval<Fib<n - 2>, Env>::value;
    };

    template<typename Env>
    struct Eval<Fib<1>, Env> {
        static const ValueType value = 1;
    };

    template<typename Env>
    struct Eval<Fib<0>, Env> {
        static const ValueType value = 0;
    };
    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        using result = Eval<Fib<0>, Env>;
    };
    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        using result = Eval<Fib<1>, Env>;
    };
    template<uint64_t N, typename Env>
    struct Eval<Lit<Fib<N>>, Env> {
        //using result = typename Eval<Lit<Fib<0>>, Env>::result; + Eval<Lit<Fib<1>>>, Env>::result;
        using result = Eval<Fib<N>, Env>;
    };*/ // todo tak ma kacper
    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        using result = Fib<0>;
        /*using result = struct LF0 {
            static constexpr ValueType value = 0;
        };*/
    };
    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        using result = Fib<1>;
        /*using result = struct LF1 {
            static constexpr ValueType value = 1;
        };*/
    };
    template<uint64_t N, typename Env>
    struct Eval<Lit<Fib<N>>, Env> {
        using result = Fib<N>;
        /*using result = struct LFN {
            static constexpr ValueType value = Eval<Lit<Fib<N - 1>>, Env>::result::value + Eval<Lit<Fib<N - 2>>, Env>::result::value;
        };*/
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
            static const ValueType value = Eval<A, Env>::result::value + Eval<B, Env>::result::value + Eval<Sum<Args...>, Env>::result::value;
        };
    };
    template<typename Env, typename A, typename B>
    struct Eval<Sum<A, B>, Env> {
        using result = struct sum2 {
            static const ValueType value = Eval<A, Env>::result::value + Eval<B, Env>::result::value;
        };
    };
    template<typename Env, typename A>
    struct Eval<Sum<A>, Env> {
        using result = struct sum1 {
            static const ValueType value = Eval<A, Env>::result::value;
        };
    };
    template<typename Env>
    struct Eval<Sum<>, Env> {
        using result = struct sum0 {
            static const ValueType value = 0;
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

    // REF
    /*template <type, typename Env>
    struct Eval<Ref<*/
};




int main() {
    //static_assert(1 == Fibin<uint64_t>::eval<Lit<Fib<1>>>());
    //static_assert(1 == Fibin<uint64_t>::eval<If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>> >>());
    //static_assert(6 == Fibin<int>::eval<Sum< Lit<Fib<2>>, Lit<Fib<3>>, Lit<Fib<4>> >>());

    static_assert(1 == Fibin<uint64_t>::eval<If< Eq< Lit<Fib<1>>, Lit<Fib<1>> >, Lit<Fib<1>>, Lit<Fib<0>> >>());

    //static_assert(6 == Fibin<int>::eval<Sum< Lit<Fib<2>>, Sum<Lit<Fib<3>>, Lit<Fib<4>>> >>());
    //static_assert(6 == Fibin<int>::eval<Sum< Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>>, Lit<Fib<1>> >>());






// Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char *>::eval<Lit<Fib<0>>>();
    std::cout << "Fibin works fine!" << std::endl;
    return 0;
}