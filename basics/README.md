# Function Templates

Function templates provide a functional behavior that can be called for different types. In other words a function template represents a family of functions. The representation looks a lot like an ordinary function except that some elements of the function are left undetermined. These elements are parametrized.

Let's start with the following simple example:

`basics/max1.hpp`
```cpp
template<typename T>
T max (T a, T b)
{
  // if b < a then yield a else yield b
  return b < a ? a : b;
}
``` 

This template definition specifies a family of functions that return the maximum of two values, which are passed as function parameters `a` and `b`. Note that the `max()` template according to [*StepanovNotes*] intentionally returns `b < a ? a : b` instead of `a < b ? b : a` to ensure that the function behaves correctly even if the two values are equivalent but not equal. The type of these parameters is left open as *template parameter* `T`. As seen in this example, template parameters must be announced with syntax of the following form:

```cpp
 template< comma-separated-list-of-parameters >
```

In our example, the list of parameters is `typename T`. The keyword `typename` introduces a *type parameter*. This is by far the most common kind of template parameter in C++ programs, but other parameters are possible. Here the type parameter is `T`. The type parameter represents an arbitrary type that is determined by the caller when the caller calls the function. You can use any type (fundamental type, class and so on) as long as it provides the operations that the template uses. In this case, type `T` has to support operator `<` because `a` and `b` are compared using this operator. Perhaps less obvious from the definition of `max()` is that the values of type `T` must also be copyable in order to be returned. Before C++17, type `T` also had to be copyabl to be able to pass in arguments, but since C++17 you can pass temporaries (rvalues) even if neither a copy nor a move constructor is valid.

For historical reasons, you can also use the keyword `class` instead of `typename` to define a type parameter. The keyword `typename` came relatively late in the evolution of the C++98 standard. Prior to that, the keyword `class` was the only way to introduce a type parameter, and this remains a valid way to do so. Hence the template `max()` could be defined equivalently as follows:

```cpp
template<class T>
T max (T a, T b)
{
  return b < a ? a : b;
}
```

Semantically, there is no difference in this context. So, even if you use `class` here, *any* type may be used for template arguments. However, because this use of `class` can be misleading (not only class types can be substituted for `T`), you should prefer the use of `typename` in this context. However, note that unlike class type definitions, the keyword `struct` cannot be used in place of `typename` when declaring type parameters.

The following code shows how to yse the `max()` function template:

`basics/max1.cpp`

```cpp
#include "max1.hpp"
#include <iostream>
#include <string>

int main()
{
  int i = 42;
  std::cout << "max(7,i): " << ::max(7,i) << '\n';

  double f1 = 3.4;
  double f2 = -6.7l
  std::cout << "max(f1,f2): " << ::max(f1,f2) << '\n';

  std::string s1 = "mathematics";
  std::string s2 = "math";
  std::cout << "max(s1,s2): " << ::max(s1,s2) << '\n';  
}
```    

Note that each call of the `max()` template is qualified with `::`. This is to ensure that our `max()` template is found in the global namespace. There is also a `std::max()` template in the standard library, which under some circumstances may be called or may lead to ambiguity. For example, if one argument type is defined in the namespace `std` (such as `std::string`), according to the lookup rules for C++, both the global and the `max()` template in `std` are found.

 Templates are not compiled into single entities that can handle any type. Instead, different entities are generated from the template for every type for which the template is used. *Note*: A "one-entity-fits-all" alternative is conceivable but not used in practice (it would be less efficient at run time). All language rules are based on the principle that different entities are generated for different template arguments. Thus, `max()` is compiled for each of these three types. For example, the first call of `max()`:

```cpp
int i = 42;
... max(7,i) ...
```  

uses the function template with `int` as template parameter `T`. Thus, it has the semantics of calling the following code:

```cpp
 int max (int a, int b)
 {
   return b < a ? a : b;
 }
``` 

