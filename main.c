#include "wordle.h"

int main()
{
    int play_again = 1;
    // initializarea ecranului
    initscr();
    noecho();
    start_color();
    // declararea perechilor de culori
    declare_colors();

    while (play_again) {
        int i, j;
        int winner = 0;
        int character, read;
        // folosite pentru a pozitiona cursorul in mijlocul blocurilor
        int yaxis = 5, xaxis = COLS / 2 - 11;

        char word[6], ch;
        choose_word(word);
        initial_text();
        refresh();

        // generarea blocurilor din tabla de joc
        WINDOW *board[6][5];
        for (i = 0; i <= 5; i++) {
            for (j = 0; j <= 4; j++) {
                board[i][j] = boardgame(i * 3, j * 5);
            }
        }

        // retine numarul de incercari
        i = 0;

        while (i < 6 && !winner) {
            // retine daca cuvantul a fost introdus
            int check = 0;

            // memoreaza pozitia blocului unde trebuie sa se afle cursorul
            j = 0;

            while (!check) {
                // muta cursorul in blocul potrivit
                if (j < 5) {
                    move(yaxis + i * 3, xaxis + j * 5);
                } else {
                    move(yaxis + i * 3, xaxis + (j - 1) * 5);
                }

                // citeste characterul de la tastatura
                character = getch();
                ch = (char) character;

                if (ch >= 'a' && ch <= 'z') {
                    // daca nu au fost introduse 5 litere
                    if (j < 5) {
                        print_letter(ch, i * 3, j * 5);
                    } else {
                        warning1();
                        j--;
                    }

                } else if (character == BACKSPACE) {
                    // daca cursorul nu se afla in blocul de pe prima pozitie
                    if (j > 0) {
                        j--;
                        // printeaza ' ' in locul characterului anterior
                        print_letter(' ', i * 3, j * 5);
                    }
                    j--;

                } else if (character == ENTER) {
                    // daca nu au fost introduse 5 litere
                    if (j < 5) {
                        warning1();
                        j--;
                    } else {
                        // castigator retine daca wordul introdus este corect
                        winner = coloring(board[i], word, i * 3 + 5);
                        check = 1;
                    }

                } else if (ch == ':' || ch == ';') {
                    read = 0;
                    erase_gameboard(board);

                    // creeaza fereastra pentru meniul de control
                    WINDOW *menu = main_menu();

                    // cat timp nu se introduce 'r'/'R' sau 'e'/'E'
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

                    // sterge fereastra
                    wbkgd(menu, COLOR_PAIR(1));
                    wclear(menu);
                    wrefresh(menu);
                    curs_set(1);

                // daca se introduce orice alt character
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

            // creeaza fereastra pentru mesajul de castig
            WINDOW *winner = winner_message();

            // cat timp nu se introduce 'y'/'Y' sau 'n'/'N'
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

            // sterge fereastra
            wbkgd(winner, COLOR_PAIR(1));
            wclear(winner);
            wrefresh(winner);
            curs_set(1);

        // daca jucatorul nu a accesat meniul de control si nici nu a castigat
        } else if (i != MAIN_MENU) {
            erase_gameboard(board);

            // creeaza fereastra pentru mesajul de pierdere
            WINDOW *loser = loser_message(word);
            play_again = 0;
            getch();

            // sterge fereastra
            wbkgd(loser, COLOR_PAIR(1));
            wclear(loser);
            wrefresh(loser);
        }
    }

    endwin();
    return 0;
}