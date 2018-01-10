; ModuleID = 'instcombineoutput.ll'

define i32 @0(i32 %x, i32 %y, i32 %z) {
  %1 = xor i32 %y, %z
  %res = and i32 %1, %x
  ret i32 %res
}
