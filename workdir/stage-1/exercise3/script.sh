 yacc -d  tree.y

 lex tree.l

 gcc y.tab.c lex.yy.c -o exe

 ./exe

 cd "/mnt/d/nitc curriculum/sem5/compiler design lab/xsm_expl"

 ./xsm -l ../files/stage-1/library.lib -e ../files/stage-1/exercise3/target_file.xsm

 cd -
rm y.tab.c
rm y.tab.h
rm lex.yy.c
rm exe
rm target_file.xsm
