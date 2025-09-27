//
// Created by Zhongwen Wang on 2023/12/14.
//

#include "DefineTest.h"
#include "Summer.h"
#include "TestBeanClass.h"
#include "boost/describe.hpp"
#include "boost/hana.hpp"

using namespace summer::bean::traits;
using namespace NormalCase;

void DefineTest::SetUp() {}

void DefineTest::TearDown() {}

class TestA : public A {
public:
  INJECT_CONSTRUCTOR(TestA, (int a, std::string &b, char c)) {}

private:
  BOOST_DESCRIBE_CLASS(TestA, (A), (), (), ())
};

TEST_F(DefineTest, test_traits) {
  using TestATrait = summer::bean::define::BeanResolver<CImpl>;
  std::cout << boost::core::demangle(typeid(TestATrait::ImplementOf).name())
            << std::endl;
  std::cout << boost::core::demangle(typeid(TestATrait::DependOn).name())
            << std::endl;
}

TEST_F(DefineTest, test_pointer) {
  A *a = nullptr;
  std::shared_ptr<A> as = nullptr;
  std::unique_ptr<A> au = nullptr;
  std::cout
      << boost::core::demangle(
             typeid(std::pointer_traits<decltype(a)>::element_type).name())
      << std::endl;
  std::cout
      << boost::core::demangle(
             typeid(std::pointer_traits<decltype(as)>::element_type).name())
      << std::endl;
  std::cout
      << boost::core::demangle(
             typeid(std::pointer_traits<decltype(au)>::element_type).name())
      << std::endl;

  std::cout << boost::core::demangle(
                   typeid(std::pointer_traits<decltype(a)>::rebind<B>).name())
            << std::endl;
  std::cout << boost::core::demangle(
                   typeid(std::pointer_traits<decltype(as)>::rebind<B>).name())
            << std::endl;
  std::cout << boost::core::demangle(
                   typeid(std::pointer_traits<decltype(au)>::rebind<B>).name())
            << std::endl;
}

TEST_F(DefineTest, test_arg_type_traits) {
  using Type0 = ArgTypeTraits<int>::type;
  using Type1 = ArgTypeTraits<int *>::type;
  using Type2 = ArgTypeTraits<std::shared_ptr<int>>::type;
  using Type3 = ArgTypeTraits<std::unique_ptr<int>>::type;
  using Type4 = ArgTypeTraits<std::list<std::shared_ptr<int>>>::type;
  using Type5 = ArgTypeTraits<std::vector<std::unique_ptr<int>>>::type;
  using Type6 = ArgTypeTraits<int &>::type;
  using Type7 = ArgTypeTraits<std::vector<int *>>::type;

  std::cout << boost::core::demangle(typeid(Type0).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type1).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type2).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type3).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type4).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type5).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type6).name()) << std::endl;
  std::cout << boost::core::demangle(typeid(Type7).name()) << std::endl;
}