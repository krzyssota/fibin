/*#ifndef JNP1_FIBIN_MASTER_FIBIN_H
#define JNP1_FIBIN_MASTER_FIBIN_H*/

#include <iostream>
#include <cstdint>
#include <type_traits>

/*template <uint64_t N>
struct Fib :public std::integral_constant<uint64_t, Fib<N-1>::value + Fib<N-2>::value> {};
template <>
struct Fib<0> : std::integral_constant<uint64_t, 0> {};
template <>
struct Fib<1> : std::integral_constant<uint64_t, 1> {};
 */
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

template<typename Condition, typename Then, typename Else>
struct If {
};

template<typename Left, typename Right>
struct Eq {
};

template<typename ... Args>
struct Sum {
};

template<typename T>
struct Inc1 {
};

template<typename T>
struct Inc10 {
};

template<uint64_t Var>
struct Ref {
};

struct Let {
};

template<uint64_t id, typename Body>
struct Lambda {
};

template<typename Lambda, typename Param>
struct Invoke {
};

struct Empty {
}; // empty environment

template<uint64_t id, typename Value, typename Tail>
struct Env {
};

template<uint64_t id, typename Env>
struct SearchList {
};

template<uint64_t id, typename Value, typename Tail>
struct SearchList<id, Env<id, Value, Tail>> {
    using result = Value; // TODO nie Eval<Value, Env>?
};
template<uint64_t id, uint64_t diff_id, typename Value, typename Tail>
struct SearchList<id, Env<diff_id, Value, Tail>> {
    using result = SearchList<id, Tail>;
};

template<typename T, typename = typename std::enable_if<(isBool<T>::value || isFib<T>::value)>::type>
struct Lit {
};
template<uint64_t N>
struct Lit<Fib<N>> {
};
template<>
struct Lit<True> /*:std::true_type */{
    static constexpr int val = 1;
};
template<>
struct Lit<False> /*:std::true_type */{
    static constexpr int val = 0;
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
        return Eval<Expr, Empty>::result::V::value;
    };

//private:

    template<typename Expr, typename Env>
    struct Eval {
    };

    /*  template<uint64_t n, typename Env>
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
          using result = Eval<Fib<N>, Env>;
      }; */
    // todo tak ma kacper
    template<typename Env>
    struct Eval<Lit<Fib<0>>, Env> {
        //using result = Fib<0>;
        using result = struct V {
            static constexpr ValueType value = 0;
        };
    };
    template<typename Env>
    struct Eval<Lit<Fib<1>>, Env> {
        //using result = Fib<1>;
        using result = struct V {
            static constexpr ValueType value = 1;
        };
    };
    template<uint64_t N, typename Env>
    struct Eval<Lit<Fib<N>>, Env> {
        //using result = Fib<N>;
        using result = struct V {
            static constexpr ValueType value =
                    Eval<Lit<Fib<N - 1>>, Env>::result::V::value + Eval<Lit<Fib<N - 2>>, Env>::result::V::value;
        };
    }; // todo tak bym wolał
    template<typename Env>
    struct Eval<Lit<True>, Env> {
        using result = Lit<True>;
    };
    template<typename Env>
    struct Eval<Lit<False>, Env> {
        using result = Lit<True>;
    };

    // IF
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
        using result =  Eval<Else, Env>;
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
        using result = struct V {
            static const ValueType value =
                    Eval<A, Env>::result::V::value + Eval<B, Env>::result::V::value + Eval<Sum<Args...>, Env>::result::V::value;
        };
    };
    template<typename Env, typename A, typename B>
    struct Eval<Sum<A, B>, Env> {
        using result = struct V {
            static const ValueType value = Eval<A, Env>::result::V::value + Eval<B, Env>::result::V::value;
        };
    };
    template<typename Env, typename A>
    struct Eval<Sum<A>, Env> {
        using result = struct V {
            static const ValueType value = Eval<A, Env>::result::V::value;
        };
    };

    // INC
    template<typename T, typename Env>
    struct Eval<Inc1<T>, Env> {
        using result = typename Eval<Sum<Lit<Fib<1>>, T>, Env>::result;
    };

    template<typename T, typename Env>
    struct Eval<Inc10<T>, Env> {
        using result = typename Eval<Sum<Lit<Fib<10>>, T>, Env>::result;
    };

    // REF
    template<uint64_t id, typename Env>
    struct Eval<Ref<id>, Env> {
        using result = SearchList<id, Env>;
    };

    // LAMBDA
    template<uint64_t id, typename Body, typename Env>
    struct Eval<Lambda<id, Body>, Env> {
        //using result = ...
    };

    // INVOKE
    /*
    template <uint64_t id, typename Body, typename Param, typename Env>
    struct Eval<Invoke<Lambda<id, Body>, Param> {
        using result = Eval<Lambda<id, Body>>
    };
     */
};


int main() {
    /*// Testing: lambda(x) {x + (Fib(1) + Fib(10)) + Fib(2)}(Fib(3))
    // Fib(0) = 0, Fib(1) = 1, Fib(2) = 1, Fib(3) = 2, Fib(10) = 55
    static_assert(59 ==
    Fibin<uint64_t>::eval<
                        Invoke<
                                Lambda<Var("x"), Sum<Ref<Var("x")>, Inc10<Lit<Fib<1>>>, Lit<Fib<2>>> >,
                                Lit<Fib<3>>
                                >
                        >());



    // Testing: if False then Fib(0) else Fib(1)
    static_assert(1 == Fibin<uint8_t>::eval<If<Lit<False>, Lit<Fib<0>>, Lit<Fib<1>>>>());

    // Testing: let z = Fib(0) in {z + Fib(1)}
    static_assert(1 == Fibin<int16_t>::eval<Let<Var("z"), Lit<Fib<0>>, Inc1<Ref<Var("Z")>>>>());*/



    static_assert(1 == Fibin<uint64_t>::eval<Lit<Fib<1>>>(), "Error");
    static_assert(1 == Fibin<uint64_t>::eval<Lit<Fib<1>>>(), "Error");
    static_assert(1 == Fibin<uint64_t>::eval<If<Lit<True>, Lit<Fib<1>>, Lit<Fib<0>> >>(), "Error");

    static_assert(6 == Fibin<int>::eval<Sum< Lit<Fib<2>>, Lit<Fib<3>>, Lit<Fib<4>> >>(), "Error");
    static_assert(6 == Fibin<int>::eval<Sum< Sum<Lit<Fib<2>>, Lit<Fib<3>>>, Lit<Fib<4>> >>(), "Error");

    static_assert(1 == Fibin<uint64_t>::eval<If< Eq<Lit<Fib<1>>,Lit<Fib<1>>> , Lit<Fib<1>> , Lit<Fib<0>> >>(), "Error");

    // Prints out to std::cout: "Fibin doesn't support: PKc"
    Fibin<const char*>::eval<Lit<Fib<0>>>();

    std::cout << "Fibin works fine!" << std::endl;
}

//#endif