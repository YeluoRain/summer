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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<A>& a)) : _a(a) {
        std::cout << "NormalCase BImpl constructor" << std::endl;
    }

    void testB() override {
        std::cout << "print b test" << std::endl;
    }

    ~BImpl() override {
        std::cout << "NormalCase BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
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
        : _a(a), _b(b) {
        std::cout << "NormalCase CImpl constructor" << std::endl;
    }

    void testC() override {
        std::cout << "print c test" << std::endl;
    }

    ~CImpl() override {
        std::cout << "NormalCase CImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    std::shared_ptr<B> _b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

inline C* createC(std::shared_ptr<A> a, std::shared_ptr<B> b) {
    return new CImpl(a, b);
}

inline CImpl* createCImpl(std::shared_ptr<A> a, std::shared_ptr<B> b) {
    return new CImpl(a, b);
}

inline B* createB(std::shared_ptr<A> a) {
    return new BImpl(a);
}

inline A* createA() {
    return new AImpl();
}

inline std::shared_ptr<A> createAShared() {
    return std::make_shared<AImpl>();
}

inline std::shared_ptr<B> createBShared(std::shared_ptr<A> a) {
    return std::make_shared<BImpl>(a);
}

inline std::shared_ptr<C> createCShared(std::shared_ptr<A> a, std::shared_ptr<B> b) {
    return std::make_shared<CImpl>(a, b);
}

inline std::unique_ptr<A> createAUnique() {
    return std::make_unique<AImpl>();
}

inline std::unique_ptr<B> createBUnique(std::shared_ptr<A> a) {
    return std::make_unique<BImpl>(a);
}

}  // namespace NormalCase

namespace BeanWithNoMacro {
class A {};

class B {
  public:
    virtual ~B() = default;
    virtual void testB() = 0;
};

class BImpl : public B {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<A>& a)) : _a(a) {
        std::cout << "NormalCase BImpl constructor" << std::endl;
    }

    void testB() override {
        std::cout << "print b test" << std::endl;
    }

    ~BImpl() override {
        std::cout << "NormalCase BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
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
        : _a(a), _b(b) {
        std::cout << "NormalCase CImpl constructor" << std::endl;
    }

    void testC() override {
        std::cout << "print c test" << std::endl;
    }

    ~CImpl() override {
        std::cout << "NormalCase CImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    std::shared_ptr<B> _b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

}  // namespace BeanWithNoMacro

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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (std::shared_ptr<A> a)) : _a(a) {
        std::cout << "FactoryCreateCase BImpl constructor" << std::endl;
    }

    void testB() override {
        std::cout << "print b test" << std::endl;
    }

    ~BImpl() override {
        std::cout << "FactoryCreateCase BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
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
        : _a(a), _b(std::move(b)) {
        std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
    }

    void testC() override {
        std::cout << "print c test" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    std::shared_ptr<B> _b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

class D {
  public:
    virtual ~D() = default;
    virtual void testD() = 0;
};

class DImpl : public D {
  public:
    INJECT_CONSTRUCTOR(DImpl, (std::shared_ptr<C> c, std::shared_ptr<B> b)) : _c(c), _b(b) {
        std::cout << "FactoryCreateCase DImpl constructor" << std::endl;
    }

    void testD() override {
        std::cout << "print d test" << std::endl;
    }

    ~DImpl() override {
        std::cout << "FactoryCreateCase DImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<C> _c;
    std::shared_ptr<B> _b;

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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (std::shared_ptr<A> a)) : _a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class B2Impl : public B {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(B2Impl, (std::shared_ptr<A> a)) : _a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~B2Impl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
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
        : _a(a), _bs(bs) {
        std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> _a;
    std::list<std::shared_ptr<B>> _bs;

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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (std::shared_ptr<A> a)) : _a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class B2Impl : public B {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(B2Impl, (std::shared_ptr<A> a)) : _a(a) {
        std::cout << "BeansWithListConstructor BImpl constructor" << std::endl;
    }

    ~B2Impl() override {
        std::cout << "BeansWithListConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(B2Impl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A>& a, std::list<std::unique_ptr<B>> bs))
        : _a(a), _bs(std::move(bs)) {
        std::cout << "FactoryCreateCase CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "FactoryCreateCase CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> _a;
    std::list<std::unique_ptr<B>> _bs;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace BeansWithListUniquePtrConstructor

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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<A> a)) : _a(a) {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor BImpl destructor" << std::endl;
    }

  private:
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A> a, std::unique_ptr<B> b))
        : _a(a), _b(std::move(b)) {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "BeansWithUniqueButHasSharedPtrConstructor CImpl destructor" << std::endl;
    }

    std::shared_ptr<A> _a;
    std::unique_ptr<B> _b;

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
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<AImpl> a)) : _a(a) {
        std::cout << "BeansWithoutInterfaceHeritated BImpl constructor" << std::endl;
    }

    std::shared_ptr<AImpl> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (), (), (), ())
};

class CImpl {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<AImpl> a, std::unique_ptr<BImpl> b))
        : _a(a), _b(std::move(b)) {
        std::cout << "BeansWithoutInterfaceHeritated CImpl constructor" << std::endl;
    }

    std::shared_ptr<AImpl> _a;
    std::unique_ptr<BImpl> _b;

    BOOST_DESCRIBE_CLASS(CImpl, (), (), (), ())
};
}  // namespace BeansWithoutInterfaceHeritated