The process of replacing template parameters by concrete types is called *instantiation*. It results in an *instance* of a template.
*Note*: the terms *instance* and *instantiate* are used in a different context in OOP - namely, for a concrete object of a class. However, in template lingo we use the same term for the "use" of templates unless otherwise specified.
  Note that the mere use of a function template can trigger such an instantiation process. There is no need for the programmer to request the instantiation separately. 
  Similarly, the other calls of `max()` instantiate the `max` template for `double` and `std::string` as if they were declared and implemented individually:
```cpp
  double max (double, double);
  std::string max (std::string, std::string);
```
Note also that `void` is a valid template argument provided the resulting code is valid. For example:
```cpp
 template<typename T>
 T foo(T*)
 {
 }

 void* vp = nullptr;
 foo(vp);  // OK: deduces void foo(void*)
```

## Two-Phase Translation

An attempt to instantiate a template for a type that does not support all the operations used within it will result in a compile-time error. For example:

```cpp
 std::complex<float> c1, c2; // does not provide operator <
 ...
 ::max(c1,c2);    // ERROR at compile time
```

Thus, templates are "compiled" in two phases:
1. Without instantation at *definition time*, the template code itself is checked for correctness ignoring the template parameters. This includes:
- Syntax errors are discovered, such as missing semicolons
- Using unknown names (type names, function names, ...) that don't depend on template parameters are discovered
- Static assertions that don't depend on template parameters are checked
2. At *instantiation time*, the template code is checked (again) to ensure that all code is valid. That is, now especially, all parts that depend on template parameters are double-checked.
 For example:
```cpp
 template<typename T>
 void foo(T t)
 {
   undeclared();   // first-phase compile-time error if undeclared() unknown
   undeclared(t);  // second-phase compile-time error if undeclared(T) unknown
   static_assert(sizeof(int) > 10,   // always fails if sizeof(int) <= 10
                 "int too small");
   static_assert(sizeof(T) > 10,     // fails if instantiated for T with size <= 10
                 "T too small"); 
 }
```
The fact that names are checked twice is called *two-phase lookup*. 
 Note that some compilers don't perform the full checks of the first phase. For example, The Visual C++ compiler in some versions (such as Visual Studio 2013 and 2015) allow undeclared names that don't depend on template parameters and even some syntax flaws (such as a missing semicolon). So you might not see general problems until the template code is instantiated at least once.

#### Compiling and Linking

Two-phase translation leads to an important problem in the handling of templates in practice: When a function template is used in a way that triggers its instantiation, a compiler will (at some point) need to see that template's definition. This breaks the usual compile and link distinction for ordinary functions, when the declaration of a function is sufficient to compile its use. 

# Template Argument Deduction

When we call a function template such as `max()` for some arguments, the template parameters are determined by the arguments we pass. If we pass two `int`s to the parameter types `T`, compiler has to conclude that `T` must be `int`.
  However, `T` might only be "part" of the type. For example, if we declare `max()` to use constant references:
```cpp
 template<typename T>
 T max (T const& a, T const& b)
 {
   return b < a ? a : b;
 }
```
and pass `int`, again `T` is deduced as `int`, because the function parameters match for `int const&`. 

#### Type Conversions During Type Deduction

Note that automatic type conversions are limited during type deduction:
* When declaring call parameters by reference, even trivial conversations do not apply to type deduction. Two arguments declared with the same template parameter `T` must match exactly. 
* When declaring call parameters by value, only trivial conversions that *decay* are supported: Qualifications with `const` or `volatile` are ignored, references convert to the reference type, and raw arrays or functions convert to the corresponding pointer type. For two arguments declared with the same template parameter `T` the *decayed* types must match.
For example:
```cpp
 template<typename T>
 T max (T a, T b);
 ...
 int const c = 42;
 max(i, c);  // OK: T is deduced as int
 max(c, c);  // OK: T is deduced as int
 int& ir = i;
 max(i, ir); // OK: T is deduced as int
 int arr[4];
 foo(&i, arr); // OK: T is deduced as int*
```
However, the following are errors:
```cpp
 max(4, 7.2); // ERROR: T can be deduced as int or double
 std::string s;
 foo("hello", s); // ERROR: T can be deduced as char const[6] or std::string
```
There are three ways to handle such errors:
1. Cast the arguments so that they both match:
```cpp
 max(static_cast<double>(4), 7.2);  // OK
```
2. Specify (or qualify) explicitly the type of T to prevent the compiler from attempting type deduction:
```cpp
 max<double>(4, 7.2);  // OK
```
3. Specify that the parameters may have different types.

