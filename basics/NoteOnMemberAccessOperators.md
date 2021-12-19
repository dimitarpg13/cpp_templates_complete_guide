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


