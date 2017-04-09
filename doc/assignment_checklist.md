Assignment Checklist
====================

Checklist for bachelors work assignment.

Language functional requirements
--------------------------------

Supported features of structured programming:

- [ ] data types (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp)):
    - [x] integer numbers
    - [x] strings
    - [X] arrays reference
    - [ ] void type
- [ ] expressions:
    - [ ] arithmetical operations: `+`, `-`, `*`, `/`
    - [x] array manipulation: `array[idx]`, `array.length`
    - [ ] function call: `foo()`
- [x] statements: (covered in: [02-basic-syntax.bp](/examples/02-basic-syntax.bp)):
    - [x] assignment: `name = expr;`
    - [x] array element assignment: `array[idx] = val;`
- [ ] control structures:
    - [ ] conditions: `if`, `if-else`
    - [x] while cycle: `while (condition) statement;`
    - [x] for cycle: `for (i: iterable) statment;`
- [ ] class declarations:
    - [x] class type declaration: `class Name { ... }`
    - [ ] members/variables: `class Name { var X : i32; var Y = 123; ... }`
    - [x] methods: `class Name { fn foo() -> type { ... } }`
    - [ ] simple inheritance: `TODO`
- [ ] class expressions:
    - [ ] variables: `TODO`
    - [ ] method call: `obj.method()`

Supported features of region based memory management:

- [ ] region management:
    - [ ] creation of region on scope
    - [ ] automatic destruction of region at scope exit
- [ ] allocation of objects within region
- [ ] types defining regions
- [ ] calling functions on/with regions


Language quality requirements
-----------------------------

N/A:

- [ ] values and variables of inner scopes aren't propagated to parrent scope