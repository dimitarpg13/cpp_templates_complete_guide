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
  * otherwise, if `std::decay<decltype(false ? std::declval<T1>() : std::declval<T2>())>::type` is a valid type, the member type denotes that type;
  * (***since C++20***) otherwise, if `std::decay<decltype(false ? std::declval<CR1>() : std::declval<CR2>())>::type` is a valid type, where `CR1` and `CR2` are `const std::remove_reference_t<T1>&` and `const std::remove_reference_t<T2>&` respectively, the member type denotes that type;  
  * otherwise, there is no member type
* if `sizeof...(T)` is greater than two (i.e. `T...` consists of the types `T1`, `T2`, `R...`), then if `std::common_type<T1, T2>::type` exists, the member type denotes `std::common_type<typename std::common_type<T1, T2>::type, R...>::type` if such a type exists. In all other cases, there is no member type.

The types in the parameter pack `T` shall each be a complete type, (possibly cv-qualified) `void`, or an array of unknown bound. Otherwise, the behavior is undefined.
If an instantiation of a template above depends, directly or indirectly, on an incomplete type, and that instantiation could yield a different result if that type were hypothetically completed, the behavior is undefined.

## Member types

`type` : the common type for all `T...`

## Helper types

(***since C++14***)
```cpp
 template< class... T >
 using common_type_t = typename common_type<T...>::type;
```

## Specializations

Users may specialize `common_type` for types `T1` and `T2` if
* at least one of `T1` and `T2` depends on a user-defined type, and
* `std::decay` is an identity transformation for both `T1` and `T2`
If such a specialization has a member named type, it must be a public and unambiguous member that names a cv-unqualified non-reference type to which both `T1` and `T2` are explictly convertible. Additionally, `std::common_type<T1, T2>::type` and `std::common_type<T2, T1>::type` must denote the same type. A program that adds `common_type` specialziations in violation of these rules has unidentified behavior. 

## Possible implementation

```cpp
// primary template (used for zero types)
template<class...>
struct common_type {};

//////// one type
template <class T>
struct common_type<T> : common_type<T, T> {};

namespace detail {
template<class...>
using void_t = void;

template<class T1, class T2>
using conditional_result_t = decltype(false ? std::declval<T1>() : std::declval<T2>());

template<class, class, class = void>


}
``` 
