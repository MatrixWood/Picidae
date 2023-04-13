#ifndef PICIDAE_CORE_UNIQUEVECTOR_HPP__
#define PICIDAE_CORE_UNIQUEVECTOR_HPP__

#include <cassert>
#include <cstddef>
#include <map>
#include <vector>

#include "picidae/core/common.hpp"

PICIDAE_NAMESPACE_BEGIN(PICIDAE_NAMESPACE)

PICIDAE_NAMESPACE_BEGIN(core)

template <typename T>
class UniqueVector {
  using VectorType = typename std::vector<T>;
  using iterator = typename VectorType::iterator;
  using const_iterator = typename VectorType::const_iterator;

 private:
  std::map<T, unsigned> Map;
  VectorType Vector;

 public:
  unsigned insert(const T& entry) {
    unsigned &Val = Map[entry];
    if (!!Val) return Val;
    Val = static_cast<unsigned>(Vector.size()) + 1;
    Vector.push_back(entry);
    return Val;
  }

  unsigned idFor(const T& entry) const {
    typename std::map<T, unsigned>::const_iterator MI = Map.find(entry);
    if (MI != Map.end()) return MI->second;
    return 0;
  }

  const T& operator[](unsigned ID) const {
    assert(ID-1 < size() && "ID is 0 or out of range!");
    return Vector[ID - 1];
  }

  iterator begin() { return Vector.begin(); }

  const_iterator begin() const { return Vector.cbegin(); }

  iterator end() { return Vector.end(); }

  const_iterator end() const { return Vector.cend(); }

  size_t size() const { return Vector.size(); }

  bool empty() const { return Vector.empty(); }

  void reset() {
    Map.clear();
    Vector.resize(0, 0);
  }
};

PICIDAE_NAMESPACE_END(core)

PICIDAE_NAMESPACE_END(PICIDAE_NAMESPACE)

#endif  // PICIDAE_CORE_UNIQUEVECTOR_HPP__