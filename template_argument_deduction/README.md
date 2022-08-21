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
Here the first call argument is of type `int`, so the parameter `T` of our original `max()` template is tentatively deduced to be `int`. The second call argument is a `double`, however, and so `T` should be `double` for this argument which conflicts with the previous conclusion.  
