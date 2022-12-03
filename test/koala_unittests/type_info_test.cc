#include <iostream>
#include <typeinfo>
#include <string>
#include <gtest/gtest.h>

#include "koala/type_info.hpp"
 
struct Base {
  virtual ~Base() = default;
private:
  std::string _name;
};
struct Derived : Base {
private:
  std::string _profession;
};

template<typename U, typename V> constexpr bool same = std::is_same_v<U, V>;
 
void BareType_Test() {
  using namespace koala::detail;
	Base b1;
	Derived d1;
 
	const Base *pb = &b1;
	std::cout << typeid(*pb).name() << '\n';
	pb = &d1;
	std::cout << typeid(*pb).name() << '\n';

  std::cout << typeid(koala::detail::BareType<const Base*>::type).name() << '\n';
  std::cout << typeid(const Base*).name() << '\n';

  static_assert(
        same< BareType_t< int >, int >
    and same< BareType_t< const int >, int >
    and same< BareType_t< volatile int >, int >
    and same< BareType_t< const volatile int >, int >
    and same< BareType_t< const volatile int* >, int >
    and same< BareType_t< const int* volatile >, int >
    and same< BareType_t< int* const volatile >, int >
  );
}

struct UnknownType {};

void TypeInfo_Test() {
  const std::type_info& unknowntype_info = typeid(UnknownType);
  std::cout << "unknowntype_info type: " << unknowntype_info.name() << "\n";

  if (typeid(Derived).before(typeid(Base)))
    std::cout << "Base goes before Derived in this implementation.\n";
  else
    std::cout << "Derived goes before Base in this implementation.\n";

  if (typeid(Base) != typeid(Derived)) {
    std::cout << "class `Base` != class `Derived`\n";
  } 
}

void test() {
  BareType_Test();
  TypeInfo_Test();
}


TEST(TypeInfo, base) {
  test();
}

TEST(TypeInfo, GetTypeInfo) {
  using namespace koala;
  using namespace koala::detail;

  class A {
  };

  TypeInfo ti = GetTypeInfo<int>::get();
  TypeInfo ti_A = GetTypeInfo<A>::get();
  EXPECT_EQ(ti.is_arithmetic(), true);
  EXPECT_EQ(ti_A.is_arithmetic(), false);
  EXPECT_EQ(std::string(ti_A.name()).find("GetTypeInfo") != std::string::npos, true);

  std::shared_ptr<A> a_ptr = std::make_shared<A>();
  TypeInfo ti_a_ptr = GetTypeInfo<std::shared_ptr<A>>::get();
  EXPECT_EQ(std::string(ti_a_ptr.name()).find("GetTypeInfo") != std::string::npos, true);
  EXPECT_EQ(std::string(ti_a_ptr.bare_name()).find("GetTypeInfo") != std::string::npos, true);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}