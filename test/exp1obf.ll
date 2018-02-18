; ModuleID = '<stdin>'
source_filename = "exp1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [17 x i8] c"Hello world: %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [12 x i8] c"%f seconds\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"pause\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define i32 @main() #0 {
  %phi130.reload.reg2mem = alloca i32
  %phi155.reload.reg2mem = alloca i32
  %phi38.reload.reg2mem = alloca i32
  %multmp92.reg2mem = alloca i32
  %multmp91.reg2mem = alloca i32
  %multmp113.reg2mem = alloca i32
  %multmp112.reg2mem = alloca i32
  %addtmp86.reg2mem = alloca i32
  %multmp78.reg2mem = alloca i32
  %multmp58.reg2mem = alloca i32
  %multmp45.reg2mem = alloca i32
  %addtmp70.reg2mem = alloca i32
  %multmp28.reg2mem = alloca i32
  %multmp27.reg2mem = alloca i32
  %addtmp19.reg2mem = alloca i32
  %addtmp1.reg2mem = alloca i32
  %addtmp.reg2mem = alloca i32
  %addtmp39.reg2mem = alloca i32
  %.reg2mem239 = alloca i32*
  %.reg2mem236 = alloca i32*
  %.reg2mem231 = alloca i32*
  %.reg2mem225 = alloca i32*
  %.reg2mem218 = alloca i32*
  %.reg2mem215 = alloca double*
  %.reg2mem212 = alloca i64*
  %.reg2mem = alloca i64*
  %multmp81.reg2mem = alloca i32
  %multmp80.reg2mem = alloca i32
  %multmp.reg2mem = alloca i32
  %multmp = mul i32 40, 40
  store i32 %multmp, i32* %multmp.reg2mem
  %multmp.reload195 = load volatile i32, i32* %multmp.reg2mem
  %greater = icmp sgt i32 40, %multmp.reload195
  %phi38.reg2mem = alloca i32
  %phi26.reg2mem = alloca i32
  %phi35.reg2mem = alloca i32
  %phi69.reg2mem = alloca i32
  %phi55.reg2mem = alloca i32
  %phi66.reg2mem = alloca i32
  %phi155.reg2mem = alloca i32
  %phi130.reg2mem = alloca i32
  br i1 %greater, label %trueblock, label %falseblock

; <label>:1:                                      ; preds = %newblock71, %defblock47, %defblock30, %defblock
  %multmp80 = mul i32 81, 81
  store i32 %multmp80, i32* %multmp80.reg2mem
  %multmp80.reload207 = load volatile i32, i32* %multmp80.reg2mem
  %multmp81 = mul i32 81, %multmp80.reload207
  store i32 %multmp81, i32* %multmp81.reg2mem
  %addtmp39.reload = load volatile i32, i32* %addtmp39.reg2mem
  %greater83 = icmp sgt i32 %addtmp39.reload, 91
  br i1 %greater83, label %2, label %falseblock85

; <label>:2:                                      ; preds = %1, %defblock158, %defblock133, %defblock88
  %3 = alloca i32, align 4
  %4 = alloca i64, align 8
  store i64* %4, i64** %.reg2mem
  %5 = alloca i64, align 8
  store i64* %5, i64** %.reg2mem212
  %6 = alloca double, align 8
  store double* %6, double** %.reg2mem215
  %7 = alloca i32, align 4
  store i32* %7, i32** %.reg2mem218
  %8 = alloca i32, align 4
  store i32* %8, i32** %.reg2mem225
  %9 = alloca i32, align 4
  store i32* %9, i32** %.reg2mem231
  %10 = alloca i32, align 4
  store i32* %10, i32** %.reg2mem236
  %11 = alloca i32, align 4
  store i32* %11, i32** %.reg2mem239
  store i32 0, i32* %3, align 4
  %12 = call i64 @clock() #3
  %.reload211 = load volatile i64*, i64** %.reg2mem
  store i64 %12, i64* %.reload211, align 8
  %.reload224 = load volatile i32*, i32** %.reg2mem218
  store i32 0, i32* %.reload224, align 4
  %.reload230 = load volatile i32*, i32** %.reg2mem225
  store i32 0, i32* %.reload230, align 4
  %.reload229 = load volatile i32*, i32** %.reg2mem225
  store i32 0, i32* %.reload229, align 4
  br label %13

