hxl
===
**hxl** [_filename_]

<pre>
$ head -n100 hxl | hxl
<span style="color:purple;">7f </span><span style="color:teal;">45 4c 46 </span><span style="color:purple;">02 01 01 </span><span style="color:gray;">00  00 00 00 00 00 00 00 00  </span><span style="color:purple;">.</span><span style="color:teal;">ELF</span><span style="color:purple;">...</span><span style="color:gray;">0 00000000
</span><span style="color:purple;">03 </span><span style="color:gray;">00 </span><span style="color:teal;">3e </span><span style="color:gray;">00 </span><span style="color:purple;">01 </span><span style="color:gray;">00 00 00  </span><span style="color:olive;">f0 </span><span style="color:green;">0a </span><span style="color:gray;">00 00 00 00 00 00  </span><span style="color:purple;">.</span><span style="color:gray;">0</span><span style="color:teal;">&gt;</span><span style="color:gray;">0</span><span style="color:purple;">.</span><span style="color:gray;">000 </span><span style="color:olive;">.</span><span style="color:green;">_</span><span style="color:gray;">000000
</span><span style="color:teal;">40 </span><span style="color:gray;">00 00 00 00 00 00 00  </span><span style="color:teal;">40 2c </span><span style="color:gray;">00 00 00 00 00 00  </span><span style="color:teal;">@</span><span style="color:gray;">0000000 </span><span style="color:teal;">@,</span><span style="color:gray;">000000
00 00 00 00 </span><span style="color:teal;">40 </span><span style="color:gray;">00 </span><span style="color:teal;">38 </span><span style="color:gray;">00  </span><span style="color:purple;">09 </span><span style="color:gray;">00 </span><span style="color:teal;">40 </span><span style="color:gray;">00 </span><span style="color:purple;">1f </span><span style="color:gray;">00 </span><span style="color:purple;">1e </span><span style="color:gray;">00  0000</span><span style="color:teal;">@</span><span style="color:gray;">0</span><span style="color:teal;">8</span><span style="color:gray;">0 </span><span style="color:purple;">.</span><span style="color:gray;">0</span><span style="color:teal;">@</span><span style="color:gray;">0</span><span style="color:purple;">.</span><span style="color:gray;">0</span><span style="color:purple;">.</span><span style="color:gray;">0
</span><span style="color:purple;">06 </span><span style="color:gray;">00 00 00 </span><span style="color:purple;">05 </span><span style="color:gray;">00 00 00  </span><span style="color:teal;">40 </span><span style="color:gray;">00 00 00 00 00 00 00  </span><span style="color:purple;">.</span><span style="color:gray;">000</span><span style="color:purple;">.</span><span style="color:gray;">000 </span><span style="color:teal;">@</span><span style="color:gray;">0000000
</span><span style="color:teal;">40 </span><span style="color:gray;">00 00 00 00 00 00 00  </span><span style="color:teal;">40 </span><span style="color:gray;">00 00 00 00 00 00 00  </span><span style="color:teal;">@</span><span style="color:gray;">0000000 </span><span style="color:teal;">@</span><span style="color:gray;">0000000
</span><span style="color:olive;">f8 </span><span style="color:purple;">01 </span><span style="color:gray;">00 00                                       </span><span style="color:olive;">.</span><span style="color:purple;">.</span><span style="color:gray;">00
</span></pre>

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
