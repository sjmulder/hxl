hxl
===
**hxl** [_filename_]

Simple hex viewer inspired by [hexyl](https://github.com/sharkdp/hexyl).
I wanted to write a simple clone in C but got a bit carried away with
optimisations.

A totally unscientific test with a ~420 MB video file on my Debian stretch
machine with hxl 0.3.1:

    $ time hexyl largefile >/dev/null
    1m00.87s real     0m55.00s user     0m05.87s system
    $ time ./hxl largefile >/dev/null
    0m09.44s real     0m09.15s user     0m00.28s system

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