#### Type Deduction for Default Arguments

Note also that type deduction does not work for defaut call arguments. For example:
```cpp
 template<typename T>
 void f(T = "");
 ...
 f(1);  // OK: deduced T to be int so that it calls f<int>(1)
 f();   // ERROR: cannot deduce T
```
To support this case, you also have to declare a default argument for the template parameter.
```cpp
  template<typename T = std::string>
  void f(T = "");
  ...
  f();  // OK
```

## Multiple Template Parameters

As we have seen so far, function templates have two distinct sets of parameters:
1. *Template parameters*, which are declared in angle brackets before the function template name:
```cpp
  template<typename T>  // T is template parameter
```

2. *Call parameters*, which are declared in parentheses after the function template name:
```cpp
  T max (T a, T b)  // a and b are call parameters
```
You may have as many template parameters as you like. For example, you could define the `max()` template for call parameters of two potentially different types:
```cpp
 template<typename T1, typename T2>
 T1 max (T1 a, T2 b)
 {
   return b < a ? a : b;
 }
 ...
 auto m = ::max(4, 7.2);  // OK but type of first argument defines return type
```

It may appear desirable to be able to pass parameters of different types of the `max()` template, but, as this example shows, it raises a problem. If you use one of the parameter types as a return type, the argument for the other parameter might get converted to this type, regardless of the caller's intention. Thus the return type depends on the call argument order. The maximum of 66.66 and 42 will be a double while the maximum of 42 and 66.66 wil be an int.
C++ provides different ways to deal with this problem:
* Introduce a third template parameter as a return type
* Let the compiler find out the return type
* Declare the return type to be the "common type" of the two parameter types.

### Template Parameters for Return Types

Our earlier discussion showed that *template argument deduction* allows us to call function templates with syntax identical to that of calling an ordinary function: we do not have to explicitly specify the types corresponding to the template parameters.
 We also mentioned, however, that we can specify the types to use for the template parameters explicitly:
```cpp
 template<typename T>
 T max (T a, T b);
 ...
 ::max<double>(4, 7.2); // instantiate T as double
```
In cases when there is no connection between template and call parameters and when template parameters cannot be determined, you must specify the template argument explicitly with the call. For example, you can introduce a third template argument type to define the return type of a function template:
```cpp
 template<typename T1, typename T2, typename RT>
 RT max (T1 a, T2 b);
``` 
However, template argument deduction does not take return types into account and `RT` does not appear in the types of the function call parameters. Therefore, `RT` cannot be deduced. *Note*: Deduction can be seen as part of overload resolution - a process that is not based on selection of return types either. The sole exception is the return type of conversion operator members. *Note2*: In C++ the return type also cannot be deduced from the context in which the caller uses the call. As a consequence, you have to specify the template argument list explicitly. For example:
```cpp
 template<typename T1, typename T2, typename RT>
 RT max (T1 a, T2 b);
 ...
 ::max<int,double,double>(4, 7.2);  // OK, but tedious
```
So far, we have looked at cases in which either all or none of the function template arguments were mentioned explicitly. Another approach is to specify only the first arguments explicitly and to allow the deduction process to derive the rest. In general, you must specify all the argument types up to the last argument type that cannot be determined implicitly. Thus, if you change the order of the template parameters in our example, the caller needs to specify only the return type:
```cpp
 template<typename RT, typename T1, typename T2>
 RT max (T1 a, T2 b);
 ...
 ::max<double>(4, 7.2);  // OK: return type is double, T1 and T2 are deduced
```  
In this example, the call to `max<double>` explicitly sets `RT` to `double`, but the parameters `T1` and `T2` are deduced to be `int` and `double` from the arguments.
  Note that these modified versions of `max()` do not lead to significant advantages. For the one-parameter version you can already specify the parameter (and return) type if two arguments of a different type are passed.

### Deducing the Return Type

