# Template Argument Deduction

Explicitly specifying template arguments on every call to a function template (e.g. `concat<std::string, int>(s, 3)`) can quickly lead to unwieldy code. Fortunately, a C++ compiler can often automatically determine the intended template arguments using a powerful process called _template argument deduction_.
Although template argument deduction was first developed to ease the invocation of function templates, it has since been broadened to apply to several other uses, including determining the types of variables from their initializers.

## The Deduction Process

The basic deduction process compares the types of an argument of a function call with the corresponding parametrized type of a function template and attempts to conclude the correct substitution for one or more of the deduced parameters. Each argument-parameter pair is analyzed independently, and if the conclusions differ in the end, the deduction process fails. Consider the following example:

```cpp
template<typename T>
T max (T a, T b)
{
  return b < a ? a : b;
}

auto g = max(1, 1.0);
```
Here the first call argument is of type `int`, so the parameter `T` of our original `max()` template is tentatively deduced to be `int`. The second call argument is a `double`, however, and so `T` should be `double` for this argument which conflicts with the previous conclusion. Note that we say that "the deduction process fails", not that "the program is invalid". After all, it is possible that the deduction process would succeed for another template named `max` (function templates can be overloaded like ordinary functions).

If all the deduced template parameters are consistently determined, the deduction process can still fail if substituting the arguments in the rest of the function declaration results in an invalid construct. For example:

```cpp
 template<typename T>
 typename T::ElementT at (T a, int i)
 {
    return a[i];
 }

 void f (int* p)
 {
    int x = at(p, 7);
 }
``` 
Here `T` is concluded to be `int*` (there is only one parameter type where `T` appears, so there are obviously no analysis conflicts). However, substituting `int*` for `T` in the return type `T::ElementT` is clearly invalid C++, and the deduction process fails.

We still need to explore how argument-parameter matching proceeds. We describe it in terms of matching a type _A_ (derived from the call argument type) to a parametrized type _P_ (derived from the call parameter declaration). If the call parameter is declared with a reference declarator, _P_ is taken to be the type referenced, and _A_ is the type of the argument. Otherwise, however, _P_ is the declared parameter type, and _A_ is obtained from the type of the argument by _decaying_ array and function types to pointer types, ignoring top-level `const` and `volatile` qualifiers. For example:

```cpp
template<typename T> void f(T);   // parametrized type P is T
template<typename T> void g(T&);  // parametrized type P is also T

double arr[20];
int const seven = 7;

f(arr);       // non-reference parameter: T is double*
g(arr);       // reference parameter: T is double[20]
f(seven);     // non-reference parameter: T is int
g(seven);     // reference parameter: T is int const
f(7);         // non-reference parameter: T is int
g(7);         // reference parameter: T is int => ERROR: can't pass 7 to int&
```

For a call `f(arr)`, the array type of `arr` decays to type `double*`, which is the type deduced for `T`.  