; <label>:13:                                     ; preds = %31, %2
  %.reload228 = load volatile i32*, i32** %.reg2mem225
  %14 = load i32, i32* %.reload228, align 4
  %15 = icmp slt i32 %14, 100000
  br i1 %15, label %16, label %34

; <label>:16:                                     ; preds = %trueblock84, %13
  %.reload235 = load volatile i32*, i32** %.reg2mem231
  store i32 1000, i32* %.reload235, align 4
  %.reload238 = load volatile i32*, i32** %.reg2mem236
  store i32 3, i32* %.reload238, align 4
  %.reload241 = load volatile i32*, i32** %.reg2mem239
  store i32 4, i32* %.reload241, align 4
  %.reload237 = load volatile i32*, i32** %.reg2mem236
  %17 = load i32, i32* %.reload237, align 4
  %.reload240 = load volatile i32*, i32** %.reg2mem239
  %18 = load i32, i32* %.reload240, align 4
  %19 = mul nsw i32 %17, %18
  %.reload223 = load volatile i32*, i32** %.reg2mem218
  %20 = load i32, i32* %.reload223, align 4
  %21 = add nsw i32 %20, %19
  %.reload222 = load volatile i32*, i32** %.reg2mem218
  store i32 %21, i32* %.reload222, align 4
  br label %truecase89

; <label>:22:                                     ; preds = %truecase89, %25
  %.reload234 = load volatile i32*, i32** %.reg2mem231
  %23 = load i32, i32* %.reload234, align 4
  %24 = icmp sge i32 %23, 0
  br i1 %24, label %newblock87, label %30

; <label>:25:                                     ; preds = %newblock157
  %.reload233 = load volatile i32*, i32** %.reg2mem231
  %26 = load i32, i32* %.reload233, align 4
  %27 = add nsw i32 %26, -1
  %.reload232 = load volatile i32*, i32** %.reg2mem231
  store i32 %27, i32* %.reload232, align 4
  %.reload221 = load volatile i32*, i32** %.reg2mem218
  %28 = load i32, i32* %.reload221, align 4
  %29 = add nsw i32 %28, 1
  %.reload220 = load volatile i32*, i32** %.reg2mem218
  store i32 %29, i32* %.reload220, align 4
  br label %22

; <label>:30:                                     ; preds = %22
  br label %31

; <label>:31:                                     ; preds = %30
  %.reload227 = load volatile i32*, i32** %.reg2mem225
  %32 = load i32, i32* %.reload227, align 4
  %33 = add nsw i32 %32, 1
  %.reload226 = load volatile i32*, i32** %.reg2mem225
  store i32 %33, i32* %.reload226, align 4
  br label %13

; <label>:34:                                     ; preds = %13
  %.reload219 = load volatile i32*, i32** %.reg2mem218
  %35 = load i32, i32* %.reload219, align 4
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i32 %35)
  %37 = call i64 @clock() #3
  %.reload214 = load volatile i64*, i64** %.reg2mem212
  store i64 %37, i64* %.reload214, align 8
  %.reload213 = load volatile i64*, i64** %.reg2mem212
  %38 = load i64, i64* %.reload213, align 8
  %.reload = load volatile i64*, i64** %.reg2mem
  %39 = load i64, i64* %.reload, align 8
  %40 = sub nsw i64 %38, %39
  %41 = sitofp i64 %40 to double
  %42 = fdiv double %41, 1.000000e+06
  %.reload217 = load volatile double*, double** %.reg2mem215
  store double %42, double* %.reload217, align 8
  %.reload216 = load volatile double*, double** %.reg2mem215
  %43 = load double, double* %.reload216, align 8
  %44 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str.1, i32 0, i32 0), double %43)
  %45 = call i32 @system(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0))
  ret i32 0

newblock:                                         ; preds = %newblock29, %newblock2
  %phi38.reload = load i32, i32* %phi38.reg2mem
  store i32 %phi38.reload, i32* %phi38.reload.reg2mem
  %multmp.reload188 = load volatile i32, i32* %multmp.reg2mem
  %addtmp39 = add i32 %multmp.reload188, 45
  store i32 %addtmp39, i32* %addtmp39.reg2mem
  %multmp.reload187 = load volatile i32, i32* %multmp.reg2mem
  %equal41 = icmp eq i32 %multmp.reload187, 45
  br i1 %equal41, label %trueblock42, label %falseblock43

