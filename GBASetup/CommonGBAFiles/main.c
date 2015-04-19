/* 
 * File:   main.c
 * Author: dsmith
 *
 * Created on September 7, 2010, 11:54 AM
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned short u16;

#ifdef _DEBUG
#define ROWS 20
#define COLS 80
#define SIZE ((COLS+1)*ROWS + 1)
#define OFFSET(r, c) ((r) * (COLS+1) + (c))
char screen[SIZE];
#else
#define ROWS 160
#define COLS 240
#define REG_DISPCTL *(u16 *)0x4000000
#define VIDEO_BUFFER ((u16*)0x6000000)
#define OFFSET(R,C) ((R)*COLS+(C))
/* Create a color with the specified RGB values */
#define COLOR(r, g, b) ((r) | (g)<<5 | (b)<<10)

#endif

#ifdef _DEBUG
#endif
/*
 * main program with simple actions
 * in both DEBUG and RELEASE modes
 */
int main() {

#ifdef _DEBUG
    printf("You have successfully run your project in DEBUG mode.\n");
#else
    REG_DISPCTL = 1027;
    VIDEO_BUFFER[OFFSET(ROWS/2, COLS/2)] = COLOR(31, 16, 16);
    while(1)
    {

    }
#endif
    return (EXIT_SUCCESS);
}