If a return type depends on template parameters, the simplest and best approach to deduce the return type is to let the compiler find out. Since C++14, this is possible by simply not declaring any return type (you still have to declare the return type to be `auto`):

`basics/maxauto`
```cpp
 template<typename T1, typename T2>
 auto max (T1 a, T2 b)
 {
   return b < a ? a : b;
 }
``` 
In fact, the use of `auto` for the return type without a corresponding *trailing return type* (which would be introduced with `a ->` at the end) indicates that the actual return type must be deduced from the return statements in the function body. Or course, deducing the return type from the function body has to be possible. Therefore, the code must be available and multiple return statements have to match.
  Before C++14, it is only possible to let the compiler determine the return type by more or less making the implementation of the function part of its declaration. In C++11 we can benefit from the fact that the *trailing return type* syntax allows us to use the call parameters. That is, we can *declare* that the return type is derived from what `operator?:` yields:

`basics/maxdecltype.hpp`

```cpp
 template<typename T1, typename T2>
 auto max (T1 a, T2 b) -> decltype(b < a ? a : b)
 {
   return b < a ? a : b;
 }
```  
Here, the resulting type is determined by the rules for operator `?:`, which are fairly elaborate but generally produce an intuitively expected result (e.g. if `a` and `b` have different arithmetic types, a common arithmetic type is found for the result).
 Note that
```cpp
 template<typename T1, typename T2>
 auto max (T1 a, T2 b) -> decltype(b < a ? a : b);
```
is a *declaration*, so that the compiler uses the rules of `operator?:` called for parameters `a` and `b` to find out the return type of `max()` at compile time. The implementation does not necessarily have to match. In fact, using `true` as the condition for `operator?:` in the declaration is enough:
```cpp
 template<typename T1, typename T2>
 auto max (T1 a, T2 b) -> decltype(true?a:b);
```
However, in any case this definition has a significant drawback: it might happen that the return type is a reference type, because under some conditions `T` might be a reference. For this reason you should ereturn the type *decayed* from `T`, which looks as follows:
`basics/maxdecltypedecay.hpp`
```cpp
#include <type_traits>

template<typename T1, typename T2>
auto max (T1 a, T2 b) -> typename std::decay<decltype(true?a:b)>::type
{
  return b < a ? a : b;
}
```
Here, the type trait `std::decay<>` is used, which returns the resulting type in a member type. Because the member `type` is a type, you have to qualify the expression with `typename` to access it.
  Note that an initialization of type `auto` always decays. This also applies to return values when the return type is just `auto`. `auto` as a return type behaves just as in the following code, where `a` is declared by the decayed type of `i`, `int`:
```cpp
 int i = 42;
 int const& ir = i; // ir refers to i
 auto a = ir;       // a is declared as new object of type int
```

### Return Type as Common Type

Since C++11, the C++ standard library provides a means to specify choosing "the more general type". `std:common_type<>::type` yields the "common type" of two (or more) different types passed as template arguments. For example:

`basics/maxcommon.hpp`
```cpp
 #include <type_traits>

 template<typename T1, typename T2>
 std::common_type_t<T1,T2> max (T1 a, T2 b)
 {
   return b < a ? a : b;
 }
```
Again, `std::common_type` is a *type trait*, defined in `<type_traits>`, which yields a structure having a type member for the resulting type. Thus, its core usage is as follows:
```cpp
 typename std::common_type<T1,T2>::type // since C++11
```
However, since C++14 you can simplify the usage of traits like this by appending `_t` to the trait name and skipping `typename` and `::type`, so that the return type definition simply becomes:
```cpp
  std::common_type_t<T1,T2>  // equivalent since C++14
```
The way `std::common_type<>` is implemented uses some tricky template programming. Internally, it chooses the resulting type according to the language rules of operator `?:` or specializations for specific types. Thus, both `::max(4, 7.2)` and `::max(7.2, 4)` yield the same value 7.2 of type `double`. Note that `std::common_type<>` also decays.    


## Default Template Arguments

You can also define default values for template parameters. These values are called _default template arguments_ and can be used with any kind of template. Prior to C++ 11, default template arguments were only permitted in class templates, due to a historical glitch in the development of function templates. The default values may even refer to previous template parameters.