trueblock:                                        ; preds = %0
  %multmp.reload194 = load volatile i32, i32* %multmp.reg2mem
  %addtmp = add i32 40, %multmp.reload194
  store i32 %addtmp, i32* %addtmp.reg2mem
  %addtmp.reload247 = load volatile i32, i32* %addtmp.reg2mem
  %addtmp1 = add i32 45, %addtmp.reload247
  store i32 %addtmp1, i32* %addtmp1.reg2mem
  switch i32 45, label %defblock [
    i32 45, label %truecase
    i32 115, label %otherblock
    i32 186, label %otherblock3
    i32 258, label %otherblock4
    i32 331, label %otherblock5
    i32 405, label %otherblock6
    i32 480, label %otherblock7
    i32 556, label %otherblock8
  ]

falseblock:                                       ; preds = %0
  %addtmp19 = add i32 45, 87
  store i32 %addtmp19, i32* %addtmp19.reg2mem
  %multmp.reload190 = load volatile i32, i32* %multmp.reg2mem
  %equal = icmp eq i32 40, %multmp.reload190
  br i1 %equal, label %trueblock21, label %falseblock22

newblock2:                                        ; preds = %otherblock8, %otherblock7, %otherblock6, %otherblock5, %otherblock4, %otherblock3, %otherblock, %truecase
  %multmp.reload191 = load volatile i32, i32* %multmp.reg2mem
  %addtmp17 = add i32 40, %multmp.reload191
  %addtmp.reload = load volatile i32, i32* %addtmp.reg2mem
  %addtmp18 = add i32 45, %addtmp.reload
  store i32 %addtmp18, i32* %phi38.reg2mem
  br label %newblock

defblock:                                         ; preds = %trueblock
  br label %1

truecase:                                         ; preds = %trueblock
  %addtmp9 = add i32 79, 40
  br label %newblock2

otherblock:                                       ; preds = %trueblock
  %addtmp1.reload248 = load volatile i32, i32* %addtmp1.reg2mem
  %addtmp10 = add i32 59, %addtmp1.reload248
  br label %newblock2

otherblock3:                                      ; preds = %trueblock
  %addtmp.reload246 = load volatile i32, i32* %addtmp.reg2mem
  %addtmp11 = add i32 45, %addtmp.reload246
  br label %newblock2

otherblock4:                                      ; preds = %trueblock
  %multmp.reload193 = load volatile i32, i32* %multmp.reg2mem
  %addtmp12 = add i32 40, %multmp.reload193
  br label %newblock2

otherblock5:                                      ; preds = %trueblock
  %addtmp1.reload = load volatile i32, i32* %addtmp1.reg2mem
  %addtmp13 = add i32 %addtmp1.reload, 27
  br label %newblock2

otherblock6:                                      ; preds = %trueblock
  %addtmp.reload245 = load volatile i32, i32* %addtmp.reg2mem
  %addtmp14 = add i32 %addtmp.reload245, 59
  br label %newblock2

otherblock7:                                      ; preds = %trueblock
  %multmp.reload192 = load volatile i32, i32* %multmp.reg2mem
  %addtmp15 = add i32 %multmp.reload192, 45
  br label %newblock2

otherblock8:                                      ; preds = %trueblock
  %addtmp16 = add i32 63, 40
  br label %newblock2

newblock20:                                       ; preds = %falseblock22, %trueblock21
  %phi26.reload = load i32, i32* %phi26.reg2mem
  %multmp27 = mul i32 95, %phi26.reload
  store i32 %multmp27, i32* %multmp27.reg2mem
  %multmp28 = mul i32 45, 87
  store i32 %multmp28, i32* %multmp28.reg2mem
  switch i32 45, label %defblock30 [
    i32 45, label %truecase31
    i32 1, label %otherblock32
  ]

trueblock21:                                      ; preds = %falseblock
  %multmp.reload189 = load volatile i32, i32* %multmp.reg2mem
  %multmp23 = mul i32 40, %multmp.reload189
  %addtmp19.reload249 = load volatile i32, i32* %addtmp19.reg2mem
  %multmp24 = mul i32 %addtmp19.reload249, 95
  store i32 %multmp24, i32* %phi26.reg2mem
  br label %newblock20

falseblock22:                                     ; preds = %falseblock
  %addtmp19.reload = load volatile i32, i32* %addtmp19.reg2mem
  %multmp25 = mul i32 %addtmp19.reload, 95
  store i32 25, i32* %phi26.reg2mem
  br label %newblock20

