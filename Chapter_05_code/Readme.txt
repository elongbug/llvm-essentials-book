LICM Testcase

$ cat licm.ll
define void @func(i32 %i) {
        br label %Loop
Loop:
        %j = phi i32 [ 0, %0 ], [ %Val, %Loop ]
        %i2 = mul i32 %i, 17
        %Val = add i32 %j, %i2
        %cond = icmp eq i32 %Val, 0
        br i1 %cond, label %Exit, label %Loop
Exit:            
        ret void
}

run the licm pass using opt:
$ opt -licm licm.ll -o licm.bc

convert .bc file to readable IR format:
$ llvm-dis licm.bc -o licm_opt.ll

View the output:
$ cat licm_opt.ll 
; ModuleID = 'licm.bc'

define void @func(i32 %i) {
  %i2 = mul i32 %i, 17
  br label %Loop

Loop:                                             ; preds = %Loop, %0
  %j = phi i32 [ 0, %0 ], [ %Val, %Loop ]
  %Val = add i32 %j, %i2
  %cond = icmp eq i32 %Val, 0
  br i1 %cond, label %Exit, label %Loop

Exit:                                             ; preds = %Loop
  ret void
}


****************************************************************************


Scalar Evolution:

Testcase file scalev1.ll

$ cat scalev1.ll 
define void @fun() {
entry:
        br label %header
header:
        %i = phi i32 [ 1, %entry ], [ %i.next, %body ]
        %cond = icmp eq i32 %i, 10
        br i1 %cond, label %exit, label %body
body:
        %a = mul i32 %i, 5
        %b = or i32 %a, 1
        %i.next = add i32 %i, 1
        br label %header
exit:        
        ret void
}


Run scalar evolution pass on testcase:
$ opt -analyze -scalar-evolution scalev1.ll 
Printing analysis 'Scalar Evolution Analysis' for function 'fun':
Classifying expressions for: @fun
  %i = phi i32 [ 1, %entry ], [ %i.next, %body ]
  -->  {1,+,1}<%header> U: [1,11) S: [1,11)		Exits: 10
  %a = mul i32 %i, 5
  -->  {5,+,5}<%header> U: [5,51) S: [5,51)		Exits: 50
  %b = or i32 %a, 1
  -->  %b U: [1,0) S: full-set		Exits: 51
  %i.next = add i32 %i, 1
  -->  {2,+,1}<%header> U: [2,12) S: [2,12)		Exits: 11
Determining loop execution counts for: @fun
Loop %header: backedge-taken count is 9
Loop %header: max backedge-taken count is 9

 
**********************************************************************


LLVM Intrinsic Example:

Testcode in intrinsic.cpp file:
$ cat intrinsic.cpp
int func()
{
        int a[5];

        for (int i = 0; i != 5; ++i)
                a[i] = 0;

        return a[0];
}


convert testcode to .ll form
$ clang -emit-llvm -S intrinsic.cpp 

view the testcode in readable IR form.
$ cat intrinsic.ll
; ModuleID = 'intrinsic.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define i32 @_Z4funcv() #0 {
  %a = alloca [5 x i32], align 16
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %1

; <label>:1                                       ; preds = %8, %0
  %2 = load i32, i32* %i, align 4
  %3 = icmp ne i32 %2, 5
  br i1 %3, label %4, label %11

; <label>:4                                       ; preds = %1
  %5 = load i32, i32* %i, align 4
  %6 = sext i32 %5 to i64
  %7 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 %6
  store i32 0, i32* %7, align 4
  br label %8

; <label>:8                                       ; preds = %4
  %9 = load i32, i32* %i, align 4
  %10 = add nsw i32 %9, 1
  store i32 %10, i32* %i, align 4
  br label %1

; <label>:11                                      ; preds = %1
  %12 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 0
  %13 = load i32, i32* %12, align 16
  ret i32 %13
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}


Run O1 optimization to see effect of LLVM intrinsics
$ opt -O1 intrinsic.ll -S -o intrinsic1.ll

$ cat intrinsic1.ll 
; ModuleID = 'intrinsic.ll'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind readnone uwtable
define i32 @_Z4funcv() #0 {
  %a = alloca [5 x i32], align 16
  %a2 = bitcast [5 x i32]* %a to i8*
  call void @llvm.memset.p0i8.i64(i8* %a2, i8 0, i64 20, i32 16, i1 false)
  %1 = getelementptr inbounds [5 x i32], [5 x i32]* %a, i64 0, i64 0
  %2 = load i32, i32* %1, align 16
  ret i32 %2
}

; Function Attrs: nounwind argmemonly
declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) #1

attributes #0 = { nounwind readnone uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind argmemonly }

!llvm.ident = !{!0}

*******************************************************************************



