// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.
//
// This is an adapted version of the "Calc1" example in Proto's documentation
// to work with compile-time integers too.

#include <boost/hana/integral_constant.hpp>

#include <boost/proto/context.hpp>
#include <boost/proto/core.hpp>

#include <cassert>
namespace proto = boost::proto;
namespace hana = boost::hana;
using namespace hana::literals;


template <int n>
struct placeholder { };

// Define some placeholders
proto::terminal<placeholder<1>>::type const _1 = {{}};
proto::terminal<placeholder<2>>::type const _2 = {{}};

// Define a calculator context, for evaluating arithmetic expressions
template <typename M, typename N>
struct calculator_context
  : proto::callable_context<calculator_context<M, N> const>
{
  // The values bound to the placeholders
  M m;
  N n;

  constexpr calculator_context(M m, N n) : m{m}, n{n} { }

  // The result of evaluating arithmetic expressions
  template <typename Sig>
  struct result;

  template <typename This, typename Terminal>
  struct result<This(Terminal, placeholder<1> const&)> {
    using type = M;
  };

  template <typename This, typename Terminal>
  struct result<This(Terminal, placeholder<2> const&)> {
    using type = N;
  };

  // Handle the evaluation of the placeholder terminals
  constexpr auto operator()(proto::tag::terminal, placeholder<1>) const
  { return m; }

  constexpr auto operator()(proto::tag::terminal, placeholder<2>) const
  { return n; }
};

template <typename Expr, typename M, typename N>
constexpr auto evaluate(Expr expr, M m, N n) {
  // Create a calculator context with d1 and d2 substituted for _1 and _2
  calculator_context<M, N> const ctx{m, n};

  // Evaluate the calculator expression with the calculator_context
  return proto::eval(expr, ctx);
}

int main() {
// sample(proto)
auto expr = (_1 - _2) / _2;

// compile-time computations
static_assert(decltype(evaluate(expr, 6_c, 2_c))::value == 2, "");

// runtime computations
int i = 6, j = 2;
assert(evaluate(expr, i, j) == 2);
// end-sample
}