newblock29:                                       ; preds = %otherblock32, %truecase31
  %phi35.reload = load i32, i32* %phi35.reg2mem
  %multmp27.reload = load volatile i32, i32* %multmp27.reg2mem
  %addtmp36 = add i32 %multmp27.reload, 33
  %multmp28.reload = load volatile i32, i32* %multmp28.reg2mem
  %addtmp37 = add i32 %multmp28.reload, %phi35.reload
  store i32 %addtmp37, i32* %phi38.reg2mem
  br label %newblock

defblock30:                                       ; preds = %newblock20
  br label %1

truecase31:                                       ; preds = %newblock20
  %multmp28.reload250 = load volatile i32, i32* %multmp28.reg2mem
  %addtmp33 = add i32 %multmp28.reload250, 47
  store i32 55, i32* %phi35.reg2mem
  br label %newblock29

otherblock32:                                     ; preds = %newblock20
  %addtmp34 = add i32 59, 40
  store i32 67, i32* %phi35.reg2mem
  br label %newblock29

newblock40:                                       ; preds = %newblock59, %newblock46
  %phi69.reload = load i32, i32* %phi69.reg2mem
  %addtmp70 = add i32 %phi69.reload, 27
  store i32 %addtmp70, i32* %addtmp70.reg2mem
  br i1 false, label %trueblock72, label %falseblock73

trueblock42:                                      ; preds = %newblock
  %multmp.reload186 = load volatile i32, i32* %multmp.reg2mem
  %multmp44 = mul i32 40, %multmp.reload186
  %phi38.reload.reload = load volatile i32, i32* %phi38.reload.reg2mem
  %multmp45 = mul i32 45, %phi38.reload.reload
  store i32 %multmp45, i32* %multmp45.reg2mem
  switch i32 83, label %defblock47 [
    i32 83, label %truecase48
    i32 67, label %otherblock49
    i32 50, label %otherblock50
    i32 32, label %otherblock51
  ]

falseblock43:                                     ; preds = %newblock
  %multmp.reload183 = load volatile i32, i32* %multmp.reg2mem
  %multmp58 = mul i32 40, %multmp.reload183
  store i32 %multmp58, i32* %multmp58.reg2mem
  %addtmp39.reload244 = load volatile i32, i32* %addtmp39.reg2mem
  %greater60 = icmp sgt i32 83, %addtmp39.reload244
  br i1 %greater60, label %trueblock61, label %falseblock62

newblock46:                                       ; preds = %otherblock51, %otherblock50, %otherblock49, %truecase48
  %phi55.reload = load i32, i32* %phi55.reg2mem
  %multmp45.reload251 = load volatile i32, i32* %multmp45.reg2mem
  %multmp56 = mul i32 %multmp45.reload251, %phi55.reload
  %multmp45.reload = load volatile i32, i32* %multmp45.reg2mem
  %multmp57 = mul i32 5, %multmp45.reload
  store i32 %multmp57, i32* %phi69.reg2mem
  br label %newblock40

defblock47:                                       ; preds = %trueblock42
  br label %1

truecase48:                                       ; preds = %trueblock42
  store i32 21, i32* %phi55.reg2mem
  br label %newblock46

otherblock49:                                     ; preds = %trueblock42
  %multmp52 = mul i32 25, 40
  store i32 33, i32* %phi55.reg2mem
  br label %newblock46

otherblock50:                                     ; preds = %trueblock42
  %multmp.reload185 = load volatile i32, i32* %multmp.reg2mem
  %multmp53 = mul i32 40, %multmp.reload185
  store i32 45, i32* %phi55.reg2mem
  br label %newblock46

otherblock51:                                     ; preds = %trueblock42
  %multmp.reload184 = load volatile i32, i32* %multmp.reg2mem
  %multmp54 = mul i32 %multmp.reload184, 45
  store i32 57, i32* %phi55.reg2mem
  br label %newblock46

newblock59:                                       ; preds = %falseblock62, %trueblock61
  %phi66.reload = load i32, i32* %phi66.reg2mem
  %multmp58.reload = load volatile i32, i32* %multmp58.reg2mem
  %addtmp67 = add i32 %multmp58.reload, 81
  %addtmp68 = add i32 %phi66.reload, %addtmp67
  store i32 %addtmp68, i32* %phi69.reg2mem
  br label %newblock40

