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

    
