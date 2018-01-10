define i32 @hadd(i32* %a) {
entry:
%0 = load i32, i32* %a, align 4
%arrayidx1 = getelementptr inbounds i32, i32* %a, i32 1
%1 = load i32, i32* %arrayidx1, align 4
%add = add nsw i32 %0, %1
%arrayidx2 = getelementptr inbounds i32, i32* %a, i32 2
%2 = load i32, i32* %arrayidx2, align 4
%add3 = add nsw i32 %add, %2
%arrayidx4 = getelementptr inbounds i32, i32* %a, i32 3
%3 = load i32, i32* %arrayidx4, align 4
%add5 = add nsw i32 %add3, %3
ret i32 %add5
}

