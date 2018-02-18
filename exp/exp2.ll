; ModuleID = '<stdin>'
source_filename = "exp2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@count = global i32 0, align 4
@.str = private unnamed_addr constant [17 x i8] c"Hello world: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"%f seconds\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define void @exe_duration() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  store i32 0, i32* %1, align 4
  br label %5

; <label>:5:                                      ; preds = %32, %0
  %6 = load i32, i32* %1, align 4
  %7 = icmp slt i32 %6, 10000
  br i1 %7, label %8, label %35

; <label>:8:                                      ; preds = %5
  store i32 1000, i32* %2, align 4
  store i32 3, i32* %3, align 4
  store i32 4, i32* %4, align 4
  %9 = load i32, i32* %3, align 4
  %10 = load i32, i32* %4, align 4
  %11 = mul nsw i32 %9, %10
  %12 = load i32, i32* @count, align 4
  %13 = add nsw i32 %12, %11
  store i32 %13, i32* @count, align 4
  %14 = load i32, i32* @count, align 4
  %15 = icmp slt i32 %14, 10000
  br i1 %15, label %16, label %21

; <label>:16:                                     ; preds = %8
  %17 = load i32, i32* @count, align 4
  %18 = add nsw i32 %17, 100
  store i32 %18, i32* @count, align 4
  %19 = load i32, i32* %3, align 4
  %20 = add nsw i32 %19, 3
  store i32 %20, i32* %3, align 4
  br label %21

; <label>:21:                                     ; preds = %16, %8
  br label %22

; <label>:22:                                     ; preds = %25, %21
  %23 = load i32, i32* %2, align 4
  %24 = icmp sge i32 %23, 0
  br i1 %24, label %25, label %31

; <label>:25:                                     ; preds = %22
  %26 = load i32, i32* %2, align 4
  %27 = add nsw i32 %26, -1
  store i32 %27, i32* %2, align 4
  %28 = load i32, i32* %3, align 4
  %29 = load i32, i32* @count, align 4
  %30 = add nsw i32 %29, %28
  store i32 %30, i32* @count, align 4
  br label %22

; <label>:31:                                     ; preds = %22
  br label %32

; <label>:32:                                     ; preds = %31
  %33 = load i32, i32* %1, align 4
  %34 = add nsw i32 %33, 1
  store i32 %34, i32* %1, align 4
  br label %5

; <label>:35:                                     ; preds = %5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i64, align 8
  %3 = alloca i64, align 8
  %4 = alloca double, align 8
  store i32 0, i32* %1, align 4
  %5 = call i64 @clock() #3
  store i64 %5, i64* %2, align 8
  call void @exe_duration()
  %6 = load i32, i32* @count, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i32 %6)
  %8 = call i64 @clock() #3
  store i64 %8, i64* %3, align 8
  %9 = load i64, i64* %3, align 8
  %10 = load i64, i64* %2, align 8
  %11 = sub nsw i64 %9, %10
  %12 = sitofp i64 %11 to double
  %13 = fdiv double %12, 1.000000e+06
  store double %13, double* %4, align 8
  %14 = load double, double* %4, align 8
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), double %14)
  ret i32 0
}

; Function Attrs: nounwind
declare i64 @clock() #1

declare i32 @printf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (tags/RELEASE_501/final)"}