namespace LifecycleTest {
static std::vector<std::string> g_destructorOrder;

class A {
  public:
    virtual ~A() {
        g_destructorOrder.push_back("A");
    }
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {
        std::cout << "LifecycleTest AImpl constructor" << std::endl;
    }

    ~AImpl() override {
        std::cout << "LifecycleTest AImpl destructor" << std::endl;
        g_destructorOrder.push_back("AImpl");
    }

  private:
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() {
        g_destructorOrder.push_back("B");
    }
};

class BImpl : public B {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<A>& a)) : _a(a) {
        std::cout << "LifecycleTest BImpl constructor" << std::endl;
    }

    ~BImpl() override {
        std::cout << "LifecycleTest BImpl destructor" << std::endl;
        g_destructorOrder.push_back("BImpl");
    }

    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() {
        g_destructorOrder.push_back("C");
    }
};

class CImpl : public C {
  public:
    INJECT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A>& a, const std::shared_ptr<B>& b))
        : _a(a), _b(b) {
        std::cout << "LifecycleTest CImpl constructor" << std::endl;
    }

    ~CImpl() override {
        std::cout << "LifecycleTest CImpl destructor" << std::endl;
        g_destructorOrder.push_back("CImpl");
    }

    std::shared_ptr<A> _a;
    std::shared_ptr<B> _b;

    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};
}  // namespace LifecycleTest

namespace ComplexDagCase {
class A {
  public:
    virtual ~A() = default;
    virtual void test() = 0;
};

class AImpl : public A {
  public:
    INJECT_CONSTRUCTOR(AImpl, ()) {}
    void test() override {}
    BOOST_DESCRIBE_CLASS(AImpl, (A), (), (), ())
};

class B {
  public:
    virtual ~B() = default;
    virtual void test() = 0;
};

class BImpl : public B {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(BImpl, (const std::shared_ptr<A>& a)) : _a(a) {}
    void test() override {}
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(BImpl, (B), (), (), ())
};

class C {
  public:
    virtual ~C() = default;
    virtual void test() = 0;
};

class CImpl : public C {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(CImpl, (const std::shared_ptr<A>& a)) : _a(a) {}
    void test() override {}
    std::shared_ptr<A> _a;
    BOOST_DESCRIBE_CLASS(CImpl, (C), (), (), ())
};

class D {
  public:
    virtual ~D() = default;
    virtual void test() = 0;
};

class DImpl : public D {
  public:
    INJECT_CONSTRUCTOR(DImpl, (const std::shared_ptr<B>& b, const std::shared_ptr<C>& c))
        : _b(b), _c(c) {}
    void test() override {}
    std::shared_ptr<B> _b;
    std::shared_ptr<C> _c;
    BOOST_DESCRIBE_CLASS(DImpl, (D), (), (), ())
};

class E {
  public:
    virtual ~E() = default;
    virtual void test() = 0;
};

class EImpl : public E {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(EImpl, (const std::shared_ptr<D>& d)) : _d(d) {}
    void test() override {}
    std::shared_ptr<D> _d;
    BOOST_DESCRIBE_CLASS(EImpl, (E), (), (), ())
};

class F {
  public:
    virtual ~F() = default;
    virtual void test() = 0;
};

class FImpl : public F {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(FImpl, (const std::shared_ptr<E>& e)) : _e(e) {}
    void test() override {}
    std::shared_ptr<E> _e;
    BOOST_DESCRIBE_CLASS(FImpl, (F), (), (), ())
};

class G {
  public:
    virtual ~G() = default;
    virtual void test() = 0;
};

class GImpl : public G {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(GImpl, (const std::shared_ptr<E>& e)) : _e(e) {}
    void test() override {}
    std::shared_ptr<E> _e;
    BOOST_DESCRIBE_CLASS(GImpl, (G), (), (), ())
};

class H {
  public:
    virtual ~H() = default;
    virtual void test() = 0;
};

class HImpl : public H {
  public:
    INJECT_CONSTRUCTOR(HImpl,
                       (const std::shared_ptr<F>& f, const std::shared_ptr<G>& g,
                        const std::list<std::shared_ptr<A>>& aList))
        : _f(f), _g(g), _aList(aList) {}
    void test() override {}
    std::shared_ptr<F> _f;
    std::shared_ptr<G> _g;
    std::list<std::shared_ptr<A>> _aList;
    BOOST_DESCRIBE_CLASS(HImpl, (H), (), (), ())
};

class H2Impl : public H {
  public:
    INJECT_EXPLICIT_CONSTRUCTOR(H2Impl,
                                (const std::shared_ptr<F>& f, const std::shared_ptr<G>& g,
                                 const std::list<std::shared_ptr<A>>& aList))
        : _f(f), _g(g), _aList(aList) {}
    void test() override {}
    std::shared_ptr<F> _f;
    std::shared_ptr<G> _g;
    std::list<std::shared_ptr<A>> _aList;
    BOOST_DESCRIBE_CLASS(H2Impl, (H), (), (), ())
};

}  // namespace ComplexDagCase

#endif /* TEST_TESTBEANCLASS */