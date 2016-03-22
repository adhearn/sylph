# sylph
A simple lisp compiler, targeting x86

# Build Instructions
The default target of the Makefile will build an executable named `parse`. I haven't tested it on anything other than Arch Linux, so YMMV. Note that the Makefile is currently very stupid, and doesn't build with the default versions of flex and bison on OS X (not to mention that you need to use `-ll` instead of `-lfl` when compiling). This will likely change soon, since I actually have an OS X machine.

# Parsing Sample Input
`sample.ss` contains an example of the structure of what we can currently parse. The executable reads from stdin, so to see a sample parse, just do `./parse < sample.ss`. The output is currently not very helpful - one of the next things I want to add is some sort of pretty-printer to make things a little bit easier to read.

# Current Status
Currently working on a compiler around an IR that I'm calling parenthesized assembly, or PASM. Right now, it can parse a tiny subset of what I want out of PASM, basically just arithmetic and assignment to identifiers (which secretly have to be register names). Emitting the correct instructions for such a small subset of assembly should be easy, and is the next obvious step. After that, I'll be adding conditionals, jumps, and memory operations. Eventually, I'll be adding actual identifiers, which will require register allocation, and enforcing calling conventions, which will bring us up to a language that is starting to actually look a bit like an impoverished version of Scheme ('JS-with-parens-but-not-objects').

In BNF, the current language looks like:

```
program   ::== (program <pasm-stmt>* retval)
pasm-stmt ::== (set! ident val)
          ::== (set! ident1 (binop ident1 val))
retval    ::== (return val)
binop     ::== + | - | *
val       ::== INTEGER | IDENTIFIER | #t | #f
```