trueblock61:                                      ; preds = %falseblock43
  %multmp.reload182 = load volatile i32, i32* %multmp.reg2mem
  %addtmp63 = add i32 40, %multmp.reload182
  %phi38.reload.reload259 = load volatile i32, i32* %phi38.reload.reg2mem
  %addtmp64 = add i32 45, %phi38.reload.reload259
  store i32 %addtmp64, i32* %phi66.reg2mem
  br label %newblock59

falseblock62:                                     ; preds = %falseblock43
  %multmp.reload181 = load volatile i32, i32* %multmp.reg2mem
  %addtmp65 = add i32 40, %multmp.reload181
  store i32 11, i32* %phi66.reg2mem
  br label %newblock59

newblock71:                                       ; preds = %falseblock73, %trueblock72
  %multmp.reload = load volatile i32, i32* %multmp.reg2mem
  %multmp78 = mul i32 40, %multmp.reload
  store i32 %multmp78, i32* %multmp78.reg2mem
  %addtmp70.reload = load volatile i32, i32* %addtmp70.reg2mem
  %multmp79 = mul i32 27, %addtmp70.reload
  br label %1

trueblock72:                                      ; preds = %newblock40
  %addtmp39.reload243 = load volatile i32, i32* %addtmp39.reg2mem
  %multmp74 = mul i32 %addtmp39.reload243, 91
  %multmp.reload180 = load volatile i32, i32* %multmp.reg2mem
  %multmp75 = mul i32 %multmp.reload180, 45
  br label %newblock71

falseblock73:                                     ; preds = %newblock40
  %addtmp39.reload242 = load volatile i32, i32* %addtmp39.reg2mem
  %multmp76 = mul i32 83, %addtmp39.reload242
  br label %newblock71

newblock82:                                       ; preds = %newblock132, %newblock87
  %phi155.reload = load i32, i32* %phi155.reg2mem
  store i32 %phi155.reload, i32* %phi155.reload.reg2mem
  %phi155.reload.reload = load volatile i32, i32* %phi155.reload.reg2mem
  %addtmp156 = add i32 %phi155.reload.reload, 55
  %multmp78.reload = load volatile i32, i32* %multmp78.reg2mem
  switch i32 %multmp78.reload, label %defblock158 [
    i32 64000, label %truecase159
    i32 63931, label %otherblock160
    i32 63861, label %otherblock161
    i32 63790, label %otherblock162
    i32 63718, label %otherblock163
    i32 63645, label %otherblock164
    i32 63571, label %otherblock165
  ]

trueblock84:                                      ; No predecessors!
  %multmp80.reload206 = load volatile i32, i32* %multmp80.reg2mem
  %addtmp86 = add i32 %multmp80.reload206, 89
  store i32 %addtmp86, i32* %addtmp86.reg2mem
  switch i32 45, label %defblock88 [
    i32 45, label %16
    i32 31, label %otherblock90
  ]

falseblock85:                                     ; preds = %1
  %multmp80.reload202 = load volatile i32, i32* %multmp80.reg2mem
  %multmp112 = mul i32 81, %multmp80.reload202
  store i32 %multmp112, i32* %multmp112.reg2mem
  %multmp80.reload201 = load volatile i32, i32* %multmp80.reg2mem
  %multmp113 = mul i32 81, %multmp80.reload201
  store i32 %multmp113, i32* %multmp113.reg2mem
  %multmp80.reload200 = load volatile i32, i32* %multmp80.reg2mem
  %greater115 = icmp sgt i32 81, %multmp80.reload200
  br i1 %greater115, label %trueblock116, label %falseblock117

newblock87:                                       ; preds = %22, %newblock102
  %multmp80.reload203 = load volatile i32, i32* %multmp80.reg2mem
  %multmp111 = mul i32 %multmp80.reload203, 89
  store i32 %multmp111, i32* %phi155.reg2mem
  br label %newblock82

defblock88:                                       ; preds = %trueblock84
  br label %2

truecase89:                                       ; preds = %16
  br label %22

