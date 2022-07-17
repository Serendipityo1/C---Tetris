#include"stdio.h"
#include"time.h"
#include"conio.h"	//kbhit()
#include"graphics.h"
#include"stdlib.h"
#include"string.h"
#pragma comment(lib,"winmm.lib")

int NO=0 ;
int score = 0;	//�ܷ�
int rank = 0;	//�ȼ�
int show = 0; //���а������0����ʾ��1��ʾ
#define BLOCK_COUNT 5
#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 5

#define UNIT_SIZE 20	//С������

#define START_X 130		//���齵��򣬷��齵����ʼλ��
#define START_Y 30

#define KEY_UP 87		//�û�����
#define KEY_LEFT 65
#define KEY_RIGHT 68
#define KEY_DOWN 83
#define KEY_SPACE 32
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80


#define MinX 30		//��Ϸ���Ͻ�λ��
#define MinY 30

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000    //��ʱ��һ��
#endif
			 
int speed = 500;	//���齵���ٶ�

int NextIndex = -1;		//��һ������
int BlockIndex = -1;		//��ǰ����

struct PLAYER
{
	char  name[50];
	char score[20];
};
struct PLAYER Player[100];
struct PLAYER Rank_Player[100];

typedef enum {		//���鳯��
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_LEFT,
	BLOCK_DOWN
}block_dir_t;

typedef enum {		//�����ƶ�����
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

//������ɫ
int color[BLOCK_COUNT] = {
	GREEN,
	CYAN,
	MAGENTA,
	YELLOW,
	BROWN
};
int visit[30][15];	//��������visit[i][j] = 1��ʾ��λ���з���
int markColor[30][15];	//��Ӧλ����ɫ
int block[BLOCK_COUNT * 4][BLOCK_WIDTH][BLOCK_HEIGHT] = {
	// | 
	{ 0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
	// L 
   { 0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
   // ��
   { 0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0 },
   // T
   { 0,0,0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0 },
   // Z 
   { 0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0 },
   { 0,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0 },
};

//���ܣ���ӭҳ��
int welcome() {
	//��ʼ������
	initgraph(550, 660);
	//���ô���
	HWND window = GetHWnd();
	SetWindowText(window, _T("Tetris"));
	//��ӭ����
	IMAGE picture;
	loadimage(&picture, "D:\\Curriculum\\C\\ClassDesign\\Tetris.jpg", 550, 660);
	putimage(0, 0, &picture);
	
	//���
	MOUSEMSG m;
	while (1) {
		m = GetMouseMsg();
		while (m.uMsg == WM_LBUTTONDOWN) {
			if (m.x >= 155 && m.x <= 381 && m.y >= 275 && m.y <= 361) {
				return 1;  //Start Game
			}
			else if (m.x >= 155 && m.x <= 381 && m.y >= 380 && m.y <= 466) {
				closegraph;
				return 0;  //Rank List
			}
		}
		
	}
	//��ʱ3��
	//Sleep(3000);
}
//�����û�����
void Inputname() {
	InputBox(Player[NO].name, 50, "Please Input Your Name(In English):");
	//_getch(); //��������رջ�ӭ����
	closegraph;
}


// ���ܣ���ʼ����Ϸ����
void initGameScene() {
	Inputname();
	char str[16];	//��ŷ���
	//1.����
	cleardevice();
	//2.������
	rectangle(29, 29, 340, 635);	//��Ϸ��
	rectangle(370, 60, 515, 205);	//��ʾ��
	//Next
	setfont(24, 0, _T("����"));		
	setcolor(WHITE);
	outtextxy(370, 30, _T("Next��"));
	//Score
	setcolor(WHITE);					
	outtextxy(370, 280, _T("Score��"));
	     //��scoreд��str
	sprintf_s(str, 16, "%d", score);
	outtextxy(380, 310, str);
	//Rank
	outtextxy(370, 375, _T("Rank��"));
		//��rankд��str
	sprintf_s(str, 16, "%d", rank);
	outtextxy(380, 405, str);
	//OI
	setcolor(WHITE);	
	outtextxy(370, 475, "OI: ");
	outtextxy(370, 500, "W: Rotate");
	outtextxy(370, 525, "S: Descend");
	outtextxy(370, 550, "A: Shift Left");
	outtextxy(370, 575, "D: Shift Right");
	outtextxy(370, 600, "Blank: Pause");
}


 // ���ܣ���շ�����ʾ����ķ���
void clearBlock() {
	setcolor(BLACK);
	setfont(23, 0, "����");
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			int x = 390 + j * UNIT_SIZE;
			int y = 70 + i * UNIT_SIZE;
			outtextxy(x, y, "��");
		}
	}
}


//���ܣ������������еķ���
void clearBlock(int x, int y, block_dir_t blockDir) {
	setcolor(BLACK);
	//	setfont(23, 0, "����");
	int id = BlockIndex * 4 + blockDir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[id][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "��");
			}
		}
	}
}

