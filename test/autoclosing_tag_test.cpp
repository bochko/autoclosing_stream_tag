#include <autoclosing_tag.hpp>
#include <catch2/catch_all.hpp>

#include <iostream>
#include <string_view>

TEST_CASE("Using various combinations of tags", "[tag -> stringstream]") {
  auto tag =
      act::stream::tag_pair{std::string{"[open]"}, std::string{"[close]"}};
  
  std::stringstream sout;

  sout << tag << "hello, world";

  auto stream = std::cout << tag;
  stream << "hello_world";
}