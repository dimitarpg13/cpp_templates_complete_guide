template<typename T1, typename T2>
auto max (T1 a, T2 b) -> decltype(b < a ? a : b)
{
  // if b < a then yield a else b
  return b < a ? a : b;
}
