#include <stdio.h>
#include <windows.h>

#define UP 72		//Ű���� ��
#define DOWN 80		//Ű���� �Ʒ�
#define LEFT 75		//Ű���� ��
#define RIGHT 77	//Ű���� ����
#define SPACEBAR 32	//Ű���� �����̽���

#define MIN_Y 2		// �� ��
#define MAX_Y 25	// �Ʒ� ��
#define MIN_X 0		// �� ��
#define MAX_X 15	// ������ ��

#define EMPTY 0		// �� �κ�
#define WALL 1		// ��
#define BLOCK 2		// ���

int bx;	//����� ���� x
int by; //����� ���� y
int b_type; //��� ������ ���� 
int b_rotation; //��� ȸ���� ���� 

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
}; //��ϸ�� ���� 4*4������ ����� ǥ�� blcoks[b_type][b_rotation][i][j]�� ��� 


void gotoxy(int x, int y) { //Ŀ�� �̵� �Լ� 
    COORD pos = { 2 * x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void hidecursor()	//Ŀ�� ����� �Լ�
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 1;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

int main(void) {
	system("cls"); //ȭ���� ����� 
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
					printf("��");
					break;
				case BLOCK:
					printf("��");
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

void new_block(void) { //���ο� ��� ����  
    int i, j;

    bx = (MIN_X+MAX_X) / 2 - 1; //��� ���� ��ġx��ǥ(�������� ���) 
    by = (MIN_Y+MAX_Y) / 2 - 1;  //��� ������ġ y��ǥ(���� ��) 

	b_type = 5;			//���߿� ���� ������ ��ü
	b_rotation = 1;		//���߿� ���� ������ ��ü
    for (i = 0;i < 4;i++) { //������ bx, by��ġ�� ������  
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

int check_crush(int bx, int by, int b_rotation) { //������ ��ǥ�� ȸ�������� �浹�� �ִ��� �˻� 
    int i, j;

    for (i = 0;i < 4;i++) {
        for (j = 0;j < 4;j++) { //������ ��ġ�� �����ǰ� ������� ���ؼ� ��ġ�� false�� ���� 
			//if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] != EMPTY) return 0;
			if (blocks[b_type][b_rotation][i][j] == 1 && game_map[by + i][bx + j] == WALL) return 0;
		}
    }
    return 1; //�ϳ��� �Ȱ�ġ�� true���� 
}