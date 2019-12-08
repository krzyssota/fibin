#ifndef JNP1_FIBIN_FIBIN3_H
#define JNP1_FIBIN_FIBIN3_H

#include <iostream>
#include <type_traits>

#define TYPE(...) using type = __VA_ARGS__
#define VAL(...) static constexpr Ret value = __VA_ARGS__

template<typename ...>
struct varList {};

struct True {};
struct False {};

template<typename T, typename ...>
struct getFirst {
    using type = T;
};

template<typename, typename, typename> struct append {};

template<typename id, typename value, typename ... Rest>
struct append<id, value, varList<Rest...>> {
    TYPE(varList<id, value, Rest...>);
};

template<typename, typename>
struct get {};

template<typename id>
struct get<id, varList<>> {};

template<typename id, typename ... Rest>
struct get<id, varList<id, Rest...>> {
    TYPE(typename getFirst<Rest...>::type);
};

template<typename id, typename otherId, typename ... Rest>
struct get<id, varList<otherId, Rest...>> {
    TYPE(typename get<id, varList<Rest...>>::type);
};

template<typename, typename, typename>
struct EvalType {};

template<typename, typename, typename>
struct EvalValue {};

template<std::size_t N>
struct Fib {};

template<typename>
struct Lit {};

template<typename Ret, std::size_t N, typename VarList>
struct EvalType<Ret, Lit<Fib<N>>, VarList> {
    TYPE(Lit<Fib<N>>);
};

template<typename Ret, std::size_t N, typename VarList>
struct EvalValue<Ret, Lit<Fib<N>>, VarList> {
    VAL(EvalValue<Ret, Lit<Fib<N-1>>, VarList>::value +
        EvalValue<Ret, Lit<Fib<N-2>>, VarList>::value);
};

template<typename Ret, typename VarList>
struct EvalValue<Ret, Lit<Fib<0>>, VarList> {
    VAL(0);
};
template<typename Ret, typename VarList>
struct EvalValue<Ret, Lit<Fib<1>>, VarList> {
    VAL(1);
};
template<typename Ret, typename VarList>
struct EvalType<Ret, Lit<True>, VarList> {
    TYPE(True);
};
template<typename Ret, typename VarList>
struct EvalType<Ret, Lit<False>, VarList> {
    TYPE(False);
};

template<typename Expr>
struct Inc1 {};

template<typename Expr>
struct Inc1Evaled {};

template<typename Ret, typename Expr, typename VarList>
struct EvalType<Ret, Inc1<Expr>, VarList> {
    TYPE(Inc1Evaled<typename EvalType<Ret, Expr, VarList>::type>);
};

template<typename Ret, typename Expr, typename VarList>
struct EvalValue<Ret, Inc1Evaled<Expr>, VarList> {
    VAL(EvalValue<Ret, Expr, VarList>::value + EvalValue<Ret, Lit<Fib<1>>, VarList>::value);
};

template<typename Expr>
struct Inc10 {};

template<typename Expr>
struct Inc10Evaled {};

template<typename Ret, typename Expr, typename VarList>
struct EvalType<Ret, Inc10<Expr>, VarList> {
    TYPE(Inc10Evaled<typename EvalType<Ret, Expr, VarList>::type>);
};

template<typename Ret, typename Expr, typename VarList>
struct EvalValue<Ret, Inc10Evaled<Expr>, VarList> {
    VAL(EvalValue<Ret, Expr, VarList>::value + EvalValue<Ret, Lit<Fib<10>>, VarList>::value);
};

template<std::size_t N>
struct Ref {};

template<typename Ret, std::size_t N, typename VarList>
struct EvalType<Ret, Ref<N>, VarList> {
    TYPE(typename get<Ref<N>, VarList>::type);
};

template<typename Ret, std::size_t N, typename VarList>
struct EvalValue<Ret, Ref<N>, VarList> {
    using ret = typename EvalType<Ret, Ref<N>, VarList>::type;
    VAL(EvalValue<Ret, ret, VarList>::value);
};

template<typename ...>
struct Sum {};

template<typename ...>
struct SumEvaled {};

