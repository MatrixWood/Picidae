#ifndef PICIDAE_CORE_ANY_HPP__
#define PICIDAE_CORE_ANY_HPP__

#include <cassert>
#include <ctime>
#include <iterator>
#include <memory>
#include <type_traits>

#include "picidae/core/common.hpp"
#include "picidae/core/stlforwardcompat.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

class Any {
  template <typename T>
  struct TypeId {
    static char Id;
  };

  struct StorageBase {
    virtual ~StorageBase() = default;
    virtual std::unique_ptr<StorageBase> clone() const = 0;
    virtual const void* id() const = 0;
  };

  template <typename T>
  struct StorageImpl : public StorageBase {
    explicit StorageImpl(const T& Value) : Value(Value) {}

    explicit StorageImpl(T&& Value) : Value(std::move(Value)) {}

    std::unique_ptr<StorageBase> clone() const override {
      return std::make_unique<StorageImpl<T>>(Value);
    }

    const void* id() const override { return &TypeId<T>::Id; }

    T Value;

   private:
    StorageImpl& operator=(const StorageImpl& Other) = delete;
    StorageImpl(const StorageImpl& Other) = delete;
  };

 public:
  Any() = default;

  Any(const Any& Other)
      : Storage(Other.Storage ? Other.Storage->clone() : nullptr) {}

  template <typename T,
            std::enable_if_t<
                std::conjunction_v<
                    std::negation<std::is_same<std::decay_t<T>, Any>>,
                    std::negation<std::is_convertible<Any, std::decay_t<T>>>,
                    std::is_copy_constructible<std::decay_t<T>>>,
                int> = 0>
  Any(T&& Value) {
    Storage =
        std::make_unique<StorageImpl<std::decay_t<T>>>(std::forward<T>(Value));
  }

  Any(Any&& Other) : Storage(std::move(Other.Storage)) {}

  Any& Swap(Any& Other) {
    std::swap(Storage, Other.Storage);
    return *this;
  }

  Any& operator=(Any Other) {
    Storage = std::move(Other.Storage);
    return *this;
  }

  bool has_value() const { return !!Storage; }

  void reset() { Storage.reset(); }

 private:
  template <typename T>
  bool isa() const {
    if (!Storage) return false;
    return Storage->id() == &Any::TypeId<remove_cvref_t<T>>::Id;
  }

  template <class T>
  friend T any_cast(const Any& Value);

  template <class T>
  friend T any_cast(Any& Value);

  template <class T>
  friend T any_cast(Any&& Value);

  template <class T>
  friend const T* any_cast(const Any* Value);

  template <class T>
  friend T* any_cast(Any* Value);

  std::unique_ptr<StorageBase> Storage;
};

template <typename T> char Any::TypeId<T>::Id = 0;

template <class T>
T any_cast(const Any& Value) {
  assert(Value.isa<T>() && "Bad any cast!");
  return static_cast<T>(*any_cast<remove_cvref_t<T>>(&Value));
}

template <class T>
T any_cast(Any& Value) {
  assert(Value.isa<T>() && "Bad any cast!");
  return static_cast<T>(*any_cast<remove_cvref_t<T>>(&Value));
}

template <class T>
T any_cast(Any&& Value) {
  assert(Value.isa<T>() && "Bad any cast!");
  return static_cast<T>(std::move(*any_cast<remove_cvref_t<T>>(&Value)));
}

template <class T>
const T* any_cast(const Any* Value) {
  using U = remove_cvref_t<T>;
  if (!Value || !Value->isa<U>())
    return nullptr;
  return &static_cast<Any::StorageImpl<U>&>(*Value->Storage).Value;
}

template <class T>
T* any_cast(Any* Value) {
  using U = remove_cvref_t<T>;
  if (!Value || !Value->isa<U>())
    return nullptr;
  return &static_cast<Any::StorageImpl<U>&>(*Value->Storage).Value;
}

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_ANY_HPP__