// ���ܣ�����ʾ�� �� ��������ʼλ�û�����
void drawBlock(int x, int y) {
	setcolor(color[NextIndex]);
	setfont(23, 0, "����");
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[NextIndex * 4][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "��");
			}
		}
	}
}

//���ܣ������½������еķ���
void drawBlock(int x, int y, block_dir_t dir) {
	setcolor(color[BlockIndex]);
	setfont(23, 0, "����");
	int id = BlockIndex * 4 + dir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[id][i][j] == 1) {
				//�����÷���ĵ�i�е�j��
				outtextxy(x + j * UNIT_SIZE, y + i * UNIT_SIZE, "��");
			}
		}
	}
}

//���ܣ�������ʾ���в����·���
void nextblock() {
	clearBlock();
	//��������������ѡ�񷽿�
	srand((unsigned)time(NULL));	//ʹ��ʱ�亯���ķ���ֵ������Ϊ�������
	NextIndex = rand() % BLOCK_COUNT;	//����0~5�������
	drawBlock(390, 70);
}

//���ܣ��жϷ����ܷ���ָ�������ƶ�
bool moveable(int x0, int y0, move_dir_t moveDir, block_dir_t blockDir) {
	//���㷽�����Ͻ���30��15����Ϸ��λ�ã��ڶ����У� �ڶ����У�
	int x = (y0 - MinY) / UNIT_SIZE;
	int y = (x0 - MinX) / UNIT_SIZE;
	int ret = 1;
	int id = BlockIndex * 4 + blockDir;
	if (moveDir == MOVE_DOWN) {
		for (int i = 0; i < BLOCK_HEIGHT; ++i) {
			for (int j = 0; j < BLOCK_WIDTH; ++j) {
				//���²����˶���������ʵ�ķ����Ѿ��ﵽ�ײ���x+i+1==30�������ߵײ����з���	
				if (block[id][i][j] == 1 &&
					(x + i + 1 == 30 || visit[x + i + 1][y + j] == 1)) {
					ret = 0;
				}
			}
		}
	}
	else if (moveDir == MOVE_LEFT) {
		for (int i = 0; i < BLOCK_HEIGHT; ++i) {
			for (int j = 0; j < BLOCK_WIDTH; ++j) {
				//�������˶���������ʵ�ķ����Ѿ��ﵽ��߽�(y+j==0),����������з���
				if (block[id][i][j] == 1 &&
					(y + j <= 0 || visit[x + i][y + j - 1] == 1)) {
					ret = 0;
				}
			}
		}
	}
	else if (moveDir == MOVE_RIGHT) {
		for (int i = 0; i < BLOCK_HEIGHT; ++i) {
			for (int j = 0; j < BLOCK_WIDTH; ++j) {
				//���²����˶���������ʵ�ķ����Ѿ��ﵽ�ұ߽磨y+j+1>=15)�������ұ����з���
				if (block[id][i][j] == 1 &&
					(y + j + 1 >= 15 || visit[x + i][y + j + 1] == 1)) {
					ret = 0;
				}
			}
		}
	}
	return ret;
}
//���ܣ����а�����Ƿ���ʾ
void WhetherRankListShow() {
	int* p;
	p = &show;
	*p = 1;
}

