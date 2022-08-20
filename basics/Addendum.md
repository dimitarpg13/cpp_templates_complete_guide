# Various topics relevant for the template discussion

## Declarations

_Declarations_ introduce (or re-introduce) names into the C++ program. Each kind of entity is declared differently.
_Definitions_ are declarations that are sufficient to use the entity identied by the name.

A declaration is one of the following:
* Function definition
* Template declaration (including Partial template specialization)
* Explicit template instantiation  
* Explicit template specialization
* Namespace definition
* Linkage specification
* Attribute declaration (attr;) _(since C++11)_
* Empty declaration (;)
* A function declaration without a _decl-specifier-seq:_
_attr_(optional) _declarator_`;`
_attr_ - (since C++11) sequence of any number of attributes
_declarator_ - a function declarator
  This declaration must declare a constructor, destructor, or user-defined type conversion function. It can only be used as part of a template declaration, explicit specialization, or explicit instantiation.

* _block-declaration_ (a declaration that can appear inside a block), which, in turn, can be one of the following:
  * asm declaration
  * type alias declaration _(since C++11)_
  * namespace alias definition
  * using-declaration
  * using directive
  * using-enum-declaration _(since C++20)_
  * `static_assert` declaration _(since C++11)_
  * opaque enum declaration _(since C++11)_
  * simple declaration

### Simple declaration
A simple declaration is a statement that introduces, creates, and optionally initializes one or several identifiers, typically variables.
_decl-specified-seq init-declarator-list_(optional)`;`   (1)


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

* A `static_assert` declaration (does not define any entities)  _(since C++ 11)_
* An attribute declaration (does not define any entities) _(since C++ 11)_

