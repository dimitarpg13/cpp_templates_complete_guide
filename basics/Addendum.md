# Various topics relevant for the template discussion

## Defintions and ODR

_Definitions_ are declarations that fully define the entity introduced by the declaration. Every declaration is a definition except for the following:

* A function declaration without a function body:

```cpp
int f(int); // declares, but doesn't define f
```

* Any declaration with an `extern` storage class specifier or with a language linkage specified (such as `extern "C"`) without an initializer

```cpp
extern const int a;     // declares, but doesn't define a
extern const int b = 1; // defines b
```

* Declaration of a non-inline _(since C++17)_ inside a class definition:

```cpp
struct S
{
  int n;               // defines S::n
  static int i;        // declares, but doesn't define S::i
  inline static int x; // defines S::x
};

int S::i;              // defines S::i
```

* _(deprecated)_ Namespace scope declaration of a static data member that was defined within the class with the `constexpr` specifier:

```cpp
struct S
{
  static constexpr int x = 42; // implicitly inline, defines S::x
};

constexpr int S::x; // declares S::x, not a redefinition
```

* Declaration of a class name (by forward declaration or by the use of the elaborated type specifier in another declaration):

```cpp
struct S;             // declares, but does not define S

class Y f(class T p); // declares, but does not define Y and T (and also f and p)
```

* An opaque declaration of an enumeration:

```cpp
enum Color : int; // declares, but doesn't define Color
```

* Declaration of a template parameter:

```cpp
template<typename T> // declares, but doesn't define T
```

* A parameter declaration in a function declaration that isn't a definition:

```cpp
int f(int x); // declares, but doesn't define f and x

int f(int x)  // defines f and x
{
  return x + a;
}
```

* A `typedef` declaration:

```cpp
typedef S S2; // declares, but doesn't define S2 (S may be incomplete)
```

* An alias-declaration:

```cpp
using S2 = S; // declares, but doesn't define S2 (S may be incomplete)
```

* A using-declaration:

```cpp
using N::d;   // declares, but doesn't define d
```

* Declaration of a deduction guide (does not define any entities) _(since C++ 17)_

