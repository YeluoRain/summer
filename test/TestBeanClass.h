#ifndef TEST_TESTBEANCLASS
#define TEST_TESTBEANCLASS

#include <iostream>
#include <memory>

#include "boost/describe.hpp"
#include "boost/hana.hpp"
#include "summer/Summer.h"

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
    void testA() override {
        std::cout << "print a test" << std::endl;
    }

    ~AImpl() override {
        std::cout << "NormalCase AImpl destructor" << std::endl;
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
    INJECT_CONSTRUCTOR(explicit BImpl, (const std::shared_ptr<A>& a)) : a(a) {
        std::cout << "NormalCase BImpl constructor" << std::endl;
    }

    void testB() override {
        std::cout << "print b test" << std::endl;
    }

    ~BImpl() override {
        std::cout << "NormalCase BImpl destructor" << std::endl;
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
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A>& a, const std::shared_ptr<B>& b))
        : a(a), b(b) {
        std::cout << "NormalCase CImpl constructor" << std::endl;
    }

    void testC() override {
        std::cout << "print c test" << std::endl;
    }

    ~CImpl() override {
        std::cout << "NormalCase CImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    std::shared_ptr<B> b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace NormalCase

namespace BeanWithRefConstructor {
class A {
  public:
    virtual ~A() = default;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeanWithRefConstructor AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "BeanWithRefConstructor AImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
};

class BImpl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (A&)) {
        std::cout << "BeanWithRefConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeanWithRefConstructor BImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (A&, B&)) {
        std::cout << "BeanWithRefConstructor CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "BeanWithRefConstructor CImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeanWithRefConstructor

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
    void testA() override {
        std::cout << "print a test" << std::endl;
    }

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

    void testB() override {
        std::cout << "print b test" << std::endl;
    }

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

    void testC() override {
        std::cout << "print c test" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    std::shared_ptr<B> b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

class D {
  public:
    virtual ~D() = default;
    virtual void testD() = 0;
};

class DImpl : public D {
  public:
    INJECT_CONSTRUCTOR(DImpl, (std::shared_ptr<C> c, std::shared_ptr<B> b)) : c(c), b(b) {
        std::cout << "FactoryCreateCase DImpl constructor" << std::endl;
    }

    void testD() override {
        std::cout << "print d test" << std::endl;
    }

    ~DImpl() override {
        std::cout << "FactoryCreateCase DImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<C> c;
    std::shared_ptr<B> b;

    BOOST_DESCRIBE_CLASS(DImpl, (D), (), (), ())
};
}  // namespace FactoryCreateCase

namespace BeansWithListConstructor {
class A {
  public:
    virtual ~A() = default;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeansWithListConstructor AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "BeansWithListConstructor AImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
};

class BImpl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (std::shared_ptr<A> a)) : a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class B2Impl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit B2Impl, (std::shared_ptr<A> a)) : a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~B2Impl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    BOOST_DESCRIBE_CLASS(B2Impl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl,
                       (const std::shared_ptr<A>& a, const std::list<std::shared_ptr<B>>& bs))
        : a(a), bs(bs) {
        std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> a;
    std::list<std::shared_ptr<B>> bs;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeansWithListConstructor

namespace BeansWithListUniquePtrConstructor {
class A {
  public:
    virtual ~A() = default;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeansWithListConstructor AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "BeansWithListConstructor AImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
};

class BImpl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (std::shared_ptr<A> a)) : a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class B2Impl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit B2Impl, (std::shared_ptr<A> a)) : a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~B2Impl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    BOOST_DESCRIBE_CLASS(B2Impl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A>& a, std::list<std::unique_ptr<B>> bs))
        : a(a), bs(std::move(bs)) {
        std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> a;
    std::list<std::unique_ptr<B>> bs;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeansWithListUniquePtrConstructor

namespace BeansWithRawPointerConstructor {
class A {
  public:
    virtual ~A() = default;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeansWithRawPointerConstructor AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "BeansWithRawPointerConstructor AImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
};

class BImpl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (A*)) {
        std::cout << "BeansWithRawPointerConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithRawPointerConstructor BImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class B2Impl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit B2Impl, (A*)) {
        std::cout << "BeansWithRawPointerConstructor BImpl constructor" << std::endl;
    }

    ~B2Impl() override {
        std::cout << "BeansWithRawPointerConstructor BImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(B2Impl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (A * a, const std::list<B*>& bs)) : a(a), bs(bs) {
        std::cout << "BeansWithRawPointerConstructor CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "BeansWithRawPointerConstructor CImpl destructor" << std::endl;
    }

    A* a;
    std::list<B*> bs;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeansWithRawPointerConstructor

namespace BeansWithUniqueButHasSharedPtrConstructor {
class A {
  public:
    virtual ~A() = default;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor AImpl destructor" << std::endl;
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
};

class BImpl : public B {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (const std::shared_ptr<A> a)) : a(a) {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A> a, std::unique_ptr<B> b))
        : a(a), b(std::move(b)) {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> a;
    std::unique_ptr<B> b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeansWithUniqueButHasSharedPtrConstructor

namespace BeansWithoutInterfaceHeritated {

class AImpl {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "BeansWithoutInterfaceHeritated AImpl constructor" << std::endl;
    }

    BOOST_DESCRIBE_CLASS(AImpl, (), (), (), ())
};

class BImpl {
  public:
    INJECT_CONSTRUCTOR(explicit BImpl, (const std::shared_ptr<AImpl> a)) : a(a) {
        std::cout << "BeansWithoutInterfaceHeritated BImpl constructor" << std::endl;
    }

    std::shared_ptr<AImpl> a;
    BOOST_DESCRIBE_CLASS(BImpl, (), (), (), ())
};

class CImpl {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<AImpl> a, std::unique_ptr<BImpl> b))
        : a(a), b(std::move(b)) {
        std::cout << "BeansWithoutInterfaceHeritated CImpl constructor" << std::endl;
    }

    std::shared_ptr<AImpl> a;
    std::unique_ptr<BImpl> b;

    BOOST_DESCRIBE_CLASS(CImpl, (), (), (), ())
};
}  // namespace BeansWithoutInterfaceHeritated

#endif /* TEST_TESTBEANCLASS */
