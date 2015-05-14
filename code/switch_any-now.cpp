// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>

#include <cassert>
#include <functional>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <utility>
#include <vector>

#include <boost/any.hpp>
namespace hana = boost::hana;


template <typename T>
auto case_ = [](auto f) {
  return std::make_pair(hana::type<T>, f);
};

struct _default;
auto default_ = case_<_default>;
auto empty = case_<void>;


// sample(switch_any-now-impl3)
template <typename Result, typename Any, typename Default>
Result switchAnyImpl(Any&, std::type_index const& t, Default& default_) {
  return default_();
}
// end-sample

// sample(switch_any-now-impl2)
template <typename Result, typename Any, typename Default, typename Case, typename ...Rest>
Result switchAnyImpl(Any& a, std::type_index const& t, Default& default_,
                   Case& case_, Rest& ...rest)
{
  using T = typename decltype(+case_.first)::type;
  if (t == typeid(T)) {
    return hana::eval_if(hana::type<T> == hana::type<void>,
      [&](auto _) { return _(case_).second(); },
      [&](auto _) { return case_.second(*boost::unsafe_any_cast<T>(_(&a))); }
    );
  }
  else
    return switchAnyImpl<Result>(a, t, default_, rest...);
}
// end-sample

// sample(switch_any-now-impl1)
template <typename Result = void, typename Any>
auto switchAny(Any& a) {
  return [&a](auto ...cases_) -> Result {
    auto cases = hana::make_tuple(cases_...);

    auto default_ = hana::find_if(cases, [](auto const& c) {
      return c.first == hana::type<_default>;
    });
    static_assert(hana::is_just(default_),
      "switch is missing a default_ case");

    auto rest = hana::filter(cases, [](auto const& c) {
      return c.first != hana::type<_default>;
    });

    return hana::unpack(rest, [&](auto& ...rest) {
      return switchAnyImpl<Result>(a, a.type(), hana::from_just(default_).second, rest...);
    });
  };
}
// end-sample


static std::vector<std::function<void()>> tests{
  [] {
    boost::any a;
    int result = switchAny<int>(a)(
        default_([] { return 1; })
    );
    assert(result == 1);
  }

  , [] {
    boost::any a;
    int result = switchAny<int>(a)(
        empty([] { return 2; }),
        default_([] { return 1; })
    );
    assert(result == 2);
  }

  , [] {
    boost::any a = 1;
    int result = switchAny<int>(a)(
        empty([] { return 2; }),
        default_([] { return 1; })
    );
    assert(result == 1);
  }

  , [] {
    boost::any a = 3;
    int result = switchAny<int>(a)(
        case_<int>([](int& i) { return i; }),
        empty([] { return 2; }),
        default_([] { return 1; })
    );
    assert(result == 3);
  }

  , [] {
    boost::any a = 3;
    const boost::any& ra = a;
    int result = switchAny<int>(ra)(
        case_<int>([](const int& i) { return i; }),
        empty([] { return 2; }),
        default_([] { return 1; })
    );
    assert(result == 3);
  }

  , [] {
    boost::any a = 3;
    int result = switchAny<int>(a)(
        empty([] { return 2; }),
        default_([] { return 1; }),
        case_<int>([](const int& i) { return i; })
    );
    assert(result == 3);
  }

  // This test case is disabled because I consider it a flaw that
  // default_ is not mandatory.
#if 0
  , [] {
    boost::any a = 3.0;
    int result = 1;
    switchAny(a,
        case_<int>([&](const int& i) { result = i; }),
        empty([&] { result = 2; })
    );
    assert(result == 1);
  }
#endif

  // implicit case_ and default_ are not supported for simplicity
#if 0
  , [] {
    boost::any a = 3;
    int result = switchAny<int>(a,
        case_([](int& i) { return i; }),
        empty([] { return 2; }),
        default_([] { return 1; })
    );
    assert(result == 3);
  }

  , [] {
    boost::any a;
    int result = switchAny<int>(a,
        [] { return 2; },
        default_([] { return 1; })
    );
    assert(result == 2);
  }

  , [] {
    boost::any a = 3;
    int result = switchAny<int>(a,
        [] { return 2; },
        default_([] { return 1; }),
        [](int& i) { return i; }
    );
    assert(result == 3);
  }
#endif
};

int main() {
  for (auto& test: tests)
    test();

// sample(switch_any-now-usage)
boost::any a = 3;
std::string result = switchAny<std::string>(a)(
    case_<int>([](int i) { return std::to_string(i); })
  , case_<double>([](double d) { return std::to_string(d); })
  , empty([] { return "empty"; })
  , default_([] { return "default"; })
);
assert(result == "3");
// end-sample
}
