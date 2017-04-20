Assignment Checklist
====================

Checklist for bachelors work assignment.

Basic language structures
-------------------------

Supported features of structured programming:

- [x] data types (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp))
    - [x] integer numbers
    - [x] strings
    - [x] void type
    - [X] arrays reference
    - [x] class reference (see classes)
- [x] expressions:
    - [x] arithmetical operations: `+`, `-`, `*`, `/`
    - [x] array manipulation: `array[idx]`, `array.length`
    - [x] function call: `foo()`
    - [ ] safety: types of arguments passed to function call must match types in function declaration
- [x] statements: (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp))
    - [x] assignment: `name = expr;`
    - [x] array element assignment: `array[idx] = val;`
    - [x] variable `var` and value `let` declaration
    - [x] safety: value in declaration must much `let` and `var` type, or type can be inferred
    - [ ] safety: type of assigned value must match target type in assignment statement
    - [ ] safety: type of assigned value must match target type in array assignment statement
- [x] control structures: (covered in: [03-cycles.bp](/examples/03-cycles.bp))
    - [x] conditions: `if`, `if-else`
    - [x] while cycle: `while (condition) statement;`
    - [x] for cycle: `for (i: iterable) statment;`

Simple OOP support:

- [x] class declarations: (covered in: [04-classes.bp](/examples/04-classes.bp))
    - [x] class type declaration: `class Name { ... }`
    - [x] members/variables: `class Name { var X : i32; var Y = 123; ... }`
    - [x] methods: `class Name { fn foo() -> type { ... } }`
    - [x] simple inheritance: `class Name: Super { ... }`
- [x] class reference expressions: (covered in: [04-classes.bp](/examples/04-classes.bp))
    - [x] variables: `obj.x`
    - [x] method call: `obj.method()`

Region based memory management
------------------------------

Supported features of region based memory management. Covers region management, and application of safe regions on implemented language functionality:

- [x] region management (covered in: [05-regions.bp](/examples/05-regions.bp), TODO: automated tests):
    - [x] creation of region on scope
    - [x] automatic destruction of region at scope exit
- [x] allocation of objects within region (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [x] default allocation on current/default region
- [ ] reference types with regions (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [x] inferred types with regions from assigned value for `let` and `var` statements 
    - [ ] explicit types with regions for `let` and `var` statements
    - [x] use default region of current scope when not specified
    - [x] declare instance regions for array types, `ClassType @InstanceR [] @ArrayR`
    - [x] safety: type checking  `ClassType @R`, `i64[] @R`, `ClassType @InstanceR [] @ArrayR`
    - [x] safety: region of assigned value must match region of variable type
- [ ] declaration of functions, and function calls, on/with regions (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [x] declaration of multiple in/out regions (in lexical meaning, static analysis)
    - [x] explicit types with regions for parameter type
    - [x] remapping region for return value: `fn foo @A () -> i64[] @A {...};`, type of `foo @B ()` is `i64[] @B`
    - [ ] passing handle default/current/first region (for allocation, TODO test for real allocation)
    - [x] safety: region of assigned call arguments must match regions of parameter types - remap regions

Other
-----

Quality requirements:

- [ ] values and variables of inner scopes aren't propagated to parrent scope
- [ ] automated tests for all safety checks
- [ ] nice output

Features not required by an assignment (future improvement):

- [ ] allocation of objects within region:
    - [ ] allocation on specified region
- [ ] calling functions on/with regions (TODO: test cases, examples):
    - [ ] passing other regions (handle for allocation, not lexical scope)
- [ ] class reference properties to data on different regions
    - [ ] region arguments for class types (like generics)
    - [ ] safety check: instance region may not outlive region parameters