//���ܣ�1.���а������� 2.���沢�����û���¼������+������
void RANK_SAVE(){
	FILE* fp;
	if (show == 1) {  //���а������ʾ
		initgraph(550, 660);
		HWND window = GetHWnd();
		SetWindowText(window, _T("RankList"));
		//�ļ�������ʾ
		int i = 0;
		//�ļ�����
		if ((fp = fopen("D:\\Curriculum\\C\\ClassDesign\\UsersRank.txt", "r")) == NULL) {
			printf_s("UsersRank.txt open error!");
			exit(1);
		}
		while (!feof(fp)) {
		//while(i<3){
			fscanf(fp, "%s  %s\n", &Rank_Player[i].name, &Rank_Player[i].score);
			i++;
		}
		fclose(fp);
		//��ʾ
		setfont(24, 0, _T("����"));
		setcolor(WHITE);
		outtextxy(0, 10, _T(" < Return "));
		outtextxy(0, 50, _T("  *************  RANK LIST  *************    "));
		//1
		outtextxy(200, 180, Rank_Player[0].name);
		outtextxy(290, 180, Rank_Player[0].score);
		//2
		outtextxy(200, 310, Rank_Player[1].name);
		outtextxy(290, 310, Rank_Player[1].score);
		//3
		outtextxy(200, 440, Rank_Player[2].name);
		outtextxy(290, 440, Rank_Player[2].score);

		//�ٴγ�ʼ��
		//memset(Player, 0, sizeof(PLAYER));
		//struct PLAYER Player[100] = { };
		NO = 0;
		//_getch();
		//����� "< Return"
		
		MOUSEMSG m;
		while (1) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN) {
				if (m.x >= 0 && m.x <= 200 && m.y >= 10 && m.y <= 40) {
					break;
				}
			}
		}
		closegraph;
	}
	else {
		if ((fp = fopen("D:\\Curriculum\\C\\ClassDesign\\UsersRank.txt", "a+")) == NULL) {
			printf_s("UsersRank.txt open error!");
			exit(2);
		}
		//д���ĵ�
		fprintf(fp, "%s  %s\n", Player[NO].name, Player[NO].score);
		rewind(fp);
		fclose(fp);
		//����ṹ��
		if ((fp = fopen("D:\\Curriculum\\C\\ClassDesign\\UsersRank.txt", "r")) == NULL) {
			printf_s("UsersRank.txt open error!");
			exit(2);
		}
		NO = 0;
		while (!feof(fp)) {
			fscanf(fp, "%s  %s\n", &Player[NO].name, &Player[NO].score);
			NO++;
		}
		rewind(fp);
		fclose(fp);
		//����
		struct PLAYER tmp;
		for (int i = 0; i < NO; i++) {
			for (int j = i + 1; j < NO; j++) {
				if (strcmp(Player[i].score, Player[j].score) < 0) {
					tmp = Player[i];
					Player[i] = Player[j];
					Player[j] = tmp;
				}
			}
		}
		//����
		if ((fp = fopen("D:\\Curriculum\\C\\ClassDesign\\UsersRank.txt", "w")) == NULL) {
			printf_s("UsersRank.txt open error!");
			exit(2);
		}
		for (int i = 0; i < NO; i++) {
			fprintf(fp, "%s  %s\n", Player[i].name, Player[i].score);
		}
		fclose(fp);
	}
}

//���ܣ������Ϸ�Ƿ����
void failCheck() {
	//��Ϸ���������Ƕ����±����Ƴ��ķ����Ҫ���̻����������»��Ƶķ��鷽���ϣ��˶�������
	if (!moveable(START_X, START_Y, MOVE_DOWN, (block_dir_t)BLOCK_UP)) {
		setcolor(WHITE);
		setfont(45, 0, "����");
		outtextxy(75, 300, "Game Over��");
		RANK_SAVE();
		Sleep(5000);
		//timer(1);
		//system("pause");
		closegraph();
		//exit(0);
	}
}

//���ܣ���ʱ�ȴ�
void wait(int interval) {
	int count = interval / 10;
	for (int i = 0; i < count; ++i) {
		Sleep(10);
		//��������ڼ��û��а��������������
		if (_kbhit()) {
			return;
		}
	}
}

//���ܣ��жϵ�ǰ�����ܷ���ָ��������ת
bool rotatable(int x, int y, block_dir_t dir) {
	//�����ж��Ƿ���Լ��������ƶ�
	if (!moveable(x, y, MOVE_DOWN, dir)) {
		return false;
	}
	int x2 = (y - MinY) / UNIT_SIZE;
	int y2 = (x - MinX) / UNIT_SIZE;
	int id = BlockIndex * 4 + dir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			//������ת���������ұ߽�Խ��������з��顰�赲��
			if (block[id][i][j] == 1 && (y2 + j < 0 || y2 + j >= 15 || visit[x2 + i][y2 + j] == 1)) {
				return false;
			}
		}
	}
	return true;
}

// ���ܣ��жϷ����Ƿ��ܼ����ƶ��󣬹̶�����
void mark(int x, int y, block_dir_t dir) {
	int id = BlockIndex * 4 + dir;
	int x2 = (y - MinY) / UNIT_SIZE;
	int y2 = (x - MinX) / UNIT_SIZE;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[id][i][j] == 1) {
				visit[x2 + i][y2 + j] = 1;
				markColor[x2 + i][y2 + j] = color[BlockIndex];
			}
		}
	}
}

