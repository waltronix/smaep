smaep
=====

something like "small/simple/speedy math expression parser"

.. plantuml::
   :caption: Caption with **bold** and *italic*

   Bob -> Alice: hello
   Alice -> Bob: hi

Sample output:
::

    λ ./smaep.exe "2 * (abs(-3.2) + data[one])"
    2 * (abs(-3.2) + data[one]) = 8.4
    ast
    |- *                     - 0x000001FC84EE1500
        |- 2.000000            - 0x000001FC84EE51D0
        |- +                   - 0x000001FC84EE1440
        |- abs()             - 0x000001FC84EE13E0
            |- -3.200000       - 0x000001FC84EE4A00
        |- "one"             - 0x000001FC84EF1DC0
