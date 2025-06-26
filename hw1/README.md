# Homework 1: Command-line Arguments and Bits

__Due Monday, July 7, 2025 at 11:59pm__

In this assignment, you will build a number of tools to give you practice
with command-line arguments, manipulation of bits, and PPM images.

!!! tip ""
    * Read the entire assignment first before you start
    * Start early and do not do all of the assignment in one sitting; coding is
        fun but fighting for hours with broken code is not
    * Do not hesitate to seek help if you are stuck

## Synopsis

__`wc`__: You will change your `wc` utility from `hw0` to work with command line arguments.

__`oct2perm`__: given a Unix permission number triple, prints the human-readable
permission string.

__`perm2oct`__: given the human-readable permission string, prints the number
triple &mdash; the inverse of the above.

__`hex2rgb`__: given a hex color, prints the individual color values.

__`rgb2hex`__: given three color values from 0 to 255, prints the hex color.

__`color`__: generates a solid color PPM image.


__Learning Objectives__:

* Develop a deeper understanding of numbers and practice bit manipulations
* Get comfortable with I/O
* Practice more C

## Getting started

We will keep using the `coursework` repository, same repository where you wrote
`hw0`. First of all, you should run `git status` to see if you have any
uncommitted changes; if so, `commit` and `push` them before proceeding.

Run `git pull upstream main`. Doing this almost certainly triggers an automerge
by `git`. When `vim` is launched that shows the merge commit message, press
`<esc>` `:` `x` `<enter>` to save and quit the editor (look at the bottom-left
corner of your screen if you don't know where you're typing).

Run `ls` to confirm that you have received a `hw1` directory.

Pay attention to any error messages that you might encounter and please ask for
help if you run into any problems.

### What is in the starter repository?

* `reference-*`: this directory contains reference implementations compiled for
    an operating system.
* `oct2perm.c`, `perm2oct.c`, `hex2rgb.c`, `rgb2hex.c`, `color.c`:
    these are empty source files for you to implement the respective programs.

## Specification

In this assignment, hexadecimal numbers use only uppercase letters from [A-F].

### `wc`

You need to alter your program to work either with the standard input or files.
If no arguments have been passed, your program should work the same as `hw0`.
Every command line argument must be a valid, readable file. You should print
with the same specification as `hw0`, but now the columns are aligned **for all**
files, not just one. Additionally, the fourth column will contain the name of the
file. If there is more than one file, print an additional line with the counts of
all files.

If you are not able to read a file, instead of counts you should print the name
of the executable, the name of the file and the `fopen` error message.

The number of files is up to 128. If more files are passed in, print a helpful
message and exit with `EXIT_FAILURE` or `1`.

```angular2html
$ ./wc143 < tests/hello.txt
 1  2 14
$ ./wc143 tests/hello.txt
 1  2 14 tests/hello.txt
$ ./wc143 tests/hello.txt tests/all-letters.txt
  1   2  14 tests/hello.txt
  2  18  90 tests/all-letters.txt
  3  20 104 total
$ ./wc143 tests/hello.txt tests/all-letters.txt non-existent
  1   2  14 tests/hello.txt
  2  18  90 tests/all-letters.txt
./wc143: non-existent: No such file or directory
  3  20 104 total
```

If any of the files cannot be processed, your program should exit with `EXIT_FAILURE` or `1`.

### `oct2perm`

Consume a three-digit octal number as a command-line argument; generate a nine
character Unix file permission string corresponding to that octal number. A
Unix file permission string usually starts with a `d` (for directory) or `-`
(try `ls -l` in a crowded directory and look at the left of each line), but
that leading character is not part of this exercise.

A Unix file permission string consists of three sets of three "bits" each,
written `rwx` for read, write, and execute, in that order.
Either the letter `r`, or `w`, or `x`, appears when that permission status is
enabled; a hyphen appears when that permission is disabled.
These permission strings are really just bits; the `r`, `w` and `x` are
mnemonics to make the bit strings easier for people to read, but they could, in
principle, be written (for example) `111000101` rather than `rwx---r-x`.
Each three-character substring in the permission string corresponds to the per-
missions of the _owner_ of the file, the _group_ of the file, and the _world_
(everyone), in that order.
Writing `111000101` all the time is annoying and error-prone, so Unix system
uses three base-8 numbers (_octal_ numbers), each of which represents one
three-bit permission.

Here are a few examples of running `oct2perm` on different three-digit octals:
```
$ ./oct2perm 400
r--------
$ ./oct2perm 440
r--r-----
$ ./oct2perm 772
rwxrwx-w-
```

`oct2perm` takes one argument, containing a string of three single-digit
numbers, whose range is 0-7 inclusively.

If `oct2perm` is given more than one argument or the argument contains anything
other than three numbers ranging from 0-7, the program should exit with
`EXIT_FAILURE` or `1` after printing a helpful error message.
You can choose what message to print.

!!! tip ""
    On Linux or Mac OS, you can use `chmod` to change the permission of a file.
    `chmod <nnn> <file>` changes the permission of the file named `<file>` to
    the permission represented by the octal number triple described above.

### `perm2oct`

The `perm2oct` tool is the inverse of `oct2perm`; it consumes a 9-character
permission string as its command-line argument, and reports the corresponding
octal number.

Here are a few examples of running `perm2oct`:
```
$ ./perm2oct rw-r-x--x
651
$ ./perm2oct rwxr-xr-x
755
$ ./perm2oct rw-------
600
```

