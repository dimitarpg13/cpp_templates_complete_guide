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
