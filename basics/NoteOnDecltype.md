# `decltype` specifier

Inspects the declared type of an entity or the type and value category of an expression.

## Syntax

(**since C++11**) `decltype (` *entity* `)`       (1)
(**since C++11**) `decltype (` *expression* `)`   (2) 

## Explanation

1) If the argument is an unparenthesized id-expression or an unparenthesized class member access expression,
then `decltype` yeilds the type of the _entity_ named in this expression. If there is no such entity, or if the argument names a set of overloaded functions, the program is ill-formed.
--
if the argument is an unparametrized id-expression naming a structured binding, then decltype yields the _referenced type_ (described in the specification of the structured binding declaration). (since **C++17**) 
