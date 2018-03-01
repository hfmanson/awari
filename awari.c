#include <stdio.h>

#define PPIT 6
#define CPIT 13
#define RANGE 14

int board[RANGE];
int level, answer;
int finis, pl;

int best[2];

void init(int* b) {
    int i;
    for (i = 0; i < RANGE; i++) {
        b[i] = 3;
    }
    b[PPIT] = 0;
    b[CPIT] = 0;
}

void makemove(int* b, int move, int* lastinpit) {
    int beans, pit;

    pit = move < 6 ? PPIT : CPIT;
    beans = b[move];
    b[move] = 0;
    do {
        move = move == CPIT ? 0 : move + 1;
        b[move]++;
        beans--;
    } while (beans > 0);
    if (move == CPIT || move == PPIT) {
        *lastinpit = move == pit;
    } else {
        if (b[move] == 1 && b[12 - move] > 0) {            
            b[pit] += b[12 - move] + 1;
            b[move] = 0;
            b[12-move] = 0;
        }
        *lastinpit = 0;
    }
}

void print(int* b) {
    int i;

    printf("\n   ");
    for (i = 12; i > PPIT; i--) {
        printf("%3d", b[i]);
    }
    printf("\n%3d                  %3d\n   ", b[CPIT], b[PPIT]);
    for (i = 0; i < PPIT; i++) {
        printf("%3d", b[i]);
    }
    printf("\n\n");
}

int stop(int* b) {
    return b[0] + b[1] + b[2] + b[3] + b[4] + b[5] == 0 || b[7] + b[8] + b[9] + b[10] + b[11] + b[12] == 0;
}

void copyboard(int* dst, int* src) {
    int i;

    for (i = 0; i < RANGE; i++) {
        dst[i] = src[i];
    }
}

int minply(int *currentboard, int lvl, int prevmax, int curmin, int first);

int maxply(int *currentboard, int lvl, int prevmin, int curmax, int first) {
    int move, n, lastinpit;

    if (lvl == 0 || stop(currentboard)) {
        return currentboard[CPIT] - currentboard[PPIT];
    } else {
        move = 7;
        do {
            if (currentboard[move] > 0) {
                int nextboard[RANGE];

                copyboard(nextboard, currentboard);
                makemove(nextboard, move, &lastinpit);
                if (lastinpit && first) {
                    n = maxply(nextboard, lvl, prevmin, curmax, 0);
                } else {
                    n = minply(nextboard, lvl - 1, curmax, 99, 1);
                }
                if (n > curmax) {
                    curmax = n;
                    if (lvl == level) {
                        best[first] = move;
                    }
                }
            }
            move++;
        } while (move != CPIT && curmax < prevmin);
        return curmax;
    }
}

int minply(int *currentboard, int lvl, int prevmax, int curmin, int first) {
    int move, n, lastinpit;

    if (lvl == 0 || stop(currentboard)) {
        return currentboard[CPIT] - currentboard[PPIT];
    } else {
        move = 0;
        do {
            if (currentboard[move] > 0) {
                int nextboard[RANGE];

                copyboard(nextboard, currentboard);
                makemove(nextboard, move, &lastinpit);
                if (lastinpit && first) {
                    n = minply(nextboard, lvl, prevmax, curmin, 0);
                } else {
                    n = maxply(nextboard, lvl - 1, curmin, -99, 1);
                }
                if (n < curmin) {
                    curmin = n;
                    if (lvl == level) {
                        best[first] = move;
                    }
                }
            }
            move++;
        } while (move != PPIT && curmin > prevmax);
        return curmin;
    }
}

int inmove(int* b, int first) {
    int move, dummy;

    do {
        do {
            printf("Your move? ");
            scanf("%d", &move);
        } while (move < 0 || move > PPIT);
        if (move == 0) {
            if (first) {
                dummy = minply(b, level, -99, 99, 1);
            }
            move = best[first] + 1;
            printf("Suggested move: %d", move);
        }
    } while (b[move - 1] == 0);
    return move - 1;
}

void player(int *b) {
    int move, lastinpit;

    move = inmove(b, 1);
    makemove(b, move, &lastinpit);
    print(b);
    finis = stop(b);
    if (!finis && lastinpit) {
        move = inmove(b, 0);
        makemove(b, move, &lastinpit);
        print(b);
        finis = stop(b);
    }
}

void computer(int* b) {
    int dummy, lastinpit;

    dummy = maxply(b, level, 99, -99, 1);
    printf("My move %d\n", best[1] - 6);
    makemove(b, best[1], &lastinpit);
    print(b);
    finis = stop(b);
    if (!finis && lastinpit) {
        printf("My second move %d\n", best[0] - 6);
        makemove(b, best[0], &lastinpit);
        print(b);
        finis = stop(b);
    }
}

int main() {
    init(board);
    print(board);
    printf("Level? ");
    scanf("%d", &level);
    printf("Do you want first? (1 - yes, 2 - no) ");
    scanf("%d", &answer);
    pl = answer == 1;
    do {
        if (pl) {
            player(board);
        } else {
            computer(board);
        }
        pl = !pl;
    } while (!finis);
    if (board[CPIT] == board[PPIT]) {
        printf("It's a draw.\n");
    } else if (board[CPIT] > board[PPIT]) {
        printf("I win!\n");
    } else {
        printf("You win!\n");
    }
    return 0;
}

