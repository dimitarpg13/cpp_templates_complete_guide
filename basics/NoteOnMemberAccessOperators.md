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



