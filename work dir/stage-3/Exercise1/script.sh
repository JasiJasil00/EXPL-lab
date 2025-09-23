 yacc -d  tree.y

 lex tree.l

 gcc y.tab.c lex.yy.c -o exe

 ./exe largest.txt

# cd "/mnt/d/nitc curriculum/sem5/compiler design lab/xsm_expl"

# ./xsm -l ../files/stage-3/library.lib -e ../files/stage-3/target_file.xsm --debug
 ##cd "/mnt/d/nitc curriculum/sem5/compiler design lab/xsm_expl"

 ##./xsm -l ../files/stage-2/library.lib -e ../files/stage-1/exercise2/target_file.xsm

#cd -
rm y.tab.c
rm y.tab.h
rm lex.yy.c
rm exe
