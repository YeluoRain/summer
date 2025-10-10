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

namespace NormalCase {
class A {
public:
  virtual ~A() = default;
  virtual void testA() = 0;
};

class AImpl : public A {
public:
  INJECT_CONSTRUCTOR(AImpl, ()) {
    std::cout << "NormalCase AImpl constructor" << std::endl;
  }
  void testA() override { std::cout << "print a wzw" << std::endl; }

  ~AImpl() override { std::cout << "NormalCase AImpl destructor" << std::endl; }

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
  INJECT_CONSTRUCTOR(explicit BImpl, (std::shared_ptr<A> a)) : a(a) {
    std::cout << "NormalCase BImpl constructor" << std::endl;
  }

  void testB() override { std::cout << "print b wzw" << std::endl; }

  ~BImpl() override { std::cout << "NormalCase BImpl destructor" << std::endl; }

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
      : a(a), b(b) {
    std::cout << "NormalCase CImpl constructor" << std::endl;
  }

  void testC() override { std::cout << "print c wzw" << std::endl; }

  ~CImpl() override { std::cout << "NormalCase CImpl destructor" << std::endl; }

private:
  std::shared_ptr<A> a;
  std::shared_ptr<B> b;

  BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
} // namespace NormalCase

namespace FactoryCreateCase {
class A {
public:
  virtual ~A() = default;
  virtual void testA() = 0;
};

class AImpl : public A {
public:
  INJECT_CONSTRUCTOR(AImpl, ()) {
    std::cout << "FactoryCreateCase AImpl constructor" << std::endl;
  }
  void testA() override { std::cout << "print a wzw" << std::endl; }

  ~AImpl() override {
    std::cout << "FactoryCreateCase AImpl destructor" << std::endl;
  }

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
  INJECT_CONSTRUCTOR(explicit BImpl, (std::shared_ptr<A> a)) : a(a) {
    std::cout << "FactoryCreateCase BImpl constructor" << std::endl;
  }

  void testB() override { std::cout << "print b wzw" << std::endl; }

  ~BImpl() override {
    std::cout << "FactoryCreateCase BImpl destructor" << std::endl;
  }

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
  INJECT_CONSTRUCTOR(CImpl, (std::shared_ptr<A> a, std::unique_ptr<B> b))
      : a(a), b(std::move(b)) {
    std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
  }

  void testC() override { std::cout << "print c wzw" << std::endl; }

  ~CImpl() override {
    std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
  }

private:
  std::shared_ptr<A> a;
  std::shared_ptr<B> b;

  BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
} // namespace FactoryCreateCase

#endif // CCTEST_TESTCLASS_H
