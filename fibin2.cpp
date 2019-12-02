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


/*template<typename ValueType>
class Fibin {};

class Fibin <typename = typename std::enable_if_t<!std::is_integral_v<ValueType>>> {
    static constexpr void eval() {
        std::cout << "Fibin doesn't support: PKc\n" << "Fibin works fine!\n";
    }

};*/
template<typename ValueType>
class Fibin {

    template<typename T>
    struct Eval {
    };

    template<typename Condition, typename Then, typename Else>
    struct Eval<If<Condition, Then, Else>> {
        using result = typename Eval<If<Eval<Condition>, Then, Else>>::result ; // bylo typename ::result
    };

    template<typename Then, typename Else>
    struct Eval<If<True, Then, Else>> {
        using result = typename Eval<Then>::result;
    };
    template<typename Then, typename Else>
    struct Eval<If<False, Then, Else>> {
        using result = typename Eval<Else>::result;
    };

    template<typename Left, typename Right>
    struct Eval<Eq<Left, Right>> {
        using result = typename Eval<Eq<Eval<Left>, Eval<Right>>>::result;
    };
    template<typename T>
    struct Eval<Eq<T, T>> {
        using result = typename std::true_type;
    }; // TODO nie zworci nigdy false

    template<typename T>
    struct Eval<Lit<T>> {
        using result = typename Eval<Lit<Eval<T>>>::result;
    };
    template<uint64_t N>
    struct Eval<Lit<Fib<N>>> {
        using result = Eval<Fib<N>>;
    };
    template<>
    struct Eval<Fib<0>> {
        static constexpr ValueType value = 0;
    };
    template<>
    struct Eval<Fib<1>> {
        static constexpr ValueType value = 1;
    };
    template<uint64_t N>
    struct Eval<Fib<N>> {
        using result = std::integral_constant<ValueType, Eval<Fib<N-1>>::value + Eval<Fib<N-2>>::value>;
    };

    template<>
    struct Eval<True> {
        static constexpr ValueType value = true;
    };
    template<>
    struct Eval<False> {
        static constexpr ValueType value = false;
    };
    template<>
    struct Eval<Lit<True>> {
        using result = Eval<True>;
    };
    template<>
    struct Eval<Lit<False>> {
        using result = Eval<False>;
    };

    public:
    template <typename T>
    static constexpr ValueType eval(){
        return Eval<T>::result::value;
    }
};

int main() {
    static_assert(1 == Fibin<uint64_t>::eval<Lit<Fib<1>>>());
    static_assert(0 == Fibin<uint64_t>::eval<If<Lit<True>, Fib<0>, Fib<1>>>());
    return 0;
}