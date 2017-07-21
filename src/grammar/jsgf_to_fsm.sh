#!/bin/bash

name=$1

sphinx_jsgf2fsg -jsgf $name.jsgf -fsm compiled/$name.fsm -symtab compiled/$name.syms || exit 1
#cp $name.fsm $name.syms /home/iceberg/qworksheet || exit 1