template<typename Ret, typename VarList, typename ... Expr>
struct EvalType<Ret, Sum<Expr...>, VarList> {
    TYPE(SumEvaled<typename EvalType<Ret, Expr, VarList>::type...>);
};

template<typename Ret, auto Value, auto ... Rest>
struct Add {
    VAL(Value + Add<Ret, Rest...>::value);
};

template<typename Ret, auto Value>
struct Add<Ret, Value> {
    VAL(Value);
};

template<typename Ret, typename VarList, typename ... Expr>
struct EvalValue<Ret, SumEvaled<Expr...>, VarList> {
    VAL(Add<Ret, EvalValue<Ret, Expr, VarList>::value...>::value);
};

template<typename, typename>
struct Eq {};

template<bool> struct eq {};

template<> struct eq<true> { TYPE(True); };

template<> struct eq<false> { TYPE(False); };

template<typename, typename>
struct EqEvaled {};

template<typename Ret, typename Left, typename Right, typename VarList>
struct EvalType<Ret, Eq<Left, Right>, VarList> {
    TYPE(EqEvaled<
            typename EvalType<Ret, Left, VarList>::type,
            typename EvalType<Ret, Right, VarList>::type
    >);
};

template<typename Ret, typename Left, typename Right, typename VarList>
struct EvalType<Ret, EqEvaled<Left, Right>, VarList> {
    TYPE(typename eq<
            EvalValue<Ret, Left, VarList>::value ==
            EvalValue<Ret, Right, VarList>::value>
                 ::type);
};

template<typename, typename, typename>
struct If {};

template<typename, typename, typename>
struct IfEvaled {};

template<typename Ret, typename Cond, typename ifTrue, typename ifFalse, typename VarList>
struct EvalType<Ret, If<Cond, ifTrue, ifFalse>, VarList> {
    TYPE(IfEvaled<
            typename EvalType<Ret, Cond, VarList>::type,
            typename EvalType<Ret, ifTrue, VarList>::type,
            typename EvalType<Ret, ifFalse, VarList>::type
    >);
};

template<typename Ret, typename Cond, typename ifTrue, typename ifFalse, typename VarList>
struct EvalType<Ret, IfEvaled<Cond, ifTrue, ifFalse>, VarList> {
    TYPE(typename EvalType<Ret, IfEvaled<
            typename EvalType<Ret, Cond, VarList>::type,
            ifTrue,
            ifFalse
    >, VarList>::type);
};

template<typename Ret, typename ifTrue, typename ifFalse, typename VarList>
struct EvalType<Ret, IfEvaled<True, ifTrue, ifFalse>, VarList> {
    TYPE(ifTrue);
};

template<typename Ret, typename ifTrue, typename ifFalse, typename VarList>
struct EvalType<Ret, IfEvaled<False, ifTrue, ifFalse>, VarList> {
    TYPE(ifFalse);
};

template<typename Ret, typename Cond, typename ifTrue, typename ifFalse, typename VarList>
struct EvalValue<Ret, IfEvaled<Cond, ifTrue, ifFalse>, VarList> {
    using ret = typename EvalType<Ret, IfEvaled<Cond, ifTrue, ifFalse>, VarList>::type;
    VAL(EvalValue<Ret, ret, VarList>::value);
};

template<std::size_t, typename, typename>
struct Let {};

template<std::size_t, typename, typename>
struct LetEvaled {};

template<typename Ret, std::size_t N, typename initExpr, typename retExpr, typename VarList>
struct EvalType<Ret, Let<N, initExpr, retExpr>, VarList> {
    using var = typename EvalType<Ret, initExpr, VarList>::type;
    using new_var_list = typename append<Ref<N>, var, VarList>::type;
    TYPE(LetEvaled<
            N,
            var,
                 typename EvalType<Ret, retExpr, new_var_list>::type>);
};

template<typename Ret, std::size_t N, typename initExpr, typename retExpr, typename VarList>
struct EvalType<Ret, LetEvaled<N, initExpr, retExpr>, VarList> {
    TYPE(retExpr);
};

