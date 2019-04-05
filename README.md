# SandhillSkipper

SelectScript virtual machine implementation in C

## Source

https://github.com/andre-dietrich/SandhillSkipper

## Installation

```bash
git clone --recursive https://github.com/andre-dietrich/SandhillSkipper
```

## Usage

You can either generate a `libS2` with `make lib` for dynamically linking or
simply include this repository into your project and include it into your
Makefile.

It includes a complete virtual machine implementation, that can even be used
on embedded microcontrollers with less than 32KB of flash. See therefore the
S2ino project at:

https://github.com/andre-dietrich/S2ino

... a portation to Arduino

Although it can be used also by other languages, it is mainly used to
implement a VM for the SelectScript programming language. The bytecode
compiler for this can be downloaded from:

https://github.com/andre-dietrich/SelectScriptC


## Todo

[ ] documentation

[ ] yield

[ ] async

## License

This project is licensed under the MIT License - see the LICENSE.md file for
details

Copyright (C) 2016-2017

André Dietrich <dietrich@ivs.cs.uni-magdeburg.de>
