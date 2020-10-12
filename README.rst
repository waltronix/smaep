|pipeline status| |code coverage|

.. |pipeline status| image:: https://gitlab.com/waltronix/smaep/badges/main/pipeline.svg
   :alt: | pipeline status |
.. |code coverage| image:: https://gitlab.com/waltronix/smaep/badges/main/coverage.svg
   :alt: | code coverage |

smaep
=====

smaep is a **SMA**\ ll **E**\ xpression **P**\ arser.
It can parse simple mathematical expressions into an abstract syntax tree and
evaluate or print that tree.

.. I started smaep, because I wanted to use LLVM_ to build a JIT compiler
.. (as explained in the the KaleidoscopeJIT_ tutorial)
.. and PEGTL_ to parse something by using grammar.

.. .. _LLVM: https://llvm.org
.. .. _KaleidoscopeJIT: https://llvm.org/docs/tutorial/BuildingAJIT1.html
.. .. _PEGTL: https://github.com/taocpp/PEGTL

smaep can be used as a command line calculator. But it is also possible to
reuse a parsed expression with different sets of data.

I use gitlab for CI, so have a look at https://gitlab.com/waltronix/smaep if 
you want to have a closer look at the build results and some sample calls.

Usage
-----

smaep provides a basic command line interface that takes expressions as
arguments.

::

    > smaep_cli -e "1 + 1"
      
        1 + 1
      
      1 + 1 = 2


Printing the syntax tree can be activated by using **-t**.
At the moment, the address of the tree items is printed as identifier.
::

    > smaep_cli -t -e "(3 + 4)/2"
      
        (3 + 4)/2
      
      
      ast
         └ /                             - 0x0x55d181024b90
           └ +                           - 0x0x55d181024c60
             └ 3.000000                  - 0x0x55d181023030
             └ 4.000000                  - 0x0x55d181024250
           └ 2.000000                    - 0x0x55d181025260
      
      (3 + 4)/2 = 3.5

In addition, it is possible to add a *data source* that can be queried during
evaluation of the expression. The command line tool is able to query data from
json files via json path.
::

    > smaep_cli -t -e "(data[\$.A.x] + data[\$.B.x])/2" -f smaep_cli/samples/temp_0*                                                                                                                         
      
        (data[$.A.x] + data[$.B.x])/2
      
      
      ast
         └ /                             - 0x0x55eda6c57c00
           └ +                           - 0x0x55eda6c57e30
             └ "$.A.x"                   - 0x0x55eda6c57f00
             └ "$.B.x"                   - 0x0x55eda6c57ac0
           └ 2.000000                    - 0x0x55eda6c57060
      
      smaep_cli/samples/temp_01.json : 2
      smaep_cli/samples/temp_02.json : 6


Thanks to
---------

My special thanks go to the following projects:

* https://github.com/taocpp/PEGTL
* https://github.com/danielaparker/jsoncons
* https://github.com/catchorg/Catch2
* https://github.com/CLIUtils/CLI11

* https://cliutils.gitlab.io/modern-cmake/
* https://github.com/StableCoder/cmake-scripts

How to build
------------

smaep is build using cmake.

There are some options to control what to build.

SMAEP_BUILD_CLI_TOOL 
  Build the smaep cli calculator.

SMAEP_BUILD_JSON_SOURCE 
  Build json data source.

SMAEP_BUILD_TESTS 
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
