hxl
===
**hexl** [_filename_]

A simple hex viewer inspired by [hexyl](https://github.com/sharkdp/hexyl).
I wanted to write a simple clone in C but got a bit carried away with
optimisations.

The other major differences from hexyl are that hxl does not draw boxes around
its output, does not use non-ASCII glyphs, and uses only basic VT100
sequences. This to enhance compatibility and composability.

Installation
------------
Should work on most Unix systems, including Linux, BSD, and macOS.

    make
    sudo install hxl /usr/local/bin

Author
------
Sijmen J. Mulder (<ik@sjmulder.nl>)
