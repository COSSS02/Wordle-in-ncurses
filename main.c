#include "wordle.h"

int main()
{
    int play_again = 1;
    initscr();
    noecho();
    start_color();
    declare_colors();

    while (play_again) {
        int i, j;
        int winner = 0;
        int character, read;

        // used to move the cursor in the middle of the windows
        int yaxis = 5, xaxis = COLS / 2 - 11;

        char word[6], ch;
        choose_word(word);
        initial_text();
        refresh();

        // generates the game board
        WINDOW *board[6][5];
        for (i = 0; i <= 5; i++) {
            for (j = 0; j <= 4; j++) {
                board[i][j] = boardgame(i * 3, j * 5);
            }
        }

        // number of words introduced by the player
        i = 0;

        while (i < 6 && !winner) {
            // checks if the player has introduced a word
            int check = 0;

            // the window where the cursor is placed
            j = 0;

            while (!check) {
                // moves the cursor in the corresponding window
                if (j < 5) {
                    move(yaxis + i * 3, xaxis + j * 5);
                } else {
                    move(yaxis + i * 3, xaxis + (j - 1) * 5);
                }

                // reads one character
                character = getch();
                ch = (char) character;

                if (ch >= 'a' && ch <= 'z') {
                    // if the player didn't write 5 letters
                    if (j < 5) {
                        print_letter(ch, i * 3, j * 5);
                    } else {
                        warning1();
                        j--;
                    }

                } else if (character == BACKSPACE) {
                    // if the cursor is not in the first window
                    if (j > 0) {
                        j--;
                        // prints ' ' to delete the character
                        print_letter(' ', i * 3, j * 5);
                    }
                    j--;

                } else if (character == ENTER) {
                    // if 5 letters weren't introduced
                    if (j < 5) {
                        warning1();
                        j--;
                    } else {
                        // winner remembers if the word introduced is correct
                        winner = coloring(board[i], word, i * 3 + 5);
                        check = 1;
                    }

                } else if (ch == ':' || ch == ';') {
                    read = 0;
                    erase_gameboard(board);

                    WINDOW *menu = main_menu();

                    // while the player is not pressing R or E
                    while (!read) {
                        character = getch();
                        ch = (char) character;

                        if (ch == 'R' || ch == 'r') {
                            read = 1;
                        } else if (ch == 'E' || ch == 'e') {
                            play_again = 0;
                            read = 1;
                        }
                    }
                    check = 1;
                    i = MAIN_MENU - 1;

                    // deletes the main menu window
                    wbkgd(menu, COLOR_PAIR(1));
                    wclear(menu);
                    wrefresh(menu);
                    curs_set(1);

                } else {
                    warning2();
                    j--;
                }
                j++;
            }
            i++;
        }

        if (winner) {
            read = 0;
            erase_gameboard(board);

            WINDOW *winner = winner_message();

            // while the player is not typing Y or N
            while (!read) {
                character = getch();
                ch = (char) character;

                if (ch == 'Y' || ch == 'y') {
                    read = 1;
                } else if (ch == 'N' || ch == 'n') {
                    play_again = 0;
                    read = 1;
                }
            }

            // deletes the winner window
            wbkgd(winner, COLOR_PAIR(1));
            wclear(winner);
            wrefresh(winner);
            curs_set(1);

            // if the player lost
        } else if (i != MAIN_MENU) {
            erase_gameboard(board);

            WINDOW *loser = loser_message(word);
            play_again = 0;
            getch();

            // deletes the loser window
            wbkgd(loser, COLOR_PAIR(1));
            wclear(loser);
            wrefresh(loser);
        }
    }

    endwin();
    return 0;
}