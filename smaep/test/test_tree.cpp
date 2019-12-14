//#include "catch2/catch.hpp"
//
//#include <iostream>
//
//#include "map_source.h"
//#include "tree.hpp"
//
// using namespace smaep;
//
// double minus(double x)
//{
//    return -x;
//}
//
// double add(double x, double y)
//{
//    return x + y;
//}
//
// double sum(double x, double y, double z)
//{
//    return x + y + z;
//}
//
// TEST_CASE("cracy", "[tast]")
//{
//    auto n1 = make_node(1.0);
//    auto x = n1->value();
//    CHECK(1 == n1->value());
//
//    auto n2a = make_node(-1.0);
//    std::function<double(double)> f2 = minus;
//    auto n2 = make_node(f2, std::move(n2a));
//    CHECK(1 == n2->value());
//
//    std::function<double(double, double)> f3 = add;
//    auto n3 = make_node(f3, std::move(n1), std::move(n2));
//    CHECK(2 == n3->value());
//
//    auto n4a = make_node(1.0);
//    auto n4b = make_node(1.0);
//    auto n4c = make_node(1.0);
//    std::function<double(double, double, double)> f4 = sum;
//    auto n4 = make_node(f4, std::move(n4a), std::move(n4b), std::move(n4c));
//    CHECK(3 == n4->value());
//
//    auto ms = smaep::map_source<double>::create_top_ten();
//    auto n5 = make_node<double>("one");
//    CHECK(1 == n5->value(ms));
//
//    CHECK_THROWS(n5->value());
//}
