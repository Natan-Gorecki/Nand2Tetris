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

    @current_color // (white == 0, black == 1)
    M=0

(MAIN_LOOP)
    @KBD
    D=M
    @current_color
    D=D-M
    @SET_COLOR
    D;JNE
    @MAIN_LOOP
    0;JMP

(SET_COLOR)
    // set current_color
    @KBD
    D=M
    @WHITE_COLOR
    D;JEQ
    @BLACK_COLOR
    0;JMP

(WHITE_COLOR)
    @current_color
    M=0

    @FILL_SCREEN
    0;JMP

(BLACK_COLOR)
    @current_color
    M=-1;
    
    @FILL_SCREEN
    0;JMP

(FILL_SCREEN)
    @SCREEN
    D=A
    @pixels
    M=A

(FILL_SCREEN_LOOP)
    // if(pixels == SCREEN + 256*32) goto LOOP
    @KBD
    D=A
    @pixels
    D=D-M
    @MAIN_LOOP
    D;JEQ

    //paint 16-pixels block
    @current_color
    D=M
    @pixels
    A=M
    M=D
    
    //increment pixels
    D=A+1
    @pixels
    M=D

    @FILL_SCREEN_LOOP
    0;JMP