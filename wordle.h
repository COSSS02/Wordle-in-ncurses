#ifndef WORDLE_H
#define WORDLE_H

#include <ncurses.h>
#include <time.h>
#include <string.h>
#define MAX_WORDS 209
#define ENTER 10
#define BACKSPACE 127
#define MAIN_MENU 9

void declare_colors();

void choose_word(char *cuvant);

void initial_text();

WINDOW *boardgame(int y, int x);

void print_letter(char ch, int y, int x);

void warning1();

void warning2();

void color_box(WINDOW *cutie, char ch, int nr);

int coloring(WINDOW **cutie, char *cuvant, int yaxis);

void erase_gameboard(WINDOW *cutii[6][5]);

WINDOW *winner_message();

WINDOW *loser_message(char *cuvant);

WINDOW *main_menu();

#endif
