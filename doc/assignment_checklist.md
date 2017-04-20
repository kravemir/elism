Assignment Checklist
====================

Checklist for bachelors work assignment.

Language functional requirements
--------------------------------

Supported features of structured programming:

- [ ] data types (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp))
    - [x] integer numbers
    - [x] strings
    - [X] arrays reference
    - [ ] void type
- [ ] expressions:
    - [x] arithmetical operations: `+`, `-`, `*`, `/`
    - [x] array manipulation: `array[idx]`, `array.length`
    - [ ] function call: `foo()`
- [x] statements: (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp))
    - [x] assignment: `name = expr;`
    - [x] array element assignment: `array[idx] = val;`
- [x] control structures: (covered in: [03-cycles.bp](/examples/03-cycles.bp))
    - [x] conditions: `if`, `if-else`
    - [x] while cycle: `while (condition) statement;`
    - [x] for cycle: `for (i: iterable) statment;`
- [x] class declarations: (covered in: [04-classes.bp](/examples/04-classes.bp))
    - [x] class type declaration: `class Name { ... }`
    - [x] members/variables: `class Name { var X : i32; var Y = 123; ... }`
    - [x] methods: `class Name { fn foo() -> type { ... } }`
    - [x] simple inheritance: `class Name: Super { ... }`
- [x] class reference expressions: (covered in: [04-classes.bp](/examples/04-classes.bp))
    - [x] variables: `obj.x`
    - [x] method call: `obj.method()`

Supported features of region based memory management. Covers region management, and application of safe regions on implemented language functionality:

- [x] region management (covered in: [05-regions.bp](/examples/05-regions.bp), TODO: automated tests):
    - [x] creation of region on scope
    - [x] automatic destruction of region at scope exit
- [x] allocation of objects within region (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [x] default allocation on current/default region
- [ ] reference types with regions (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [ ] inferred types with regions from assigned value for `let` and `var` statements 
    - [ ] explicit types with regions for `let` and `var` statements, and for argument type
    - [x] use default region of current scope when not specified
    - [x] declare instance regions for array types, `ClassType @InstanceR [] @ArrayR`
    - [x] safety check: `ClassType @R`, `i64[] @R`, `ClassType @InstanceR [] @ArrayR`
    - [x] safety check: region of assigned value must match region of variable type
- [ ] declaration of functions, and function calls, on/with regions (covered in: [05-regions.bp](/examples/05-regions.bp), N/A: automated tests):
    - [x] declaration of multiple in/out regions (in lexical meaning, static analysis)
    - [ ] explicit types with regions for parameter type
    - [ ] remapping region for return value: `fn foo @A () -> i64[] @A {...};`, type of `foo @B ()` is `i64[] @B`
    - [x] passing handle default/current region (for allocation)
    - [x] safety check: region of assigned call arguments must match regions of parameter types - remap regions


Features not required by an assignment (future improvement):

- [ ] allocation of objects within region:
    - [ ] allocation on specified region
- [ ] calling functions on/with regions (TODO: test cases, examples):
    - [ ] passing other regions (handle for allocation, not lexical scope)
- [ ] class reference properties to data on different regions
    - [ ] region arguments for class types (like generics)
    - [ ] safety check: instance region may not outlive region parameters

Language quality requirements
-----------------------------

N/A:

- [ ] values and variables of inner scopes aren't propagated to parrent scope