// ���ܣ���ȡ�û�������ʱʱ���½���ķ���
void move() {
	int x = START_X;	//������ʼλ��
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = (block_dir_t)BLOCK_UP;
	//��ȡ�û�����ǰ�ж���Ϸ�Ƿ����
	failCheck();
	//�������½���
	while (1) {
		int curSpeed = speed;	//���嵱ǰ���齵���ٶ�
		//�������
		clearBlock(x, k + y, blockDir);
		//�ж�ѡ��ķ���
		if (_kbhit( )) {
			int key = _getch();
			if (key == KEY_SPACE ) {
				system("pause");
			}
			else if (key == KEY_UP || key == UP) {
				block_dir_t nextDir = (block_dir_t)((blockDir + 1) % 4);
				if (rotatable(x, y + k, nextDir)) {
					blockDir = nextDir;
				}
			}
			else if (key == KEY_LEFT || key == LEFT) {
				if (moveable(x, y + k + 20, MOVE_LEFT, blockDir)) {
					x -= UNIT_SIZE;
				}
			}
			else if (key == KEY_RIGHT || key == RIGHT) {
				if (moveable(x, y + k + 20, MOVE_RIGHT, blockDir)) {
					x += UNIT_SIZE;
				}
			}
			else if (key == KEY_DOWN || key == DOWN) {
				curSpeed = 50;
			}
		}
		k += 20;
		//���Ʒ���
		drawBlock(x, y + k, blockDir);
		//����
		wait(curSpeed);
		//����Ĺ̻���������̶������ѭ������ǰһ�������moveִ�����
		if (!moveable(x, y + k, MOVE_DOWN, blockDir)) {
			mark(x, y + k, blockDir);
			break;
		}
	}
}

//���ܣ����ƸմӶ�������ķ��飬������ʾ���ڵķ��飬���÷��齵�亯��move()
void newblock() {
	BlockIndex = NextIndex;
	//���ƸմӶ����½��ķ���
	drawBlock(START_X, START_Y);
	//���³��ַ�����ͣһ��
	Sleep(200);
	//timer(0.2);
	//���Ͻ����������һ������
	nextblock();
	//���齵��
	move();
}

//���ܣ�������i�У�����������ж�������
void down(int x) {
	for (int i = x; i > 0; --i) {
		for (int j = 0; j < 15; ++j) {
			if (visit[i - 1][j] == 1) {
				visit[i][j] = 1;
				markColor[i][j] = markColor[i - 1][j];
				setcolor(markColor[i][j]);
				outtextxy(20 * j + MinX, 20 * i + MinY, "��");
			}
			else {
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20 * j + MinX, 20 * i + MinY, "��");
			}
		}
	}
	//�����㷽��
	setcolor(BLACK);
	for (int j = 0; j < 15; ++j) {
		visit[0][j] = 0;
		outtextxy(20 * j + MinX, MinY, "��");
	}
}

// ���ܣ����·���
void addScore(int lines) {
	//char str[32];
	score += lines * 10;
	sprintf_s(Player[NO].score, 32, "%d", score);
	setcolor(WHITE);
	outtextxy(380, 310, Player[NO].score);
}

//���ܣ����µȼ����ٶ�
void updateGrade() {
	//����10��һ��
	rank = score / 10;
	char str[32];
	sprintf_s(str, 32, "%d", rank);
	setcolor(WHITE);
	outtextxy(380, 405, str);
	//�����ٶ�
	if (speed <= 30) {
		speed = 30;
	}
	else {
		speed = 500 - rank * 50;
	}
}

//���ܣ�����Ƿ������з���
void check() {
	int i, j;
	int clearLines = 0;
	for (i = 29; i >= 0; i--) {
		// ����i����û����
		for (j = 0; j < 15 && visit[i][j]; j++);
		//ִ�е��˴�ʱ�������������
		// 1. ��i��û����������ʾ�п�λ ��ʱ j<15
		// 2. ��i�������ˣ���ʱ j>=15
		if (j >= 15) {
			// ��ʱ����i���Ѿ����ˣ�����Ҫ������i��
			down(i);  
			i++;  // ��Ϊ������ѭ������ i--, ����������i++, ʹ���´�ѭ��ʱ���ٰ���һ�м��һ��
			clearLines++;
		}
	}
	// ���·���
	addScore(clearLines);
	// ���µȼ����ٶ�
	updateGrade();
}




//������
int main() {
	//���ű�������
	mciSendString(_T("open TetrisBGM2.mp3 alias mymusic"), NULL, 0, NULL);
	//mciSendString(_T("play mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	int welcomeRet;    //welcome( ) �� return��1������Ϸ���棬0�������а����
	while (1) {
		welcomeRet = welcome();
		if (!welcomeRet) {
			WhetherRankListShow();
			RANK_SAVE();
			//printf("%d", show);
			//system("pause");
			//Sleep(5000);
		}else {
			show = 0;
			initGameScene();
			//�����·���
			nextblock();
			//	system("pause");
			Sleep(800);
			//timer(0.8);
			//��ʼ����������
			memset(visit, 0, sizeof(visit));
			while (1) {
				newblock();
				//�������У������·������ٶ�
				check();
			}
			//printf("HELLO HELLO");
			//system("pause");
			//Sleep(5000);
			//_getch();
			//closegraph();
		}
	}
	
		return 0;
}