otherblock90:                                     ; preds = %trueblock84
  %multmp80.reload205 = load volatile i32, i32* %multmp80.reg2mem
  %multmp91 = mul i32 81, %multmp80.reload205
  store i32 %multmp91, i32* %multmp91.reg2mem
  %multmp80.reload204 = load volatile i32, i32* %multmp80.reg2mem
  %multmp92 = mul i32 %multmp80.reload204, 89
  store i32 %multmp92, i32* %multmp92.reg2mem
  %multmp81.reload210 = load volatile i32, i32* %multmp81.reg2mem
  %greater94 = icmp sgt i32 89, %multmp81.reload210
  br i1 %greater94, label %trueblock95, label %falseblock96

newblock93:                                       ; preds = %falseblock96, %trueblock95
  %multmp91.reload256 = load volatile i32, i32* %multmp91.reg2mem
  %addtmp101 = add i32 21, %multmp91.reload256
  %multmp91.reload255 = load volatile i32, i32* %multmp91.reg2mem
  %uequal = icmp ne i32 %multmp91.reload255, 29
  br i1 %uequal, label %trueblock103, label %falseblock104

trueblock95:                                      ; preds = %otherblock90
  %multmp92.reload258 = load volatile i32, i32* %multmp92.reg2mem
  %addtmp97 = add i32 29, %multmp92.reload258
  br label %newblock93

falseblock96:                                     ; preds = %otherblock90
  %multmp81.reload209 = load volatile i32, i32* %multmp81.reg2mem
  %addtmp98 = add i32 89, %multmp81.reload209
  %addtmp99 = add i32 37, 51
  br label %newblock93

newblock102:                                      ; preds = %falseblock104, %trueblock103
  %multmp92.reload = load volatile i32, i32* %multmp92.reg2mem
  %multmp109 = mul i32 29, %multmp92.reload
  br label %newblock87

trueblock103:                                     ; preds = %newblock93
  %addtmp86.reload = load volatile i32, i32* %addtmp86.reg2mem
  %multmp105 = mul i32 97, %addtmp86.reload
  %multmp92.reload257 = load volatile i32, i32* %multmp92.reg2mem
  %multmp106 = mul i32 29, %multmp92.reload257
  br label %newblock102

falseblock104:                                    ; preds = %newblock93
  %multmp91.reload = load volatile i32, i32* %multmp91.reg2mem
  %multmp107 = mul i32 21, %multmp91.reload
  br label %newblock102

newblock114:                                      ; preds = %newblock121, %trueblock116
  %phi130.reload = load i32, i32* %phi130.reg2mem
  store i32 %phi130.reload, i32* %phi130.reload.reg2mem
  %multmp80.reload198 = load volatile i32, i32* %multmp80.reg2mem
  %multmp131 = mul i32 81, %multmp80.reload198
  switch i32 5, label %defblock133 [
    i32 5, label %truecase134
    i32 24, label %otherblock135
    i32 44, label %otherblock136
    i32 65, label %otherblock137
    i32 87, label %otherblock138
    i32 110, label %otherblock139
    i32 134, label %otherblock140
  ]

trueblock116:                                     ; preds = %falseblock85
  %multmp112.reload253 = load volatile i32, i32* %multmp112.reg2mem
  %addtmp118 = add i32 %multmp112.reload253, 29
  store i32 %addtmp118, i32* %phi130.reg2mem
  br label %newblock114

falseblock117:                                    ; preds = %falseblock85
  %multmp112.reload252 = load volatile i32, i32* %multmp112.reg2mem
  %addtmp119 = add i32 %multmp112.reload252, 29
  %multmp81.reload208 = load volatile i32, i32* %multmp81.reg2mem
  %addtmp120 = add i32 %multmp81.reload208, 97
  %equal122 = icmp eq i32 51, %addtmp119
  br i1 %equal122, label %trueblock123, label %falseblock124

newblock121:                                      ; preds = %falseblock124, %trueblock123
  %multmp129 = mul i32 97, 21
  store i32 %multmp129, i32* %phi130.reg2mem
  br label %newblock114

trueblock123:                                     ; preds = %falseblock117
  %multmp80.reload199 = load volatile i32, i32* %multmp80.reg2mem
  %multmp125 = mul i32 %multmp80.reload199, 89
  br label %newblock121

falseblock124:                                    ; preds = %falseblock117
  %multmp126 = mul i32 97, 21
  %multmp113.reload254 = load volatile i32, i32* %multmp113.reg2mem
  %multmp127 = mul i32 %multmp113.reload254, 37
  br label %newblock121

