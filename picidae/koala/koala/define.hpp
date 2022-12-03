#ifndef KOALA_DEFINE_HPP__
#define KOALA_DEFINE_HPP__

#include <string>
#include <vector>
#include <type_traits>

#define KOALA_COMPILER_VERSION __VERSION__

namespace koala {

  constexpr static const int version_major = 0;
  constexpr static const int version_minor = 1;
  constexpr static const char *compiler_version = KOALA_COMPILER_VERSION;

  struct BuildInfo {
    [[nodiscard]] constexpr static int version_major() noexcept { return koala::version_major; }

    [[nodiscard]] constexpr static int version_minor() noexcept { return koala::version_minor; }

    [[nodiscard]] static std::string version() {
      return std::to_string(version_major()) + '.' + std::to_string(version_minor());
    }

    [[nodiscard]] static std::string compiler_version() { return koala::compiler_version; }
  };

  template <typename T>
  [[nodiscard("parse_num")]] constexpr auto parse_num(const std::string_view t_str) noexcept
    -> typename std::enable_if_t<std::is_integral_v<T>, T> {
    T t;
    for (const auto c : t_str) {
      if (c < '0' || c > '9') {
        return t;
      }
      t *= 10;
      t += c - '0';
    }
    return t;
  }

  struct StrEqual {
    [[nodiscard]] bool operator () (const std::string& lhs, const std::string& rhs) const noexcept {
      return lhs == rhs;
    }
    template <typename LHS, typename RHS>
    [[nodiscard]] constexpr bool operator () (const LHS& lhs, const RHS& rhs) const noexcept {
      return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
  };


  template <typename Container, typename... T>
  [[nodiscard]] constexpr auto make_container(T&&... t) {
    Container c;
    c.reserve(sizeof...(t));
    (c.push_back(std::forward<T>(t)), ...);
    return c;
  }

  template <typename... T>
  [[nodiscard]] auto make_vector(T&&... t)
    -> std::vector<std::common_type_t<std::decay_t<T>...>> {
    using container_type = std::vector<std::common_type_t<std::decay_t<T>...>>;
    return make_container<container_type>(std::forward<T>(t)...);
  }
} // namespace koala

#endif
