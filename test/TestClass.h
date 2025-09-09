//
// Created by Zhongwen Wang on 2023/11/5.
//

#ifndef CCTEST_TESTCLASS_H
#define CCTEST_TESTCLASS_H

#include "boost/hana.hpp"
#include <boost/describe/class.hpp>
#include <iostream>
#include <memory>

class A {
public:
  virtual void testA() = 0;
};

class AImpl : public A {
public:
  void testA() override { std::cout << "print wzw" << std::endl; }

private:
  BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
public:
  virtual void testB() = 0;
};

class BImpl : public B {
public:
  explicit BImpl(std::shared_ptr<A> &a) : a(a) {}

  void testB() override { std::cout << "print wzw" << std::endl; }

private:
  std::shared_ptr<A> a;
  BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
public:
  virtual void testC() = 0;
};

class CImpl : public C {
public:
  explicit CImpl(std::shared_ptr<A> &a, std::shared_ptr<B> &b) : a(a), b(b) {}

  void testC() override { std::cout << "print wzw" << std::endl; }

private:
  std::shared_ptr<A> a;
  std::shared_ptr<B> b;

  BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

class A0 {};

class B0 {};

class A1 : public A0, public B0 {
  BOOST_DESCRIBE_CLASS(A1, (A0, B0), (), (), ())
};

class A2 : public A1 {
  BOOST_DESCRIBE_CLASS(A2, (A1), (), (), ())
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
