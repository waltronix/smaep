smaep
=====

smaep is a **SMA**\ ll **E**\ xpression **P**\ arser.
It can parse simple mathematical expressions into an abstract syntax tree and
evaluate or print that tree.

I started smaep, because I wanted to use LLVM_ to build a JIT compiler
(as explained in the the KaleidoscopeJIT_ tutorial)
and PEGTL_ to parse something by using grammar.

.. _LLVM: https://llvm.org
.. _KaleidoscopeJIT: https://llvm.org/docs/tutorial/BuildingAJIT1.html
.. _PEGTL: https://github.com/taocpp/PEGTL

smaep can be used as a command line calculator. But it is also possible to
reuse a parsed expression with different sets of data.

Usage
-----

smaep provides a basic command line interface that takes expressions as
arguments.

::

    > smaep "1 + 1"

      1 + 1 = 2

Printing the syntax tree can be activated by using **-a**.
At the moment, the address of the tree items is printed as identifier.
::

    > smaep -a "(3^2 + 4^2)^(0.5)"

      (3^2 + 4^2)^(0.5) = 5

    ast
       └ ^                             - 0x0x55a0e473dbd0
         └ +                           - 0x0x55a0e473de00
           └ ^                         - 0x0x55a0e473ded0
             └ 3.000000                - 0x0x55a0e473d0b0
             └ 2.000000                - 0x0x55a0e473d520
           └ ^                         - 0x0x55a0e473da90
             └ 4.000000                - 0x0x55a0e473e030
             └ 2.000000                - 0x0x55a0e473c1f0
         └ 0.500000                    - 0x0x55a0e473bdf0

In addition, it is possible to add a *data source* that can be queried during
evaluation of the expression. The command line tool contains some *lookups* for
the numbers from one to ten.
::

    > smaep "1 + data[one]" -a

      1 + data[one] = 2

    ast
       └ +                             - 0x0x5571084a7eb0
         └ 1.000000                    - 0x0x5571084a6300
         └ "one"                       - 0x0x5571084a7de0

Thanks to
---------

My special thanks go to the following projects:

* https://github.com/taocpp/PEGTL
* https://github.com/danielaparker/jsoncons
* https://github.com/catchorg/Catch2

* https://cliutils.gitlab.io/modern-cmake/


How to build
------------

smaep is build using cmake.

There are some options to control what to build.

BUILD_CLI_TOOL 
  Build the smaep cli calculator.

BUILD_JSON_SOURCE 
  Build json data source.

BUILD_TESTS 
  Build unit tests for all components.

By default these options are activated.


How it works
------------

The following diagram give a rough overview, on what happens during parsing.

The single values, operators and functions are extracted by PEGTL_ 
and pushed into the `tree_nursery`. In there, they are converted into a tree
by using the `Shunting Yard Algorithm`_. And thats it.

.. _Shunting Yard Algorithm: https://en.wikipedia.org/wiki/Shunting-yard_algorithm

.. plantuml::

  boundary smaep
  participant tree_nursery
  collections nodes
  boundary pegtl

  [-> smaep: parse(\n  expression, \n  operators);
  activate smaep
      smaep -> tree_nursery **
      activate tree_nursery #LightGreen
          smaep -> pegtl: parse\n  <grammar, actions>\n  (expr, ops, tr);
          activate pegtl
              group during expression parsing 
                  opt value
                      pegtl -> pegtl: make node
                      pegtl -> nodes**
                      activate nodes #LightSalmon
                      pegtl -> tree_nursery: push(node)
                  else operation or function
                      pegtl -> tree_nursery: push(&function)
                  end
              end
          pegtl --> smaep
          deactivate pegtl

          smaep -> tree_nursery: get_ast()
          activate tree_nursery
              tree_nursery -> nodes: transfer\n ownership\n of nodes
          smaep <-- tree_nursery
          deactivate tree_nursery
          
          [<- smaep: ast
      destroy tree_nursery
  deactivate smaep
