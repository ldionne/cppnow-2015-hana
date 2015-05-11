// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>

#include <cstddef>
using namespace boost::hana;
using namespace boost::hana::literals;


// sample(indexed_sort-now-impl)
auto indexed_sort = [](auto list, auto predicate) {
  auto indices = to<Tuple>(range(0_c, size(list)));
  auto indexed_list = zip.with(make_pair, list, indices);
  auto sorted = sort(indexed_list, [&](auto const& x, auto const& y) {
    return predicate(first(x), first(y));
  });
  return make_pair(transform(sorted, first), transform(sorted, second));
};
// end-sample


int main() {
// sample(indexed_sort-now-usage1)
auto types = tuple_t<int[3], int[2], int[1]>;
auto indexed = indexed_sort(types, [](auto t, auto u) {
    return sizeof_(t) < sizeof_(u);
});

auto sorted = first(indexed);
auto indices = second(indexed);

static_assert(sorted == tuple_t<int[1], int[2], int[3]>, "");
static_assert(indices == tuple_c<std::size_t, 2, 1, 0>, "");
// end-sample

// sample(indexed_sort-now-usage2)
using Sequence = decltype(unpack(sorted, template_<_tuple>))::type;
auto index_map = second(indexed_sort(indices, less));

Sequence s;
int (&a)[3] = s[index_map[0_c]];
int (&b)[2] = s[index_map[1_c]];
int (&c)[1] = s[index_map[2_c]];
// end-sample

(void)a;
(void)b;
(void)c;
}