For example, if you want to combine the approaches to define the return type with the ability to have multiple parameter types (as discussed in the section before), you can introduce a template parameter `RT` for the return type with the common type of the two arguments as default. Again, we have multiple options:

1. We can use operator `?:` directly. However, because we have to apply operator `?:` before the call parameters `a` and `b` are declared, we can only use their types:

`basics/maxdefault1.hpp`
```cpp
#include <type_traits>

template<typename T1, typename T2, 
         typename RT = std::decay_t<decltype(true ? T1() : T2())>>
RT max (T1 a, T2 b)
{
   return b < a ? a : b;
}
``` 
Note again the usage of `std::decay_t<>` to ensure that no reference can be returned.
Note also that this implementation requires that we are able to call default constructors for the passed types. There is another solution, using `std::declval`, which, however, makes the declaration even more complicated:

`basics/maxdefaultdeclval.hpp`
```cpp
#include <utility>

template<typename T1, typename T2,
         typename RT = std::decay_t<decltype(true ? std::declval<T1>()
                                                  : std::declval<T2>())>>
RT max (T1 a, T2 b)
{
   return b < a ? a : b;
}
```
So in this case the function template `std::declval<>()` is used as a placeholder for an object reference of a specific type. The function does not have a definition and therefore cannot be called (and does not create an object). Hence, it can only be used in unevaluated operands (such as those of `decltype` and `sizeof` constructs). So, instead of trying to create an object, you can assume you have an object of the corresponding type.
To avoid that we have to call a (default) constructor for `T1` and `T2` to be able to call operator `?:` in the expression to initialize `RT`, we use `std::declval` to "use" objects of the corresponding type without creating them. This is only possible in the unevaluated context of `decltype`.

2. We can also use the `std::common_type<>` type trait to specify the default value for the return type:
`basics/maxdefault3.hpp`
```cpp
#include <type_traits>

template<typename T1, typename T2,
         typename RT = std::common_type_t<T1,T2>>
RT max (T1 a, T2 b)
{
  return b < a ? a : b;
}
```
Again, note that `std::common_type<>` decays so that the return type can't become a reference.
In all cases, as a caller, you can now use the default value for the return type:
```cpp
auto a = ::max(4, 7.2);
```
or specify the return type after all other argument types explicitly:
```cpp
auto b = ::max<double,int,long double>(7.2, 4);
```
However, again we have the problem that we have to specify three types to be able to specify the return type only. Instead, we would need the ability to have the return type as the first template parameter, while still being able to deduce it from the argument types. In principle, it is possible to have default arguments for leading function template parameters even if parameters without default arguments follow:

```cpp
template<typename RT = long, typename T1, typename T2>
RT max (T1 a, T2 b)
{
  return b < a ? a : b;
}
```
With this definition, for example, you can call:
```cpp
int i;
long l;
...
max(i, l);  // returns long (default argument of template parameter for return type)
max<int>(4, 42); // returns int as explicitly requested
```

## Overloading Function Templates

Like ordinary functions, function templates can be overloaded. That is, you can have different function definitions with the same function name so that when that name is used in a function call, a C++ compiler must decide which one of the various candidates to call. The rules for this decision may become rather complicated, even without templates. 

## Appendix

### `std::declval`

Defined in header `<utility>`

```cpp
template<class T>
typename std::add_value_reference<T>::type declval() noexcept; // since C++11
```
Converts any type `T` to a reference type, making it possible to use member functions in `decltype` expressions without the need to go through constructors.

`declval` is commonly used in templates where acceptable template parameters may have no constructor in common, but have the same member function whose return type is needed.

Note that `declval` can only be used in _unevaluated contexts_ and is not required to be defined; it is an error to evaluate an expression that contains this function. Formally, the program is ill-formed if this function is odr-used. 

#### Parameters

(none)

#### Return value
Cannot be called and thus never returns a value. The return type is `T&&` unless `T` is (possibly `cv`-qualified) `void`, in which case the return type is `T`.

#### Possible implementation

