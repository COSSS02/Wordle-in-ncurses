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

// alege cuvantul castigator
void choose_word(char *cuvant)
{
    // lista de cuvinte posibile
    const char cuvinte[MAX_WORDS][6] = {"arici", "atent", "baiat", "ceata",
        "debut", "peste", "fixat", "hamac", "harta", "jalon", "jucam",
        "lacat", "magie", "nufar", "oaste", "perus", "rigle", "roman",
        "sanie", "scris", "sonda", "texte", "tipar", "titan", "zebra",
        "vapor", "vatra"};
    // alegerea cuvantului in mod aleator
    strcpy(cuvant, cuvinte[time(NULL) % MAX_WORDS]);
}

// printeaza titlul jocului si un mesaj informativ
void initial_text()
{
    int center = COLS / 2 - 3;
    // seteaza fundalul intregii ferestre
    bkgd(COLOR_PAIR(1));

    attron(COLOR_PAIR(1) | A_BOLD);
    mvaddstr(1, center, "WORDLE");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvaddstr(LINES - 3, center - 9, "APASA : PENTRU MAIN MENU");
    attroff(COLOR_PAIR(3) | A_BOLD);
}

// creeaza un bloc din tabla de joc la pozitia (x,y)
WINDOW *boardgame(int y, int x)
{
    // folosite pentru a pozitiona tabla de joc in centru
    int yaxis = 4, xaxis = COLS / 2 - 13;

    // generarea noii ferestre si desenarea conturului
    WINDOW *cutie = newwin(3, 5, yaxis + y, xaxis + x);
    box(cutie, 0, 0);

    wrefresh(cutie);
    return cutie;
}

// printeaza caracterul ch in tabla de joc
void print_letter(char ch, int y, int x)
{
    int yaxis = 5, xaxis = COLS / 2 - 11;
    attron(COLOR_PAIR(3));
    mvaddch(yaxis + y, xaxis + x, ch);
    attroff(COLOR_PAIR(3));
    refresh();
}

void warning1()
{
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddstr(LINES - 5, COLS / 2 - 12, "TREBUIE SCRISE 5 LITERE");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
}

void warning2()
{
    attron(COLOR_PAIR(2) | A_BOLD);
    mvaddstr(LINES - 4, COLS / 2 - 12, "LITERE MICI SAU BACKSPACE");
    attroff(COLOR_PAIR(2) | A_BOLD);
    refresh();
}

// coloreaza un bloc cu verde pentru nr = 4 sau cu galben pentru nr = 6
void color_box(WINDOW *cutie, char ch, int nr)
{
    // colorarea blocului
    wbkgd(cutie, COLOR_PAIR(nr));

    // reprintarea literei intrucat este acoperita la colorarea blocului
    wattron(cutie, COLOR_PAIR(nr + 1));
    wmove(cutie, 1, 2);
    waddch(cutie, ch);
    wattroff(cutie, COLOR_PAIR(nr + 1));

    wrefresh(cutie);
}

