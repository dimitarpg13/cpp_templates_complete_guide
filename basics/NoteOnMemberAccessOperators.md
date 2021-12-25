# Member Access Operators

Accesses a member of its operand


| Operator name | Syntax | Overloadable | Prototype examples (for `class T`) inside class definition | Prototype examples (for `class T`) outside class definition |
|:-------------:|:------:|:------------:|:----------------------:|:------------------------:|
|  _subscript_    |  `a[b]`  |    Yes       | `R& T::operator[](S b);`  |           N/A            |
|                 |          |              | `R& T::operator[](S1 s1, ...);` (**since C++23**) | N/A  |
|  _indirection_  |  `*a`    |    Yes       | `R& T::operator*();` | `R& operator*(T a);` |
|  _address-of_   |  `&a`    |    Yes       | `R* T::operator&();` | `R* operator&(T a);` |
|  _member of object_ | `a.b` |  No  |  N/A  |  N/A |
|  _pointer to member of object_ | `a.*b` | No | N/A | N/A |  
|  _pointer to member of pointer_ | `a->*b` | Yes | `R& operator->*(S b);` | `R& operator->*(T a, S b);` |

_Notes_:
As with most user-defined overloads, return types should match return types provided by the built-in operators so that the user-defined operators can be used in the same manner as the built-ins. However, in a user-defined operator overload, any type can be used as a return type (including `void`). One exception is `operator->` which must return a pointer or another class with overloaded operator `->` to be realistically usable.

## Explanation

Built-in *subscript* operator provides access to an object pointed-to by the pointer or array operand
Built-in *indirection* operator provides access to an object or function pointed-to by the pointer operand
Built-in *address-of* operator creates a pointer pointing to the object or function operand
*Member of object* and *pointer to member of object* operators provide access to a data member or member function of the object operand
Built-in *member of pointer* and *pointer to member of pointer* operators provide access to a data or member function of the class pointed-to by the pointer operand.

### Build-in subscript operator

The subscript operator expressions have the form
--------------
*expr1* `[` *expr2* `]`          (1)
--------------
*expr1* `[ {` *expr*, `...}]`        (2)    (**C++11**)
--------------
*expr1* `[` *expr2*, *expr*, `...]`      (3)   (**C++23**)
--------------
1) For the built-in operator, one of the expressions (either *expr1* or *expr2*) must be a glvalue of type "array of T" or a prvalue of type "pointer to T", while the other expression (*expr2* or *expr1*, respectively) must be a prvalue of unscoped enumeration or integral type. The result of this expression has the type T. *expr2* cannot be a unparenthesized comma expression (**since C++23**).
2) The form with brace-enclosed list inside the square brackets is only used to call an overloaded `operator[]`.
3) The form with comma-separated expression list inside the square brackets is only used to call an overloaded `operator[]`.

The built-in subscript expression `E1[E2]` is exactly identical to the expression `*(E1 + E2)` except evaluation order (**since C++17**), that is, the pointer operand (which may be a result of array-to-pointer conversion, and which must point to an element of some array or one past the end) is adjusted to point to another element of the same array, following the rules of pointer arithemtics and is then dereferenced.
When applied to an array, the subscript expression is an lvalue if the array is an lvalue, and an xvalue if it isn't (**since C++11**).
When applied to a pointer, the subscript expression is always an lvalue.
The type `T` is not allowed to be an incomplete type, even if the size or internal structure of `T` is never used, as in `&x[0]`. 

-------------------
Using an unparenthesized comma expression as second (right) argument of a subscript operator is deprecated. For example `a[b,c]` is deprecated and `a[(b,c)]` is not (**since C++20**), (**until C++23**)
-------------------
An unparenthesized comma expression cannot be second (right) argument of a subscript operator. For example `a[b, c]` is either ill-formed or equivalent to `a.operator[](b, c)`. (**since C++23**)
Parentheses are needed to for using a comma expression as the subscript, e.g. `a[(b,c)]`.
-------------------
In overload resolution against user-defined operators for every object type `T` (possibly `cv`-qualified), the following function signature participates in overload resolution:
-------------------
```cpp
 T& operator[](T*, std::ptrdiff_t);
```
-------------------
```cpp
 T& operator[](std::ptrdiff_t, T*);
```
-------------------