```cpp
template<typename T> constexpr bool always_false = false;

template<typename T>
typename std::add_rvalue_reference<T>::type declval() noexcept {
   static_assert(always_false<T>, "declval not allowed in an evaluated context");
}
```

```cpp
#include <utility>
#include <iostream>

struct Default { int foo() const { return 1; } };

struct NonDefault
{
  NonDefault() = delete;
  int foo() const { return 1; }
};

int main()
{
   decltype(Default().foo()) n1 = 1;
// decltype(NonDefault().foo()) n2 = n1; // this won't compile
   decltype(std::declval<NonDefault>().foo()) n2 = n1;
   std::cout << "n1 = " << n1 << '\n'
             << "n2 = " << n2 << '\n';
}
```

### `std::common_type`

defined in header `<type_traits>` _(since C++11)_
```cpp
template<class... T>
struct common_type;
```

Determines the common type among all types `T...` that is the type all `T...` can be implicitly converted to. If such a type exists (as determined according to the rules below), the member `type` names that type. Otherwise, there is no member type.

* if `sizeof...(T)` is zero, there is no member type.
* if `sizeof...(T)` is one (i.e. `T...` contains only one type `T0`), the member type names the same type as `std::common_type<T0, T0>::type` if it exists; otherwise there is no member type.
* if `sizeof...(T)` is two (i.e. `T...` contains exactly two types `T1` and `T2`),
  * if applying `std::decay` to at least one of `T1` and `T2` produces a different type, the member type names the same type as `std::common_type<std::decay<T1>::type, std::decay<T2>::type>::type`, if it exists; if not, there is no member type. 
  * otherwise, if there is a user specialization for `std::common_type<T1, T2>`, that specialization is used;
  * otherwise, if `std::decay<decltype(false ? std::declval<T1>() : std::declval<T2>())>::type` is a valid type, the member type denotes that type; 
  * _(since C++ 20)_ otherwise, if ```std::decay<decltype(false ? std::declval<CR1>() : std::declval<CR2>())>::type``` is a valid type, where `CR1` and `CR2` are `const std::remove_reference_t<T1>&` and `const std::remove_reference_t<T2>&` respectively, the member denotes that type;
  * otherwise, there is no member type.
* if `sizeof...(T)` is greater than two (i.e. `T...` consists of the types `T1`, `T2`, `R...`), then if `std::common_type<T1, T2>::type` exists, the member type denotes `std::common_type<typename std::common_type<T1, T2>::type, R...>::type` if such a type exists. In all other cases, there is no member type.

The types in the parameter pack `T` shall each be a complete type, (possibly `cv`-qualified) `void`, or an array of unknown bound. Otherwise, the behavior is undefined.
If an instantiation of a template above depends, directly or indirectly, on an incomplete type, and that instantiation could yield a different result if that type were hypothetically completed, the behavior is undefined.

#### Member types

| Name | Definition |
|----|----|
|type|the common type for all `T...`|

#### Helper types

```cpp
template< class... T >
using common_type_t = typename common_type<T...>::type; 
```
_(since C++ 14)_

#### Specializations

Users may specialize `common_type` for types `T1` and `T2` if

* at least one of `T1` and `T2` depends on a user-defined type, and
* `std::decay` is an identity transformation for both `T1` and `T2`.

