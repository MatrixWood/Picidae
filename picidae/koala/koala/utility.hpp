#ifndef KOALA_UTILITY_HPP__
#define KOALA_UTILITY_HPP__

#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <cstdint>

namespace koala::utility
{
  struct StaticString {
    template <size_t N>
    constexpr StaticString(const char (&str)[N]) noexcept
      : _size(N - 1),
        _data(&str[0])
    { }

    constexpr size_t size() const noexcept { return _size; }

    constexpr const char* c_str() const noexcept { return _data; }

    constexpr auto begin() const noexcept { return _data; }

    constexpr auto end() const noexcept { return _data + _size; }

    constexpr bool operator == (std::string_view other) const noexcept {
      auto b1 = begin();
      const auto e1 = end();
      auto b2 = other.begin();
      const auto e2 = other.end();

      if (e1 - b1 != e2 - b2) {
        return false;
      }

      while (b1 != e1) {
        if (*b1 != *b2) {
          return false;
        }
        ++b1;
        ++b2;
      }
      return true;
    }

    bool operator==(const std::string &t_str) const noexcept {
      return std::equal(begin(), end(), std::cbegin(t_str), std::cend(t_str));
    }

    const size_t _size;
    const char* _data = nullptr;
  };

  namespace fnv1a {
    template<typename Itr>
    static constexpr std::uint32_t hash(Itr begin, Itr end) noexcept {
      std::uint32_t h = 0x811c9dc5;

      while (begin != end) {
        h = (h ^ (*begin)) * 0x01000193;
        ++begin;
      }
      return h;
    }

    template<size_t N>
    static constexpr std::uint32_t hash(const char (&str)[N]) noexcept {
      return hash(std::begin(str), std::end(str) - 1);
    }

    static constexpr std::uint32_t hash(std::string_view sv) noexcept {
      return hash(sv.begin(), sv.end());
    }

    static inline std::uint32_t hash(const std::string &s) noexcept {
      return hash(s.begin(), s.end());
    }
  } // namespace fnv1a

  namespace jenkins_one_at_a_time {
    template<typename Itr>
    static constexpr std::uint32_t hash(Itr begin, Itr end) noexcept {
      std::uint32_t hash = 0;

      while (begin != end) {
        hash += std::uint32_t(*begin);
        hash += hash << 10;
        hash ^= hash >> 6;
        ++begin;
      }

      hash += hash << 3;
      hash ^= hash >> 11;
      hash += hash << 15;
      return hash;
    }

    template<size_t N>
    static constexpr std::uint32_t hash(const char (&str)[N]) noexcept {
      return hash(std::begin(str), std::end(str) - 1);
    }

    static constexpr std::uint32_t hash(std::string_view sv) noexcept {
      return hash(sv.begin(), sv.end());
    }

    static inline std::uint32_t hash(const std::string &s) noexcept {
      return hash(s.begin(), s.end());
    }
  } // namespace jenkins_one_at_a_time

  using fnv1a::hash;
} // namespace koala::utility



#endif
