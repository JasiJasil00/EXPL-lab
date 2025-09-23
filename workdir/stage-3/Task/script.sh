#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

# build
yacc -d tree.y
lex tree.l
gcc y.tab.c lex.yy.c -o exe

# run with input file (inside input files/ dir)
./exe "input files/$1"

#run label translation
lex label.l
gcc  lex.yy.c -o exe

./exe target_file.xsm

# run in xsm
cd "/mnt/d/nitc curriculum/sem5/compiler design lab/xsm_expl" || exit 1
./xsm -l ../workdir/stage-3/Task/library.lib -e ../workdir/stage-3/Task/translatedFile.xsm --debug

# cleanup
cd - >/dev/null
rm -f y.tab.c y.tab.h lex.yy.c exe target_file.xsm translatedFile.xsm