newblock132:                                      ; preds = %otherblock140, %otherblock139, %otherblock138, %otherblock137, %otherblock136, %otherblock135, %truecase134
  %phi130.reload.reload270 = load volatile i32, i32* %phi130.reload.reg2mem
  %addtmp154 = add i32 %phi130.reload.reload270, 5
  store i32 %addtmp154, i32* %phi155.reg2mem
  br label %newblock82

defblock133:                                      ; preds = %newblock114
  br label %2

truecase134:                                      ; preds = %newblock114
  br label %newblock132

otherblock135:                                    ; preds = %newblock114
  %addtmp141 = add i32 31, 81
  %multmp113.reload = load volatile i32, i32* %multmp113.reg2mem
  %addtmp142 = add i32 29, %multmp113.reload
  br label %newblock132

otherblock136:                                    ; preds = %newblock114
  %addtmp143 = add i32 97, 21
  %phi130.reload.reload = load volatile i32, i32* %phi130.reload.reg2mem
  %addtmp144 = add i32 %phi130.reload.reload, 5
  br label %newblock132

otherblock137:                                    ; preds = %newblock114
  %phi130.reload.reload268 = load volatile i32, i32* %phi130.reload.reg2mem
  %addtmp145 = add i32 37, %phi130.reload.reload268
  %addtmp146 = add i32 13, 71
  br label %newblock132

otherblock138:                                    ; preds = %newblock114
  %addtmp147 = add i32 91, 81
  %addtmp148 = add i32 99, 81
  br label %newblock132

otherblock139:                                    ; preds = %newblock114
  %addtmp149 = add i32 97, 21
  %multmp81.reload = load volatile i32, i32* %multmp81.reg2mem
  %addtmp150 = add i32 89, %multmp81.reload
  br label %newblock132

otherblock140:                                    ; preds = %newblock114
  %phi130.reload.reload269 = load volatile i32, i32* %phi130.reload.reg2mem
  %addtmp151 = add i32 37, %phi130.reload.reload269
  %multmp112.reload = load volatile i32, i32* %multmp112.reg2mem
  %addtmp152 = add i32 21, %multmp112.reload
  br label %newblock132

newblock157:                                      ; preds = %otherblock165, %otherblock164, %otherblock163, %otherblock162, %otherblock161, %otherblock160, %truecase159
  %multmp80.reload = load volatile i32, i32* %multmp80.reg2mem
  %multmp179 = mul i32 81, %multmp80.reload
  br label %25

defblock158:                                      ; preds = %newblock82
  br label %2

truecase159:                                      ; preds = %newblock82
  br label %newblock157

otherblock160:                                    ; preds = %newblock82
  %phi155.reload.reload260 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp166 = mul i32 97, %phi155.reload.reload260
  %phi155.reload.reload261 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp167 = mul i32 97, %phi155.reload.reload261
  br label %newblock157

otherblock161:                                    ; preds = %newblock82
  %phi155.reload.reload262 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp168 = mul i32 97, %phi155.reload.reload262
  %multmp80.reload197 = load volatile i32, i32* %multmp80.reg2mem
  %multmp169 = mul i32 81, %multmp80.reload197
  br label %newblock157

otherblock162:                                    ; preds = %newblock82
  %phi155.reload.reload263 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp170 = mul i32 97, %phi155.reload.reload263
  %multmp171 = mul i32 63, 21
  br label %newblock157

otherblock163:                                    ; preds = %newblock82
  %phi155.reload.reload264 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp172 = mul i32 97, %phi155.reload.reload264
  %phi155.reload.reload265 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp173 = mul i32 97, %phi155.reload.reload265
  br label %newblock157

otherblock164:                                    ; preds = %newblock82
  %phi155.reload.reload266 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp174 = mul i32 97, %phi155.reload.reload266
  %multmp80.reload196 = load volatile i32, i32* %multmp80.reg2mem
  %multmp175 = mul i32 81, %multmp80.reload196
  br label %newblock157

otherblock165:                                    ; preds = %newblock82
  %phi155.reload.reload267 = load volatile i32, i32* %phi155.reload.reg2mem
  %multmp176 = mul i32 97, %phi155.reload.reload267
  %multmp177 = mul i32 63, 81
  br label %newblock157
}

; Function Attrs: nounwind
declare i64 @clock() #1

declare i32 @printf(i8*, ...) #2

declare i32 @system(i8*) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (tags/RELEASE_501/final)"}