template<typename Ret, std::size_t N, typename initExpr, typename retExpr, typename VarList>
struct EvalValue<Ret, LetEvaled<N, initExpr, retExpr>, VarList> {
    VAL(EvalValue<Ret, retExpr, typename append<Ref<N>, initExpr, VarList>::type>::value);
};

template<std::size_t, typename>
struct Lambda {};

template<typename ArgId, typename Body, typename VarList>
struct Closure {
    using argId = ArgId;
    using body = Body;
    using envir = VarList;
};

template<typename Ret, std::size_t argId, typename body, typename VarList>
struct EvalType<Ret, Lambda<argId, body>, VarList> {
    TYPE(Closure<
            Ref<argId>,
            typename EvalType<Ret, body, typename append<Ref<argId>, Ref<argId>, VarList>::type>::type,
                 VarList>);
};

template<typename body, typename envir>
struct LambdaToBeInvoked {};

template<typename list1, typename list2> struct merge {};

template<typename ... A1, typename ... A2>
struct merge<varList<A1...>, varList<A2...>> {
    TYPE(varList<A1..., A2...>);
};

template<typename arg2Id, typename Body, typename envir2, typename Envir>
struct LambdaToBeInvoked<Closure<arg2Id, Body, envir2>, Envir> {
    using argId = arg2Id;
    using body = Body;
    using envir = typename merge<Envir, envir2>::type;
};

template<typename Ret, typename Expr, typename envir, typename VarList>
struct EvalType<Ret, LambdaToBeInvoked<Expr, envir>, VarList> {
    TYPE(Expr);
};

template<typename Func, typename param>
struct Invoke {};

template<typename Func, typename param>
struct InvokeEvaled {};

template<typename Ret, typename Func, typename param, typename VarList>
struct EvalType<Ret, Invoke<Func, param>, VarList> {
    TYPE(InvokeEvaled<
            typename EvalType<Ret, Func, VarList>::type,
            typename EvalType<Ret, param, VarList>::type
    >);
};

template<typename Ret, typename argId, typename body, typename envir, typename VarList>
struct EvalType<Ret, Closure<argId, body, envir>, VarList> {
    TYPE(Closure<argId, body, envir>);
};

template<typename Ret, typename Func, typename param, typename VarList>
struct EvalType<Ret, InvokeEvaled<Func, param>, VarList> {
    using func = typename EvalType<Ret, Func, VarList>::type;
    using argId = typename func::argId;
    using body = typename func::body;
    using envir = typename func::envir;
    TYPE(LambdaToBeInvoked<body, typename append<argId, param, envir>::type>);
};

template<typename Ret, typename Func, typename param, typename VarList>
struct EvalValue<Ret, InvokeEvaled<Func, param>, VarList> {
    VAL(EvalValue<
            Ret,
            typename EvalType<Ret, InvokeEvaled<Func, param>, VarList>::type,
                VarList>::value);
};
template<typename Ret, typename Body, typename envir, typename VarList>
struct EvalValue<Ret, LambdaToBeInvoked<Body, envir>, VarList> {
    VAL(EvalValue<Ret, Body,envir>::value);
};

using VarIDType = unsigned long long;

template<std::size_t N>
static constexpr VarIDType Var(const char (&id)[N]) {
    VarIDType ret = 0;
    for(std::size_t i=0; i < N; ++i) {
        char c = id[i];
        if(c >= 'a' && c <= 'z') {
            c ^= 0x20;
        }
        ret |= static_cast<VarIDType>(static_cast<unsigned char>(c)) << (i*8);
    }
    return ret;
}

template<typename ValueType>
class Fibin {
public:
    template<typename Expr, typename V = ValueType,
            typename = typename std::enable_if<!(std::is_integral<V>::value)>::type>
    static constexpr void eval() {
        std::cout << "Fibin doesn't support: " << typeid(V).name() << "\n";
    };

    template<typename Expr, typename V = ValueType,
            typename = typename std::enable_if<std::is_integral<V>::value>::type>
    static constexpr ValueType eval() {
        using evaled = typename EvalType<ValueType, Expr, varList<>>::type;
        return EvalValue<ValueType, evaled, varList<>>::value;
    };
};

#endif //JNP1_FIBIN_FIBIN3_H
