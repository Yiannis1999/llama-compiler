# llama-compiler
Llama is a relatively simple language that combines the functional and imperative programming models. It is based on a subset of OCaml, with which it shares many similarities. Its main features in brief are the following:
- Powerful type system with type inference.
- Basic data types for integers, characters, boolean values and real numbers.
- Pointers and tables of one or more dimensions. The memory locations where references and array elements point are mutable and assignable.
- Programmer-defined data types, possibly recursive as well.
- High-order functions, but without partial evaluation. Passing parameters by value.
- Function library.
## Requirements
| Software  | Version |
|-----------|---------|
| make      | 4.3     |
| flex      | 2.6.4   |
| bison     | 3.8.2   |
| g++       | 11.2.0  |
| llvm-12   | 12.0.1  |
## Usage
```sh
./llamac [-O] <file.lla>
```
```sh
./llamac [options]
```
Options
| Flag | Description                           |
|------|---------------------------------------|
| -O   | Optimization flag.                    |
| -f   | Input from stdin, final code in stdout.|
| -i   | Input from stdin, intermediate code in stdout.|
| -p   | Input from stdin, AST in stdout.       |

Examples
```sh
./llamac -O ./programs/hello.lla
```
```sh
./llamac -O -f
```
```sh
./llamac -i
```
Note: You first need to compile the compiler.
## Linking
```sh
gcc <file.s> ./lib/lib.a -o <file.out> -lm -no-pie
```
Note: You first need to compile the library (i.e., run `make` inside the `lib` dir).
