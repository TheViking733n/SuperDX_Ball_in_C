#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>



#define ROWS    20
#define COLS    80
#define HEART   3       // ♥
#define TL      201     // ╔
#define TR      187     // ╗
#define BL      200     // ╚
#define BR      188     // ╝
#define H       205     // ═
#define V       186     // ║
#define BLOCK   178     // █
#define BALL    79      // O
#define PADDLE  223     // ▀
#define WALL    220     // ▄
#define SPACE   32      // space
#define SPIKES  30      // ▲
#define NOKEY   255     // no key

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESC 27
#define KB_F8 66

char Canvas[ROWS][COLS + 1];

int X, Y, velX, velY, pX, pY, Sz, Score, Lives;


// Fundtion prototypes
void init_window();
void flush_canvas();
void pause();
void draw_border();
void create_canvas();
void move_cursor(int x, int y);
void update_life(int new_life);
void update_score(int new_score);
void draw_ball(int x, int y, int col);
int in_bounds_x(int x);
int in_bounds_y(int y);
int in_bounds(int x, int y);
int will_collide_x();
int will_collide_y();
int will_collide();
void move_ball();
void init_game();
void hide_cursor();
void hidecursor();
void draw_paddle(int x, int y, int col);
void move_paddle(int dist);
char input();


void init_window() {
    system("chcp 437 > nul");   // To change codepage to 437
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, 0, 0, r.right - r.left, r.bottom - r.top, TRUE);
    char command[40];
    memset(command, ' ', 40);
    sprintf(command, "mode con: cols=%d lines=%d", COLS, ROWS);
    system(command);
    system("cls");
}


void flush_canvas() {
    system("cls");
    for (int i = 0; i < ROWS; i++) {
        printf("%s%c", Canvas[i], "\n"[i == ROWS - 1]);
    }
}

void pause() {
    system("pause >nul");
}

void draw_border() {
    for (int i = 1; i < COLS - 1; i++) {
        Canvas[1][i] = H;
        Canvas[ROWS - 2][i] = SPIKES;
        Canvas[ROWS - 1][i] = H;
    }
    for (int i = 2; i < ROWS - 1; i++) {
        Canvas[i][0] = V;
        Canvas[i][COLS - 1] = V;
    }
    Canvas[1][0] = TL;
    Canvas[1][COLS - 1] = TR;
    Canvas[ROWS - 1][0] = BL;
    Canvas[ROWS - 1][COLS - 1] = BR;
}

void create_canvas() {
    memset(Canvas, SPACE, sizeof(Canvas));
    sprintf(&Canvas[0][0], " Lives: ");
    for (int i = 0; i < Lives; i++) {
        Canvas[0][8 + i*2] = 3;
        Canvas[0][9 + i*2] = SPACE;

    }
    sprintf(&Canvas[0][COLS - 14], "Score: %06d", Score);
    draw_border();
    for (int i = 0; i < ROWS; i++) {
        Canvas[i][COLS] = '\0';
    }
}

void move_cursor(int x, int y) {
    printf("\033[%d;%dH", y + 1, x + 1);
}

void update_life(int new_life) {
    if (new_life < 0) return;
    memset(&Canvas[0][8], SPACE, Lives * 2);
    Lives = new_life;
    for (int i = 0; i < Lives; i++) {
        Canvas[0][8 + i*2] = 3;
        Canvas[0][9 + i*2] = SPACE;
    }
}

void update_score(int new_score) {
    Score = new_score;
    sprintf(&Canvas[0][COLS - 14], "Score: %06d", Score);
}

void draw_ball(int x, int y, int col) {
    // If col = 0, hides the ball at x, y position
    // else draws the ball at x, y
    if (col == 0) {
        Canvas[y][x] = SPACE;
        move_cursor(x, y);
        printf("%c", SPACE);
    } else {
        Canvas[y][x] = BALL;
        move_cursor(x, y);
        printf("%c", BALL);
    }
}


int in_bounds_x(int x) {
    return x >= 0 && x < COLS;
}

int in_bounds_y(int y) {
    return y >= 0 && y < ROWS;
}

