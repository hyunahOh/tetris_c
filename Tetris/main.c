#include <stdio.h>
#include <windows.h>

#define UP 72		//키보드 위
#define DOWN 80		//키보드 아래
#define LEFT 75		//키보드 왼
#define RIGHT 77	//키보드 오른
#define SPACEBAR 32	//키보드 스페이스바

#define MIN_Y 2		// 위 끝
#define MAX_Y 25	// 아래 끝
#define MIN_X 0		// 왼 끝
#define MAX_X 15	// 오른쪽 끝

#define EMPTY 0		// 빈 부분
#define WALL 1		// 벽
#define BLOCK 2		// 블록

int bx;	//블록의 현재 x
int by; //블록의 현재 y

char key;
void check_key(void);
void reset_map(void);
void draw_map(void);
void new_block(void);
void move_block(int dir);

int game_map[MAX_Y][MAX_X];

void gotoxy(int x, int y) { //gotoxy함수 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int main(void) {
	reset_map();
	new_block();
	draw_map();
	while (1) {
		check_key();
	}
	return 0;
}

void check_key(void) {
	if (_kbhit()) {
		key = _getch();
		switch (key)
		{
		case UP:
			move_block(UP);
			draw_map();
			break;
		case DOWN:
			move_block(DOWN);
			draw_map();
			break;
		case LEFT:
			move_block(LEFT);
			draw_map();
			break;
		case RIGHT:
			move_block(RIGHT);
			draw_map();
			break;
		case SPACEBAR:
			printf("spacebar\n");
			break;
		default:
			break;
		}
	}
}

void reset_map(void) {
	int i, j;

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map[i][j] = EMPTY;
		}
	}

	for (i = MIN_X; i < MAX_X; i++) {
		game_map[MIN_Y][i] = WALL;
		game_map[MAX_Y-1][i] = WALL;
	}

	for (i = MIN_Y; i < MAX_Y; i++) {
		game_map[i][MIN_X] = WALL;
		game_map[i][MAX_X-1] = WALL;
	}
}

void draw_map(void) {
	int i, j;

	system("cls"); //화면을 지우고 
	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			gotoxy(j, i);
			switch (game_map[i][j]) {
			case EMPTY:
				printf(" ");
				break;
			case WALL:
				printf("▩");
				break;
			case BLOCK:
				printf("■");
				break;
			}
		}
	}
}

void new_block(void) { //새로운 블록 생성  
    int i, j;

    bx = (MIN_X+MAX_X) / 2 - 1; //블록 생성 위치x좌표(게임판의 가운데) 
    by = (MIN_Y+MAX_Y) / 2 - 1;  //블록 생성위치 y좌표(제일 위) 

	for (i = 0; i < 2; i++) {
		for (j = 0; j < 2; j++) {
			game_map[by + j][bx + i] = BLOCK;
		}
	}
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		game_map[by][bx - 1] = BLOCK;
		game_map[by + 1][bx - 1] = BLOCK;
		game_map[by][bx + 1] = EMPTY;
		game_map[by + 1][bx + 1] = EMPTY;
		bx--;
		break;
	case RIGHT:
		game_map[by][bx + 2] = BLOCK;
		game_map[by + 1][bx + 2] = BLOCK;
		game_map[by][bx] = EMPTY;
		game_map[by + 1][bx] = EMPTY;
		bx++;
		break;
	case UP:
		game_map[by - 1][bx] = BLOCK;
		game_map[by - 1][bx + 1] = BLOCK;
		game_map[by + 1][bx] = EMPTY;
		game_map[by + 1][bx + 1] = EMPTY;
		by--;
		break;
	case DOWN:
		game_map[by + 2][bx] = BLOCK;
		game_map[by + 2][bx + 1] = BLOCK;
		game_map[by][bx] = EMPTY;
		game_map[by][bx + 1] = EMPTY;
		by++;
		break;


	}
}