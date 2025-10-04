#!/bin/bash

# go to project directory
cd "/mnt/d/nitc curriculum/sem5/compiler design lab/workdir/stage-4/Task/" || exit 1

# check input argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <filename>"
    exit 1
fi

INPUT_FILE="input files/$1"

if [[ ! -f "$INPUT_FILE" ]]; then
    cd "./input files"
    echo "file names"
    ls
    echo "Error: Input file '$INPUT_FILE' does not exist!"
    exit 1
fi

# build
yacc -d tree.y
lex tree.l
gcc y.tab.c lex.yy.c -o exe

# run compiler on input
./exe "$INPUT_FILE"

# run label translation
lex label.l
gcc lex.yy.c -o exe

TARGET_FILE="target_file.xsm"

if [[ ! -f "$TARGET_FILE" ]]; then
    cd ./scripts
    ./clean.sh
    echo "Error: '$TARGET_FILE' was not generated!"
    exit 1
fi

./exe "$TARGET_FILE"

# run in xsm
cd "/mnt/d/nitc curriculum/sem5/compiler design lab/xsm_expl" || exit 1

TRANSLATED_FILE="../workdir/stage-4/Task/translatedFile.xsm"

if [[ ! -f "$TRANSLATED_FILE" ]]; then
    cd ./scripts
    ./clean.sh
    echo "Error: '$TRANSLATED_FILE' not found!"
    exit 1
fi

./xsm -l ../workdir/stage-4/Task/library.lib -e "$TRANSLATED_FILE" --debug

# cleanup
cd "/mnt/d/nitc curriculum/sem5/compiler design lab/workdir/stage-4/Task/" || exit 1
rm -f y.tab.c y.tab.h lex.yy.c exe target_file.xsm translatedFile.xsm
