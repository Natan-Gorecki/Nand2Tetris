(START)
@256
D=A
@SP
M=D

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@AFTER_CONDITION.1
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JEQ
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.1)

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@AFTER_CONDITION.2
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JEQ
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.2)

// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1

// eq
@AFTER_CONDITION.3
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JEQ
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.3)

// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@AFTER_CONDITION.4
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JLT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.4)

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@AFTER_CONDITION.5
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JLT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.5)

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1

// lt
@AFTER_CONDITION.6
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JLT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.6)

// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@AFTER_CONDITION.7
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JGT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.7)

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@AFTER_CONDITION.8
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JGT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.8)

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1

// gt
@AFTER_CONDITION.9
D=A
@R13
M=D
@SP
AM=M-1
D=M
@SP
AM=M-1
D=M-D
@TRUE_CONDITION
D;JGT
@FALSE_CONDITION
0;JMP
(AFTER_CONDITION.9)

// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1

// push constant 53
@53
D=A
@SP
A=M
M=D
@SP
M=M+1

// add
@SP
AM=M-1
D=M
A=A-1
M=M+D

// push constant 112
@112
D=A
@SP
A=M
M=D
@SP
M=M+1

// sub
@SP
AM=M-1
D=M
A=A-1
M=M-D

// neg
@SP
A=M-1
M=-M

// and
@SP
AM=M-1
D=M
A=A-1
M=M&D

// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1

// or
@SP
AM=M-1
D=M
A=A-1
M=M|D

// not
@SP
A=M-1
M=!M

(END)
@END
0;JMP

(TRUE_CONDITION)
@1
D=-A
@SP
A=M
M=D
@SP
M=M+1
@R13
A=M
0;JMP

(FALSE_CONDITION)
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
@R13
A=M
0;JMP
