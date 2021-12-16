# c-shell

This program demonstrates a naive implementation of a bash shell, written in C. This shell currently supports program execution, piping, and a variety of commands that bash supports. Quality-of-life features such as autocomplete and tab scroll are not implemented in this version.

## Compiling

This program provides a `makefile` to assist in the compiling of this program. To run `make`, simply type:

```
$ make
```
to create an executible object file `a.out`.

## Running

To run this program (after compiling), type:

```
$ ./a.out
```
to initialize the shell program. A shell symbol `:-)` will appear, and you can type any shell commands.

## Cleaning

Use the make file to clean any and all object files generated during compilation. Type:

```
$ make clean
```

to do so.
