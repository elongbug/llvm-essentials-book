Using opt tool:

Use the opt tool in the code bundle to compile the testcase test.ll in code bundle
with various optimization levels:


$ opt -O1 -S test.ll > test_out_O1.ll
$ opt -O2 -S test.ll > test_out_O2.ll

***************************************************

Pass and Pass Manager


For the pass to work, place the FnNamePrint folder in llvm/lib/Transforms folder and also replace the CMakeList.txt and Makefile with the given CmakeList.txt and Makefile in the code bundle.
It will generate LLVMFnNamePrint.so in llvm/build-folder/lib, also opt is built

To run it on the testfile use the command with opt tool present in the code bundle. 
Use the .so file present in the code bundle:

$ opt -load LLVMFnNamePrint.so -funcnameprint test.ll


***************************************************
Use -debug-pass=Structure option of opt tool:

$ opt -O2 -S test.ll -debug-pass=Structure


***************************************************
Instruction Combining example

Relace the InstCombineAndOrXor.cpp file in llvm/lib/Transforms/InstCombine folder
with the one present in the code bundle and build LLVM. You will get the new opt
tool with this optimization enabled. This opt tools is present in the code bundle. Use the following command to see instruction combining at work:

$ opt -S -instcombine instcombinetc.ll -o instcombineoutput.ll

$ cat instcombineoutput.ll

***************************************************
