//
// Created by Zhongwen Wang on 2023/11/5.
//

#ifndef CCTEST_TESTCLASS_H
#define CCTEST_TESTCLASS_H

#include "Summer.h"
#include "boost/describe.hpp"
#include "boost/hana.hpp"
#include <iostream>
#include <memory>

class A {
public:
  virtual ~A() = default;
  virtual void testA() = 0;
};

class AImpl : public A {
public:
  INJECT_CONSTRUCTOR(AImpl, ()) {}
  void testA() override { std::cout << "print wzw" << std::endl; }

private:
  BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
public:
  virtual ~B() = default;
  virtual void testB() = 0;
};

class BImpl : public B {
public:
  INJECT_CONSTRUCTOR(explicit BImpl, (std::shared_ptr<A> a)) : a(a) {}

  void testB() override { std::cout << "print wzw" << std::endl; }

private:
  std::shared_ptr<A> a;
  BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
public:
  virtual ~C() = default;
  virtual void testC() = 0;
};

class CImpl : public C {
public:
  INJECT_CONSTRUCTOR(CImpl, (std::shared_ptr<A> a, std::shared_ptr<B> b))
      : a(a), b(b) {}

  void testC() override { std::cout << "print wzw" << std::endl; }

private:
  std::shared_ptr<A> a;
  std::shared_ptr<B> b;

  BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

struct BeanResolverA {
  using BeanType = AImpl;
  constexpr static auto DependOn = boost::hana::make_tuple();
  constexpr static auto ImplementOf =
      boost::hana::make_tuple(boost::hana::type_c<A>);
};

struct BeanResolverB {
  using BeanType = BImpl;
  constexpr static auto DependOn =
      boost::hana::make_tuple(boost::hana::type_c<A>);
  constexpr static auto ImplementOf =
      boost::hana::make_tuple(boost::hana::type_c<B>);
};

struct BeanResolverC {
  using BeanType = CImpl;
  constexpr static auto DependOn =
      boost::hana::make_tuple(boost::hana::type_c<A>, boost::hana::type_c<B>);
  constexpr static auto ImplementOf =
      boost::hana::make_tuple(boost::hana::type_c<C>);
};

#endif // CCTEST_TESTCLASS_H
