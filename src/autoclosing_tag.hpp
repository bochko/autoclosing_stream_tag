// MIT License
// Copyright (c) 2023 Boyan Atanasov

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <concepts>
#include <ostream>
#include <string>

namespace act::stream {

template <typename T>
  requires requires(std::ostream &os, T &&t) {
    { os << t } -> std::convertible_to<std::ostream &>;
  }
struct tag {
  tag(T &&t) : arg{std::forward<T>(t)} {}
  T arg;
};

template <typename T>
auto operator<<(std::ostream &os, const tag<T> &st) -> std::ostream & {
  return os << st.arg;
}

template <typename T, typename U> struct tag_pair {
  tag_pair(T t, U u) : first{std::move(t)}, second{std::move(u)} {}
  tag<T> first;
  tag<U> second;
};

template <typename T, typename U> struct ostream_wrapper;

template <typename T> struct ostream_wrapper<std::ostream, T> {
  ostream_wrapper(std::ostream &os, tag<T> closing_tag)
      : os{os}, closing_tag{std::move(closing_tag)} {}
  ~ostream_wrapper() { os = os << closing_tag; }
  std::ostream &os;
  tag<T> closing_tag;
};

template <typename T, typename U, typename V>
struct ostream_wrapper<ostream_wrapper<T, U>, V> {
  ostream_wrapper(ostream_wrapper<T, U> os, tag<V> closing_tag)
      : os{std::move(os)}, closing_tag{std::move(closing_tag)} {}
  ~ostream_wrapper() { os = os << closing_tag; }
  ostream_wrapper<T, U> os;
  tag<V> closing_tag;
};

template <typename T, typename U, typename V>
auto operator<<(ostream_wrapper<T, U> &ow, const V &val)
    -> ostream_wrapper<T, U> & {
  return ow.os << val;
}

template <typename T, typename U>
auto operator<<(std::ostream &ow, tag_pair<T, U> bst)
    -> ostream_wrapper<std::ostream, U> {
  return ostream_wrapper<std::ostream, U>{ow << std::move(bst.first),
                                          std::move(bst.second)};
};

template <typename T, typename U, typename V, typename X>
auto operator<<(ostream_wrapper<T, U> &ow, tag_pair<V, X> bst)
    -> ostream_wrapper<ostream_wrapper<T, U>, X> {
  return ostream_wrapper<ostream_wrapper<T, U>, X>{ow << std::move(bst.first),
                                                   std::move(bst.second)};
}

template <typename T, typename U, typename V>
auto operator<<(ostream_wrapper<T, U> &&ow, const V &val)
    -> ostream_wrapper<T, U> && {
  return ow << val;
}

} // namespace act::stream