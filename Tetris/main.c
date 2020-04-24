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
int b_type; //블록 종류를 저장 
int b_rotation; //블록 회전값 저장 

char key;
void check_key(void);
void reset_map(void);
void draw_map(void);
void new_block(void);
void move_block(int dir);
int check_crush(int bx, int by, int b_rotation);

int game_map[MAX_Y][MAX_X];
int game_map_copy[MAX_Y][MAX_X];

int blocks[7][4][4][4] = {
{{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
{{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
{{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
 {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
{{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
{{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
{{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
 {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
}; //블록모양 저장 4*4공간에 블록을 표현 blcoks[b_type][b_rotation][i][j]로 사용 


void gotoxy(int x, int y) { //커서 이동 함수 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hidecursor()	//커서 숨기는 함수
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int main(void) {
	system("cls"); //화면을 지우고 
	hidecursor();
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
			if (check_crush(bx, by, (b_rotation + 1) % 4) != 0) {
				move_block(UP);
			}
			draw_map();
			break;
		case DOWN:
			if (check_crush(bx, by + 1, b_rotation) != 0) {
				move_block(DOWN);
			}
			draw_map();
			break;
		case LEFT:
			if (check_crush(bx - 1, by, b_rotation)!=0) {
				move_block(LEFT);
			}
			draw_map();
			break;
		case RIGHT:
			if (check_crush(bx + 1, by, b_rotation) != 0) {
				move_block(RIGHT);
			}
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

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			if (game_map[i][j] != game_map_copy[i][j]) {
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

	for (i = 0; i < MAX_Y; i++) {
		for (j = 0; j < MAX_X; j++) {
			game_map_copy[i][j] = game_map[i][j];
		}
	}
}

void new_block(void) { //새로운 블록 생성  
    int i, j;

    bx = (MIN_X+MAX_X) / 2 - 1; //블록 생성 위치x좌표(게임판의 가운데) 
    by = (MIN_Y+MAX_Y) / 2 - 1;  //블록 생성위치 y좌표(제일 위) 

	b_type = 5;			//나중엔 임의 값으로 대체
	b_rotation = 1;		//나중엔 임의 값으로 대체
    for (i = 0;i < 4;i++) { //게임판 bx, by위치에 블럭생성  
        for (j = 0;j < 4;j++) {
            if (blocks[b_type][b_rotation][i][j] == 1) game_map[by + i][bx + j] = BLOCK;
        }
    }
}

void move_block(int dir) {
	int i, j;

	switch (dir) {
	case LEFT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		bx--;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	case RIGHT:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		bx++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	case UP:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		b_rotation = (b_rotation + 1) % 4;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	case DOWN:
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (game_map[by + i][bx + j] != WALL) {
					game_map[by + i][bx + j] = EMPTY;
				}
			}
		}
		by++;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (blocks[b_type][b_rotation][i][j] == 1) {
					game_map[by + i][bx + j] = BLOCK;
				}
			}
		}
		break;
	}
}

int check_crush(int bx, int by, int b_rotation) { //지정된 좌표와 회전값으로 충돌이 있는지 검사 
    int i, j;

    for (i = 0;i < 4;i++) {
        for (j = 0;j < 4;j++) { //지정된 위치의 게임판과 블럭모양을 비교해서 겹치면 false를 리턴 
			//if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] != EMPTY) return 0;
			if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] == WALL) return 0;
		}
    }
    return 1; //하나도 안겹치면 true리턴 
}