int in_bounds(int x, int y) {
    return in_bounds_x(x) && in_bounds_y(y);
}

int will_collide_x() {
    // Returns the character code to which the ball is colliding,
    // if not colliding then returns 0
    int sgn = velX > 0 ? 1 : -1;
    for (int i = 1; i <= abs(velX); i++) {
        if (!in_bounds_x(X + sgn*i) || Canvas[Y][X + sgn*i] != SPACE) {
            return in_bounds_x(X + sgn*i) ? Canvas[Y][X + sgn*i] : 1;
        }
    }
    return 0;
}

int will_collide_y() {
    // Returns the character code to which the ball is colliding,
    // if not colliding then returns 0
    int sgn = velY > 0 ? 1 : -1;
    for (int i = 1; i <= abs(velY); i++) {
        if (!in_bounds_y(Y + sgn*i) || Canvas[Y + sgn*i][X] != SPACE) {
            return in_bounds_y(Y + sgn*i) ? Canvas[Y + sgn*i][X] : 1;
        }
    }
    return 0;
}

int will_collide() {
    return !in_bounds(X + velX, Y + velY) || Canvas[Y + velY][X + velX] != SPACE || will_collide_x() || will_collide_y();
}

void move_ball() {
    draw_ball(X, Y, 0);
    int f = 1, ch;
    if (ch = will_collide_x()) {
        velX = -velX;
        f = 0;
    }
    if (ch = will_collide_y()) {
        if (ch == SPIKES) {
            init_game();
            Sleep(300);
            create_canvas();
            update_life(Lives );
            draw_ball(X, Y, 1);
            draw_paddle(pX, pY, 1);
            flush_canvas();
        } else {
            velY = -velY;
            f = 0;
        }
    }
    if (f && will_collide()) {
        velX = -velX;
        velY = -velY;
    }
    

    X += velX;
    Y += velY;
    draw_ball(X, Y, 1);
}


void init_game() {
    X = COLS / 2;
    Y = ROWS / 2;
    velX = 2;
    velY = 1;
    Sz = 12;
    pX = (COLS - Sz) / 2;
    pY = ROWS - 5;
    Score = 0;
    Lives = 14;
}


void hide_cursor() {
    // printf("\033[?25l");  // Hide cursor
    printf("\e[?25l");  // Hide cursor
}

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void draw_paddle(int x, int y, int col) {
    // If col = 0, hides the paddle at x, y position
    // else draws the paddle at x, y
    int ch = col == 0 ? SPACE : PADDLE;
    for (int i = 0; i < Sz && in_bounds(x + i, y); i++) {
        Canvas[y][x + i] = ch;
        move_cursor(x + i, y);
        printf("%c", ch);
    }
}

void move_paddle(int dist) {
    draw_paddle(pX, pY, 0);
    pX += dist;
    if (pX <= 0) pX = 1;
    if (pX + Sz >= COLS) pX = COLS - Sz - 1;
    draw_paddle(pX, pY, 1);
}

char input()
{
    if (kbhit()) {
        switch (getch()) {
        case KB_LEFT:
        case 'A':
        case 'a':
            return 'a';
            break;
        case  KB_RIGHT:
        case 'D':
        case 'd':
            return 'd';
            break;
        case KB_ESC:
            return 'q';
            break;
        }
    }
    return NOKEY;
}

int main() {
    init_window();
    init_game();
    hide_cursor();
    create_canvas();
    draw_ball(X, Y, 1);
    draw_paddle(pX, pY, 1);
    flush_canvas();

    move_cursor(12, 8);
    printf("Welcome  to   the     game             of The Viking");
    sprintf(&Canvas[8][12], "Welcome  to   the     game             of The Viking");
    int t = 1000000;
    while (t--) {
        // hide_cursor();
        hidecursor();
        move_ball();
        Sleep(100);
        char inp = input();
        if (inp == 'a') {
            move_paddle(-3);
        } else if (inp == 'd') {
            move_paddle(3);
        } else if (inp == 'q') {
            break;
        }
        if (t%100==0) {
            draw_border();
            flush_canvas();
        }
    }


    return 0;
}