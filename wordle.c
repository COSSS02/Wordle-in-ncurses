#include "wordle.h"

void declare_colors()
{
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_GREEN);
    init_pair(5, COLOR_BLACK, COLOR_GREEN);
    init_pair(6, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(7, COLOR_BLACK, COLOR_YELLOW);
    init_pair(8, COLOR_BLACK, COLOR_RED);
    init_pair(9, COLOR_GREEN, COLOR_RED);
    init_pair(10, COLOR_GREEN, COLOR_BLACK);
}

void choose_word(char *word)
{
    const char list[MAX_WORDS][6] = {"abuse", "adult", "agent", "anger",
        "beach", "birth", "block", "blood", "board", "brain", "bread",
        "break", "brown", "buyer", "cause", "chain", "chair", "chest",
        "chief", "child", "china", "claim", "class", "clock", "coach",
        "coast", "court", "cover", "cream", "crime", "cross", "crowd",
        "crown", "cycle", "dance", "death", "depth", "doubt", "draft",
        "drama", "dream", "dress", "drink", "drive", "earth", "enemy",
        "entry", "error", "event", "faith", "fault", "field", "fight",
        "final", "floor", "focus", "force", "frame", "frank", "front",
        "fruit", "glass", "grant", "grass", "green", "group", "guide",
        "heart", "henry", "horse", "hotel", "house", "image", "index",
        "input", "issue", "japan", "jones", "judge", "knife", "laura",
        "layer", "level", "lewis", "light", "limit", "lunch", "major",
        "march", "match", "metal", "model", "money", "month", "motor",
        "mouth", "music", "night", "noise", "north", "novel", "nurse",
        "offer", "order", "other", "owner", "panel", "paper", "party",
        "peace", "peter", "phase", "phone", "piece", "pilot", "pitch",
        "place", "plane", "plant", "plate", "point", "pound", "power",
        "press", "price", "pride", "prize", "proof", "queen", "radio",
        "range", "ratio", "reply", "right", "river", "round", "route",
        "rugby", "scale", "scene", "scope", "score", "sense", "shape",
        "share", "sheep", "sheet", "shift", "shirt", "shock", "sight",
        "simon", "skill", "sleep", "smile", "smith", "smoke", "sound",
        "south", "space", "speed", "spite", "sport", "squad", "staff",
        "stage", "start", "state", "steam", "steel", "stock", "stone",
        "store", "study", "stuff", "style", "sugar", "table", "taste",
        "terry", "theme", "thing", "title", "total", "touch", "tower",
        "track", "trade", "train", "trend", "trial", "trust", "truth",
        "uncle", "union", "unity", "value", "video", "visit", "voice",
        "waste", "watch", "water", "while", "white", "whole", "woman",
        "world", "youth"};
    // chooses word randomly using time
    strcpy(word, list[time(NULL) % MAX_WORDS]);
}

// prints game title and information about the main menu
void initial_text()
{
    int center = COLS / 2 - 3;
    // background of the entire window
    bkgd(COLOR_PAIR(1));

    attron(COLOR_PAIR(1) | A_BOLD);
    mvaddstr(1, center, "WORDLE");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvaddstr(LINES - 3, center - 9, "PRESS : TO OPEN THE MAIN MENU");
    attroff(COLOR_PAIR(3) | A_BOLD);
}

// creates a window at the (x,y) position
WINDOW *boardgame(int y, int x)
{
    int yaxis = 4, xaxis = COLS / 2 - 13;

    WINDOW *win = newwin(3, 5, yaxis + y, xaxis + x);
    // box draws the border of the new window
    box(win, 0, 0);

    wrefresh(win);
    return win;
}

void print_letter(char ch, int y, int x)
{
    int yaxis = 5, xaxis = COLS / 2 - 11;
    attron(COLOR_PAIR(3));
    mvaddch(yaxis + y, xaxis + x, ch);
    attroff(COLOR_PAIR(3));
    refresh();
}

/* prints a warning when the player presses ENTER without introducing
5 letters or when the player introduces more than 5 letters*/
void warning1()
{
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddstr(LINES - 5, COLS / 2 - 12, "WRITE 5 LETTERS AND PRESS ENTER");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
}

// warning for wrong input
void warning2()
{
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddstr(LINES - 4, COLS / 2 - 12, "ONLY SMALL LETTERS AND BACKSPACE");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
}

