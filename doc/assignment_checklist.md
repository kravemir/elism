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

Supported features of region based memory management:

- [ ] region management (TODO: test cases, examples):
    - [x] creation of region on scope
    - [x] automatic destruction of region at scope exit
    - [ ] automated tests for regions
- [ ] allocation of objects within region:
    - [x] default allocation on current/default region
    - [ ] allocation on specified region
- [ ] calling functions on/with regions (TODO: test cases, examples):
    - [x] passing default/current region
    - [ ] passing other regions (handle for allocation, not lexical scope)
- [ ] regions of reference types (TODO: test cases, examples):
    - [x] use default region of current scope when not specified
    - [ ] use own regions for array types, `ClassType @InstanceR [] @ArrayR`
    - [ ] safety check: `ClassType @R`, `i64[] @R`, `ClassType @InstanceR [] @ArrayR`
    - [x] safety check: region of assigned value must match region of variable type
    - [x] safety check: region of assigned call arguments must match regions of parameter types - remap regions
- [ ] class reference properties to data on different regions
    - [ ] region arguments for class types (like generics)
    - [ ] safety check: instance region may not outlive region parameters


Language quality requirements
-----------------------------

N/A:

- [ ] values and variables of inner scopes aren't propagated to parrent scope