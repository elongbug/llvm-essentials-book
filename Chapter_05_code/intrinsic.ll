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

!0 = !{!"clang version 3.8.0 (trunk 247500) (llvm/trunk 247501)"}