// colors a window green (nr = 4) or yellow (nr = 6)
void color_box(WINDOW *win, char ch, int nr)
{
    wbkgd(win, COLOR_PAIR(nr));

    // reprints the letter since its covered when coloring the window
    wattron(win, COLOR_PAIR(nr + 1));
    wmove(win, 1, 2);
    waddch(win, ch);
    wattroff(win, COLOR_PAIR(nr + 1));

    wrefresh(win);
}

// checks which windows should be colored and returns 1 if all are green
int coloring(WINDOW **win, char *word, int yaxis)
{
    int i, j;
    int xaxis = COLS / 2 - 11;
    // number of green windows
    int green = 0;
    // used to check which windows where verified
    int checked[5] = {0};

    for (i = 0; i <= 4; i++) {
        // if the letter isn't verified
        if (!checked[i]) {
            // extracts the letter at the i position
            char character = mvinch(yaxis, xaxis + i * 5);

            // number of occurences of character in the winner word
            int counter = 0;
            for (j = 0; j <= 4; j++) {
                if (word[j] == character) {
                    counter++;
                }
            }

            for (j = i; j <= 4; j++) {
                // searches for letters equal to character
                char temp = mvinch(yaxis, xaxis + j * 5);
                if (temp == character) {
                    // if there are uncolored windows containing character
                    if (counter) {
                        // if the letter is in the correct window
                        if (word[j] == temp) {

                            // colors the window green
                            color_box(win[j], temp, 4);
                            checked[j] = 1;

                            counter--;
                            green++;
                        }
                    }
                }
            }

            // checks again every window to see if they should be yellow
            for (j = i; j <= 4; j++) {
                char temp = mvinch(yaxis, xaxis + j * 5);
                if (temp == character) {
                    if (counter && !checked[j]) {
                        if (strchr(word, temp)) {
                            color_box(win[j], temp, 6);
                            counter--;
                        }
                    }
                    checked[j] = 1;
                }
            }
        }
    }

    if (green == 5) {
        return 1;
    }
    return 0;
}

void erase_gameboard(WINDOW *board[6][5])
{
    // deletes informations and warnings on the screen
    mvaddstr(LINES - 3, COLS / 2 - 12, "                             ");
    mvaddstr(LINES - 5, COLS / 2 - 12, "                               ");
    mvaddstr(LINES - 4, COLS / 2 - 12, "                                ");
    curs_set(0);

    int i, j;
    for (i = 0; i <= 5; i++) {
        for (j = 0; j <= 4; j++) {
            wbkgd(board[i][j], COLOR_PAIR(1));
            wclear(board[i][j]);
            wrefresh(board[i][j]);
        }
    }
}

// creates a winner window with the option to play again
WINDOW *winner_message()
{
    WINDOW *win = newwin(10, 30, LINES / 2 - 7, COLS / 2 - 15);
    wbkgd(win, COLOR_PAIR(5));
    wmove(win, 1, 10);
    waddstr(win, "YOU WON!");
    wmove(win, 3, 9);
    waddstr(win, "PLAY AGAIN?");
    wmove(win, 6, 8);
    waddstr(win, "Y           N");
    wrefresh(win);
    return win;
}

// creates a loser window which displays the correct word
WINDOW *loser_message(char *word)
{
    WINDOW *win = newwin(10, 30, LINES / 2 - 7, COLS / 2 - 15);
    wbkgd(win, COLOR_PAIR(8));
    wmove(win, 1, 10);
    waddstr(win, "YOU LOST!");
    wmove(win, 3, 6);
    waddstr(win, "THE WORD WAS:");
    wmove(win, 3, 19);
    wattron(win, COLOR_PAIR(9) | A_BOLD);
    waddstr(win, word);
    wattroff(win, COLOR_PAIR(9) | A_BOLD);
    wmove(win, 6, 4);
    waddstr(win, "PRESS ANY KEY TO EXIT");
    wrefresh(win);
    return win;
}

// creates the main menu window with the options to restart or play again
WINDOW *main_menu()
{
    WINDOW *win = newwin(10, 30, LINES / 2 - 7, COLS / 2 - 15);
    wbkgd(win, COLOR_PAIR(10));
    wmove(win, 1, 10);
    waddstr(win, "MAIN MENU");
    wmove(win, 4, 7);
    waddstr(win, "RESTART? PRESS R");
    wmove(win, 7, 9);
    waddstr(win, "EXIT? PRESS E");
    wrefresh(win);
    return win;
}