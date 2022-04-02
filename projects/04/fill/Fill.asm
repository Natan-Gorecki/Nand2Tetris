// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

    @white_screen
    M=1
    @black_screen
    M=0
    @last_color // (white == 0, black == 1)
    M=0
    @in_progress
    M=0
    @pixels
    M=0
(LOOP)
    @KBD
    D=M
    @BLACK
    D;JGT
    @WHITE
    0;JMP

(WHITE)
    //if(white_screen == 1) goto LOOP
    @white_screen
    D=M
    @LOOP
    D;JGT

    // if(last_color > 0)
    @last_color
    D=M
    @WHITE_START
    D;JGT

    // if(in_progress > 0) goto WHITE_CONTINUE; else goto WHITE_START
    @in_progress
    D=M
    @WHITE_CONTINUE
    D;JGT
    @WHITE_START
    0;JMP

(WHITE_START)
    //initialize variables
    @white_screen
    M=0
    @black_screen
    M=0
    @last_color
    M=0
    @in_progress
    M=1
    @SCREEN
    D=A
    @pixels
    AM=D

    //paint first 16-pixels block
    M=0

    //increment pixels
    D=A+1
    @pixels
    M=D
    
    @LOOP
    0;JMP

(WHITE_CONTINUE)
    //initialize variables
    @last_color
    M=0

    // if(pixels == SCREEN + 256*32) goto WHITE_END
    @KBD
    D=A
    @pixels
    D=D-M
    @WHITE_END
    D;JEQ

    // paint next 16-pixels block
    @pixels
    A=M
    M=0

    //increment pixels
    D=A+1
    @pixels
    M=D

    @LOOP
    0;JMP

(WHITE_END)
    @white_screen
    M=1
    @in_progress
    M=0
    @LOOP
    0;JMP

(BLACK)
    //if(black_screen == 1) goto LOOP
    @black_screen
    D=M
    @LOOP
    D;JGT

    // if(last_color == 0)
    @last_color
    D=M
    @BLACK_START
    D;JEQ

    // if(in_progress > 0) goto BLACK_CONTINUE; else goto BLACK_START
    @in_progress
    D=M
    @BLACK_CONTINUE
    D;JGT
    @BLACK_START
    0;JMP

(BLACK_START)
    //initialize variables
    @white_screen
    M=0
    @black_screen
    M=0
    @last_color
    M=1
    @in_progress
    M=1
    @SCREEN
    D=A
    @pixels
    AM=D

    //paint first 16-pixels block
    M=-1

    //increment pixels
    D=A+1
    @pixels
    M=D
    
    @LOOP
    0;JMP

(BLACK_CONTINUE)
    //initialize variables
    @last_color
    M=1

    // if(pixels == SCREEN + 256*32) goto BLACK_END
    @KBD
    D=A
    @pixels
    D=D-M
    @BLACK_END
    D;JEQ

    // paint next 16-pixels block
    @pixels
    A=M
    M=-1

    //increment pixels
    D=A+1
    @pixels
    M=D

    @LOOP
    0;JMP

(BLACK_END)
    @black_screen
    M=1
    @in_progress
    M=0
    @LOOP
    0;JMP