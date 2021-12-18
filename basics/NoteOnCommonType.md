# Note on `std::common_type` (from cppreference.com)

`std::common_type`
Defined in `<type_traits>`
```cpp
template< class... T >  // since C++ 11
struct common_type;
```

Determines the common type among all types `T...`, that is the type all `T...` can be implicitly converted to. If such a type exists (as determined according to the rules below), the member type names that type. Otherwise, there is no member type.
* if `sizeof...(T)` is zero, there is no member type
* if `sizeof...(T)` is one (i.e. `T...` contains only one type `T0`), the member type names the same type as `std::common_type<T0, T0>::type` if it exists; otherwise there is no member type.
* if `sizeof...(T)` is two (i.e. `T...` contains exactly two types `T1` and `T2`),
  * if applying `std::decay` to at least one of `T1` and `T2` produces a different type, the member type names the same type as `std::common_type<std::decay<T1>::type, std::decay<T2>::type>::type`, if it exists; if not, there is no member type.
  * otherwise, if there is a user specialization for `std::common_type<T1, T2>`, that specialization is used;
  * otherwise, if   
