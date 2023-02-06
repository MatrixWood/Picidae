#ifndef PICIDAE_CORE_ARENA_HPP__
#define PICIDAE_CORE_ARENA_HPP__

#include <memory>
#include <type_traits>
#include <vector>

#include "picidae/core/nonnull.hpp"
#include "picidae/core/common.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

class Arena {
 public:
  template <
      typename T, typename... Args,
      typename std::enable_if_t<std::is_constructible_v<T, Args...>>* = nullptr>
  auto New(Args&&... args) -> Nonnull<T*> {
    auto smart_ptr =
        std::make_unique<ArenaEntryTyped<T>>(std::forward<Args>(args)...);
    Nonnull<T*> ptr = smart_ptr->Instance();
    arena_.push_back(std::move(smart_ptr));
    return ptr;
  }

 private:
  class ArenaEntry {
   public:
    virtual ~ArenaEntry() = default;
  };

  template <typename T>
  class ArenaEntryTyped : public ArenaEntry {
   public:
    template <typename... Args>
    explicit ArenaEntryTyped(Args&&... args)
        : instance_(std::forward<Args>(args)...) {}

    auto Instance() -> Nonnull<T*> { return Nonnull<T*>(&instance_); }

   private:
    T instance_;
  };

  std::vector<std::unique_ptr<ArenaEntry>> arena_;
};

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_ARENA_HPP__