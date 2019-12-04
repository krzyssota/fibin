#include "fibin.h"
#include <iostream>
#include <cstdint>
#include <type_traits>


template<uint64_t N>
struct Fib {
};
struct True {
};
struct False {
};

template<typename T>
struct isFib : std::false_type {
};
template<uint64_t N>
struct isFib<Fib<N>> : std::true_type {
};
template<typename T>
struct isBool : std::false_type {
};
template<>
struct isBool<False> : std::true_type {
};
template<>
struct isBool<True> : std::true_type {
};

template<typename ... Args>
struct Sum {
};

template<typename Left, typename Right>
struct Eq {
};

struct Ref {
};
struct Let {
};

template<typename Condition, typename Then, typename Else>
struct If {
};

struct Lambda {
};
struct Invoke {
};

struct Empty {
}; // empty environment

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

    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        //static constexpr ValueType value = 0;
        using result = struct LF0 {
            static constexpr ValueType value = 0;
        };
    };
    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        //static constexpr ValueType value = 1;
        using result = struct LF1 {
            static constexpr ValueType value = 1;
        };
    };
    template<uint64_t N, typename Env>
    struct Eval<Lit<Fib<N>>, Env> {
        //using result = typename Eval<Lit<Fib<0>>, Env>::result; + Eval<Lit<Fib<1>>>, Env>::result;
        using result = struct LFN {
            static constexpr ValueType value = Eval<Lit<Fib<N - 1>>, Env>::value + Eval<Lit<Fib<N - 2>>, Env>::value;
        };
    };

    template<typename Condition, typename Then, typename Else, typename Env>
    struct Eval<If<Condition, Then, Else>, Env> {
        using result = Eval<If<Eval<Condition, Env>, Then, Else>, Env>;
    };

    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<True>, Then, Else>, Env> {
        using result = Eval<Then, Env>;
    };
    template<typename Then, typename Else, typename Env>
    struct Eval<If<Lit<False>, Then, Else>, Env> {
        using result = Eval<Else, Env>;
    };


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

    template<typename Env, typename A, typename B, typename ... Args>
    struct Eval<Sum<A, B, Args>, Env> {
        using result = struct sum {
            static const ValueType value = Eval<A, Env>::value + Eval<B, Env>::value + Eval<Sum<Env, Args>>::value;
        };
    };
    template<typename Env, typename A, typename B>
    struct Eval<Sum<Env, A, B>, Env> {
        using result = struct sum {
            static const ValueType value = Eval<A, Env>::value + Eval<B, Env>::value;
        };
    };
    template<typename Env, typename A>
    struct Eval<Sum<Env, A>, Env> {
        using result = struct sum {
            static const ValueType value = Eval<A, Env>::value;
        };
    };


};


int main() {
    static_assert(1 == Fibin<uint64_t>::eval<Lit<Fib<1>>>());
    static_assert(1 == Fibin<uint64_t>::eval<If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>> >>());


// Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char *>::eval<Lit<Fib<0>>>();
    std::cout << "Fibin works fine!" << std::endl;
    return 0;
}