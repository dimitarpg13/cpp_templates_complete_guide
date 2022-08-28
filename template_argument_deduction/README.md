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

For a call `f(arr)`, the array type of `arr` decays to type `double*`, which is the type deduced for `T`. In `f(seven)` the `const` qualification is stripped and hence `T` is deduced to be `int`. In contrast, calling `g(x)` deduces `T` to be type `double[20]` (no decay occurs). Similarly, `g(seven)` has an lvalue argument of type `int const`, and because `const` and `volatile` qualifiers are not dropped when matching reference parameters, `T` is deduced to be `int const`.However, note that `g(7)` would deduce `T` to be `int` (because nonclass rvalue expressions never have `const` or `volatile` qualified types), and the call would fail because an argument `7` cannot be passed to a parameter of type `int &`.
The fact that no decay occurs for arguments bound to reference parameters can be surprising when the arguments are string literals. Reconsider our `max()` template declared with references:

```cpp
template<typename T>
T const& max(T const& a, T const& b);
```

It would be reasonable to expect that for the expression `max("Apple", "Pie")` `T` is deduced to be `char const*`. However, the type of `"Apple"` is `char const[6]`, and the type of `"Pie"` is `char const[4]`. No array-to-pointer occurs (because the deduction involves reference parameters), and therefore `T` would have to be both `char[6]` and `char[4]` for deduction to succeeds. That is, of course, impossible. 

## Deduced Contexts

Parametrized types that are considerably more complex than just `T` can be matched to a given argument type. Here are a few examples that are still fairly basic:

```cpp
template<typename T>
void f1(T*);

template<typename E, int N>
void f2(E(&)[N]);

template<typename T1, typename T2, typename T3>
void f3(T1 (T2::*)(T3*));

class S {
public:
  void f(double*);
};

void g (int*** ppp)
{
   bool b[42];
   f1(ppp);     // deduces T to be int**
   f2(b);       // deduces E to be bool and N to be 42
   f3(&S::f);   // deduces T1 = void, T2 = S, and T3 = double 
}
```

Complex type declarations are built from more elementary constructs (pointer, reference, array, and function declarators; pointer-to-member declarators; template-ids; and so forth), and the matching process proceeds from the top-level construct and recurses through the composing elements. It is fair to say that the most type declaration constructs can be matched in this way, and these are called _deduced contexts_. However, a few constructs are not deduced contexts. For example:

* Qualified type names. For example, a type name like `Q<T>::X` will never be used to deduce a template parameter `T`. * Nontype expressions that are not just a nontype parameter. For example, a type name like `S<I+1>` will never be used to deduce `I`. Neither will `T` be deduced by matching against a parameter of type `int(&)[sizeof(S<T>)]`.

This limitations should come as no surprise because the deduction would, in general, not be unique (or even finite), although this limitation of qualified type names is sometimes easily overlooked. A nondeduced context does not automatically imply that the program is in error or even that the parameter being analyzed cannot participate in type deduction. To illustrate this consider the following more intricate example:

```cpp
template<int N>
class X {
    public:
       using I = int;
       void f(int) {
           
       }
};

template<int N>
void fppm(void (X<N>::*p)(typename X<N>::I));

int main()
{
    fppm(&X<33>::f);  //fine: N deduced to be 33
}
```

In the function template `fppm()`, the subconstruct `X<N>::I` is a nondeduced context. However, the member-class component `X<N>` of the pointer-to-member type is a deducible context, and when the parameter `N`, which is deduced from it, is plugged in the nondeduced context, a type compatible with that of the actual argument `&X<33>::f` is obtained. The deduction therefore succeeds on that argument-parameter pair.
  
Conversely, it is possible to deduce contradictions for a parameter type entirely built from deduced contexts. For example, assuming suitably declared class templates `X` and `Y`:

```cpp
template<typename T>
void f(X<Y<T>, Y<T>>);

void g()
{
    
}
```