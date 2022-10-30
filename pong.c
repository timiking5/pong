#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define MAX_SCORE 21
#define ROCKET1_Y 11
#define ROCKET2_Y 11
#define ROCKET1_X 1
#define ROCKET2_X 78
#define FRAME_SPEED 30000
#define BALL_SPEED 300

void moveRocket(int *rocket1_y, int *rocket2_y);
void moveBall(int* x_ball, int* y_ball, int* rocket1_y, int* rocket2_y,
int* x_vector, int* y_vector);
void check_bounds(int* x_ball, int* y_ball, int* x_vector, int* y_vector,
int* player1_sc, int* player2_sc);


int main(void) {
    int rocket1_y = ROCKET1_Y;
    int rocket2_y = ROCKET2_Y;
    int x_ball = 25;
    int y_ball = 12;
    int x_vector = -1;
    int y_vector = -1;
    int player1_sc = 0;
    int player2_sc = 0;
    system("stty cbreak");
    char field[25][80];
    for (int i = 0; i < 25; i++) {
        if ((i == 0) || (i == 24))
            memset(field[i], '#', sizeof(char)*80);
        else {
            memset(field[i], ' ', sizeof(char)*80);
        }
    }

    while ((player1_sc < MAX_SCORE) && (player2_sc < MAX_SCORE)) {
        for (int k = 1; k < 24; k++) {
            field[k][ROCKET1_X] = ' ';
            field[k][ROCKET2_X] = ' ';
        }
        for (int k = 0; k < 3; k++) {
            field[rocket1_y + k][ROCKET1_X] = '|';
            field[rocket2_y + k][ROCKET2_X] = '|';
        }
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 80; j++) {
                printf("%c", field[i][j]);
            }
            printf("\n");
        }
        field[4][36] = '0' + player1_sc%10;
        field[4][35] = '0' + player1_sc/10;
        field[4][45] = '0' + player2_sc/10;
        field[4][46] = '0' + player2_sc%10;  // табло
        field[y_ball][x_ball] = 'O';  // шар
        field[y_ball - y_vector][x_ball - x_vector] = ' ';
        moveRocket(&rocket1_y, &rocket2_y);
        moveBall(&x_ball, &y_ball, &rocket1_y, &rocket2_y, &x_vector, &y_vector);
        check_bounds(&x_ball, &y_ball, &x_vector, &y_vector, &player1_sc, &player2_sc);
        usleep(FRAME_SPEED);  // не моментальное движение
    }
    if (player1_sc == MAX_SCORE)
        printf("\n\n\n\n\n\n\n player 1 won");
    else
        printf("\n\n\n\n\n\n\n player 1 won");
    return 0;
}

void moveRocket(int *rocket1_y, int *rocket2_y) {
    int move;
    initscr();
    keypad(stdscr, TRUE);
    timeout(BALL_SPEED);
    move = getch();
    switch (move) {
        case 'a' :
            if ((*rocket1_y) < 21)
                *rocket1_y += 1;
            break;
        case 'z' :
            if (*rocket1_y - 1 > 0)
                *rocket1_y -= 1;
            break;
        case 'k' :
            if (*rocket2_y < 21)
                *rocket2_y += 1;
            break;
        case 'm' :
            if (*rocket2_y - 1 > 0)
                *rocket2_y -= 1;
            break;
        default:
            break;
    }
    refresh();
    endwin();
}


void moveBall(int* x_ball, int* y_ball, int* rocket1_y, int* rocket2_y, int* x_vector, int* y_vector) {
    if ((*x_ball == ROCKET1_X + 1) && (*y_ball >= *rocket1_y) && (*y_ball <= *rocket1_y + 2)) {
        *x_vector = -*x_vector;
    }
    else if ((*x_ball == ROCKET2_X - 1) && (*y_ball >= *rocket2_y) && (*y_ball <= *rocket2_y + 2)) {
        *x_vector = -*x_vector;
    }
    else if ((*y_ball == 1))
        *y_vector = -*y_vector;
    else if ((*y_ball == 23))
        *y_vector = -*y_vector;
    *x_ball += *x_vector;
    *y_ball += *y_vector;
}

void check_bounds(int* x_ball, int* y_ball, int* x_vector, int* y_vector, int* player1_sc, int* player2_sc) {
    if (*x_ball > 78) {
        *player1_sc += 1;
        *x_ball = 65;
        *y_ball = 12;
        *x_vector = -1;
        *y_vector = 1;
    }
    else if (*x_ball < 1) {
        *player2_sc += 1;
        *x_ball = 25;
        *y_ball = 12;
        *x_vector = 1;
        *y_vector = 1;
    }
}