// verifica ce blocuri ale cuvantului introdus trebuie colorate
int coloring(WINDOW **cutie, char *cuvant, int yaxis)
{
    int i, j;
    int xaxis = COLS / 2 - 11;
    // memoreaza cate blocuri au fost colorate cu verde
    int green = 0;
    // marcheaza literele ce au fost verificate din cuvantul introdus
    int cautat[5] = {0};

    for (i = 0; i <= 4; i++) {
        // daca litera nu a fost verificata
        if (!cautat[i]) {
            // litera de pe pozitia i din cuvantul introdus
            char caracter = mvinch(yaxis, xaxis + i * 5);

            // retine de cate ori apare litera actuala in cuvantul cautat
            int contor_cuv = 0;
            for (j = 0; j <= 4; j++) {
                if (cuvant[j] == caracter) {
                    contor_cuv++;
                }
            }

            for (j = i; j <= 4; j++) {
                // cauta literele egale cu litera de pe pozitia i
                char temp = mvinch(yaxis, xaxis + j * 5);
                if (temp == caracter) {
                    // daca litera mai exista in cuvantul cautat
                    if (contor_cuv) {
                        // daca cele doua litere sunt pe aceiasi pozitie
                        if (cuvant[j] == temp) {
                            // memoreaza verificarea literei
                            cautat[j] = 1;

                            // coloreaza blocul cu verde
                            color_box(cutie[j], temp, 4);

                            contor_cuv--;
                            green++;
                        }
                    }
                }
            }

            /* se verifica din nou fiecare litera pentru a le gasi pe cele
            ce trebuie colorate cu galben */
            for (j = i; j <= 4; j++) {
                char temp = mvinch(yaxis, xaxis + j * 5);
                if (temp == caracter) {
                    if (contor_cuv && !cautat[j]) {
                        if (strchr(cuvant, temp)) {
                            color_box(cutie[j], temp, 6);
                            contor_cuv--;
                        }
                    }
                    cautat[j] = 1;
                }
            }
        }
    }

    // daca a fost ghicit cuvantul
    if (green == 5) {
        return 1;
    }
    return 0;
}

// sterge blocurile din tabla de joc
void erase_gameboard(WINDOW *cutii[6][5])
{
    int i, j;
    for (i = 0; i <= 5; i++) {
        for (j = 0; j <= 4; j++) {
            wbkgd(cutii[i][j], COLOR_PAIR(1));
            wclear(cutii[i][j]);
            wrefresh(cutii[i][j]);
        }
    }
}

WINDOW *winner_message()
{
    // stergerea informatiilor de pe ecran
    mvaddstr(LINES - 3, COLS / 2 - 12, "                        ");
    mvaddstr(LINES - 5, COLS / 2 - 12, "                       ");
    mvaddstr(LINES - 4, COLS / 2 - 12, "                         ");
    curs_set(0);

    // generarea ferestrei pentru cuvantul ghicit
    WINDOW *win = newwin(10, 30, LINES / 2 - 7, COLS / 2 - 15);
    wbkgd(win, COLOR_PAIR(5));
    wmove(win, 1, 9);
    waddstr(win, "AI CASTIGAT!");
    wmove(win, 3, 9);
    waddstr(win, "PLAY AGAIN?");
    wmove(win, 6, 8);
    waddstr(win, "Y           N");
    wrefresh(win);
    return win;
}

WINDOW *loser_message(char *cuvant)
{
    // stergerea informatiilor de pe ecran
    mvaddstr(LINES - 3, COLS / 2 - 12, "                        ");
    mvaddstr(LINES - 5, COLS / 2 - 12, "                       ");
    mvaddstr(LINES - 4, COLS / 2 - 12, "                         ");
    curs_set(0);

    // generarea ferestrei daca cuvantul nu a fost ghicit
    WINDOW *win = newwin(10, 30, LINES / 2 - 7, COLS / 2 - 15);
    wbkgd(win, COLOR_PAIR(8));
    wmove(win, 1, 9);
    waddstr(win, "AI PIERDUT!");
    wmove(win, 3, 6);
    waddstr(win, "CUVANTUL ERA:");
    wmove(win, 3, 19);

    // afisarea cuvantului care trebuia ghicit
    wattron(win, COLOR_PAIR(9) | A_BOLD);
    waddstr(win, cuvant);
    wattroff(win, COLOR_PAIR(9) | A_BOLD);

    wmove(win, 6, 4);
    waddstr(win, "PRESS ANY KEY TO EXIT");
    wrefresh(win);
    return win;
}

WINDOW *main_menu()
{
    // stergerea informatiilor de pe ecran
    mvaddstr(LINES - 3, COLS / 2 - 12, "                        ");
    mvaddstr(LINES - 5, COLS / 2 - 12, "                       ");
    mvaddstr(LINES - 4, COLS / 2 - 12, "                         ");
    curs_set(0);

    // generarea meniului de control
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