!!! tip ""
    Note that an individual character can be read from a string using subscript
    notation. This is to say, assuming `argv[1]` is `"rwxr--rw-"`, then
    `argv[1][0]` is `'r'`, `argv[1][1]` is `'w'`, and so on, up to `argv[1][8]`,
    which is `'-'`, and terminated by `argv[1][9]` which is `'\0'`.

The permission string is exactly 9 characters.

* The 0th, 3rd, and 6th character can only be either `'r'` or `'-'`.
* The 1st, 4th, and 7th character can only be either `'w'` or `'-'`.
* The 2nd, 5th, and 8th character can only be either `'x'` or `'-'`.

If `perm2oct` is given more than one argument, or the permission string contains
any invalid character or has the wrong length, the program should exit with
`EXIT_FAILURE` or `1` after printing a helpful error message.

!!! error ""
    It is possible to hard-code all input combinations. For example, an octal
    digit has 7 cases and their respective permission strings can be hard-coded.
    *Do not* do this as it defeats the purpose of this exercise and you will be
    marked down.

### `rgb2hex`

The `rgb2hex` tool consumes three decimal numbers, each on the interval
[0,255], and produces a hex color string, which, starting with `0x`, each of
red, green, blue is represented by two hex digits in that order.
Examples:

```
$ ./rgb2hex 0 0 255
0x0000FF
$ ./rgb2hex 0 255 0
0x00FF00
$ ./rgb2hex 255 0 0
0xFF0000
$ ./rgb2hex 3 5 7
0x030507
```

If `rgb2hex` is given the incorrect number of arguments or any of arguments is
not a number on the interval [0,255], the program should exit with
`EXIT_FAILURE` or `1` after printing a helpful error message.

!!! tip ""
    Look up `atoi()` function in C.

### `hex2rgb`

This tool is the inverse of `rgb2hex`. The argument is a hexadecimal number.

```
$ ./hex2rgb 0x0000FF
0 0 255
$ ./hex2rgb 0x00FF00
0 255 0
$ ./hex2rgb 0xFF0000
255 0 0
$ ./hex2rgb 0xABCDEF
171 205 239
```

If `hex2rgb` is given the incorrect number of arguments or the argument is not
a 6-digit hexadecimal number starting with `0x`, the program should exit with
`EXIT_FAILURE` or `1` after printing a helpful error message.

### `color`

This tool consumes the following arguments in order:

* Width, an positive integer
* Height, an positive integer
* Color, a 6-digit hexadecimal number starting with `0x`.

This tool then prints a PPM image of the given size to the standard output of a
single, solid color indicated by the hex color string.

A PPM image starts `P3`, then the width and the height, the maximum
color value, in our case, the maximum color value is 255, and finally a raster
of color triple in decimal.
The number of color triples is equal to `width * height`.

```
$ ./color 3 4 0xAA0000
P3
3 4
255
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
170 0 0
```

To view the generated PPM image, you can redirect the output to a file, e.g.
`./color 200 300 0xAA0000 > redish.ppm`.

* On Mac, you can run `open redish.ppm` to view the image via Preview.
* On WSL/Linux, you can run `display redish.ppm` to view via `imagemagick`.
* If you are using VSCode, there is an extension that allows you to view PPM images directly in the editor.

!!! tip ""
    You might be tempted to import your code from `hex2rgb` as a part of this
    program. It is a good practice to reuse code, but we have not learnt how to
    do it in C yet. If you want to reuse your code for `hex2rgb`, feel free to copy/paste it here.

### Crashing vs Aborting

__Crashing__: An unexpected and abrupt termination of your C program due to
errors like segmentation faults or division by zero. It's an indication of a bug
and should be avoided at all costs, as it may result in data loss or system
instability.

__Aborting__: A deliberate stoppage of your program when an unrecoverable error
or violation of assumptions is detected. It is preferable to crashing, as it's a
controlled exit, informs the user of the error, and allows for post-mortem
analysis.
This is usually done by using the `exit(EXIT_FAILURE)` function in `<stdlib.h>`.
C also has assertions; the `assert(condition)` function in `<assert.h>`
&mdash; `assert` will abort the program if the `condition` is not met.
These two methods are both valid for aborting a program.

In this course, unless otherwise specified, your program must not crash, and
crashing in testing will incur a higher penalty than aborting or producing wrong
results.

## Testing

You are provided with reference implementations for all parts in this homework.
If you have questions about the specifications, try running the reference
implementations; if any of the reference implementations is inconsistent with
the written specification, let me know immediately.
If the executables do not run on your operating system, let me know as well.

There is no provided test cases. You should test your programs frequently and
with as many inputs as you need to be convinced that the program
behaves correctly on all possible inputs.

The command to compile each program is the same as `hw0`, repeated here for
convenience:
```sh
clang -std=c11 -Wall -Wextra -pedantic -o <name> <name>.c
```

## Submission checklist

Everything below is inside `hw1` directory of your coursework repository.

* `oct2perm.c`
* `perm2oct.c`
* `hex2rgb.c`
* `rgb2hex.c`
* `color.c`
* `WRITTEN.md`
* all changes are committed and pushed to your github repository

Submit your program to Gradescope by selecting your coursework directory and the
correct branch.

## Grading

|       | Percentage |
|-------| -----------|
| Correctness | 70% |
| Style       | 20% |
| Written     | 10% |

__Warning__: If your program cannot be compiled using the commands above without
error or warning, you will receive 0 points in correctness since there is
no executables for us to run.
