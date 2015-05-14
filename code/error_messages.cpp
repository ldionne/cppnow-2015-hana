// Copyright Louis Dionne 2015
// Distributed under the Boost Software License, Version 1.0.

#include <boost/hana.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/reverse.hpp>
namespace hana = boost::hana;
namespace mpl = boost::mpl;


#if 0
// sample(error_messages-then)
using xs = mpl::reverse<mpl::int_<1>>::type;
// end-sample
#endif

#if 0
// sample(error_messages-now)
auto xs = hana::reverse(1);
// end-sample
#endif


int main() { }
