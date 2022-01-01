# `decltype` specifier

Inspects the declared type of an entity or the type and value category of an expression.

## Syntax

(**since C++11**) `decltype (` *entity* `)`       (1)

(**since C++11**) `decltype (` *expression* `)`   (2) 

## Explanation

1) If the argument is an unparenthesized id-expression or an unparenthesized class member access expression,
then `decltype` yeilds the type of the _entity_ named in this expression. If there is no such entity, or if the argument names a set of overloaded functions, the program is ill-formed.

   if the argument is an unparametrized id-expression naming a structured binding, then decltype yields the _referenced type_ (described in the specification of the structured binding declaration). (since **C++17**) 

   if the argument is an unparenthesized id-expression naming a non-type template parameter, then `decltype` yields the type of the template parameter (after performing any necessary type deduction if the template parameter is declared with a placeholder type). The type is non-const even if the entity is a template parameter object (which is a const object).  (since **C++20**)

2) If the argument is any other expression of type `T`, and 

   a) if the value category of *expression* is *xvalue*, then `decltype` yields `T&&`;

   b) if the value category of *expression* is *lvalue*, then `decltype` yields `T&`;

   c) if the value category of *expression* is *prvalue*, then `decltype` yields `T`;

        
    
