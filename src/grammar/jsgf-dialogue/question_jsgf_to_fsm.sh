#!/bin/bash

name=$1
type=$2

sphinx_jsgf2fsg -jsgf $type/$name.jsgf -fsm $type/compiled/$name.fsm -symtab $type/compiled/$name.syms || exit 1
#cp $name.fsm $name.syms /home/iceberg/qworksheet || exit 1