If such a specialization has a member named `type`, it must be a public and unambiguous member that names `cv`-unqualified non-reference type to which both `T1` and `T2` are explicitly convertible. Additionally, `std::common_type<T1, T2>::type` and `std::common_type<T2, T1>::type` must denote the same type.
A program that adds `common_type` specializations in violations of these rules has undefined behavior.
Note that the behavior of a program that adds a specialization to any other template (except for `std::basic_common_reference)` _(since C++ 20)_ from `<type_traits>` is undefined.

The following specializations are already provided by the standard library:

* `std::common_type<std::chrono::duration>` _(C++ 11)_ : specializes `std::common_type` trait via class template specialization  
* `std::common_type<std::chrono::time_point>` _(C++ 11)_ : specializes `std::common_type` trait via class template specialization
* `std::common_type<std::pair>` _(C++ 23)_ : determines the common type of two pairs via class templater specialization
* `std::common_type<tuple-like>` _(C++ 23)_ : determines the common type of a tuple and tuple-like type

#### Possible implementation

```cpp
// primary template (used for zero types)
template<class...>
struct common_type {};

// one type
template <class T>
struct common_type<T> : common_type<T, T> {};

namespace detail {
template<class...>
using void_t = void;

template<class T1, class T2>
using conditional_result_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

template<class, class, class = void>
struct decay_conditional_result {};


}

``` 

### Overload Resolution

_Overload resolution_ is the process that selects the function to call for a given call expression. Consider the following simple example:

```cpp
void display_num(int);     // #1
void display_num(double);  // #2

int main()
{
   display_num(399);   // #1 matches better than #2
   display_num(3.99);  // #2 matches better than #1
}
```
In this example, the function name `display_num()` is _overloaded_. When this name is used in a call, a C++ compiler must therefore distinguish between the various candidates using additional information; mostly, this information is the types of the call arguments. In our example, it makes intuitive sense to call the `int` version when the function is called with integer argument and the `double` version when a floating-point argument is provided. The formal process that attempts to model this intuitve choice is the overload resolution process.

The general ideas behind the rules that guide the overload resolution process are simple enough, but the details have become quite complex during the C++ standardization process. This complexity was driven mostly by the desire to support various real-wrold examples that intuitively (to a human) seem to have an "obviously best match" but when trying to formalize this intuition various subtleties arose.

#### When Does Overload Resolution Kick In?

Overload resolution is just one part of the complete processing of a function call. In fact, it is not part of every function call. First, calls through function pointers and calls through pointers to member functions are not subject to overload resolution because the function to call is entirely determined (at run time) by the pointers. Second, function-like macros cannot be overloaded and are therefore not subject to overload resolution.
   At a very high level, a call to a named function can be processed in the following way:
* the name is looked up to form an initial _overload set_.
* if necessary, this set is adjusted in various ways (e.g. template argument deduction and substitution occurs, which can cause some function template candidates to be discarded).
* any candidate that doesn't match the call at all (even after considering implicit conversions and default arguments) is eliminated from the overload set. This results in a set of _viable function candidates_. 
* _overload resolution_ is performed to find a _best_ candidate. If there is one, it is selected; otherwise, the call is ambiguous.
* the selected candidate is checked. For example, if it is a deleted function (i.e. one defined with `= delete`) or an inaccessible private member function, a diagnostic is issued.

Each of these steps has its own subtelties, but overload resolution is arguably the most complex. Fortunately, a few simple principles clarify the majority of situations.

#### Simplified Overload Resolution

Overload resolution ranks the viable candidate functions by comparing how each argument of the call matches the corresponding parameter of the candidates. For one candidate to be considered better than another, the better candidate cannot have any of its parameters to be a worse match than the corresponding parameter in the other candidate. The following example illustrates this:

```cpp
 void combine (int , double);
 void combine (long, int);

 int main()
 {
   combine(1, 2); // ambiguous!
 } 
``` 
In this example, the call to `combine()` is ambiguous because the first candidate matches the first argument (the literal 1 of type `int`) _best_, whereas the second candidate matches the second argument _best_. We could argue that `int` is in some sense closer to `long` than to `double` (which supports choosing the second candidate), but C+ does not attempt to define a measure of closeness that involves multiple call arguments. 

Given this first principle we are left with specifying how well a given argument matches the corresponding parameter of a viable candidate. As a first approximation, we can rank the possible matches as follows (from best to worst):

1. Perfect match. The parameter has the type of the expression or it has a type that is a reference to the type of the expression (possibly with added `const` and/or `volatile` qualifiers).
2. Match with minor adjustments. This includes, for example, the decay of an array variable to a pointer to its first element or the addition of `const` to match an argument of type `int\*\*` to a parameter of type `int const \* const \*`. 
3. Match with promotion. Promotion is a kind of implicit conversion that includes the conversion of small integral types (such as `bool`, `char`, `short`, and sometimes enumerations) to `int`, `unsigned int`, `long` or `unsigned long` and the conversion of `float` to `double`.
4.   
