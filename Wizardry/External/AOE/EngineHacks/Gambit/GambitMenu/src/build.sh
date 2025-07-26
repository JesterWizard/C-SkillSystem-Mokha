FileName='GambitMenuCore'

rm $FileName.o $FileName.asm $FileName.lyn.event
make $FileName.asm
make $FileName.o
make "../../include/_Definitions.h.o"
make "../../include/MokhaRAMSpace.o"
make $FileName.lyn.event
rm *.o
cd "../../include"
rm *.o