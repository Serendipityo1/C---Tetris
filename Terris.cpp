#include"stdio.h"
#include"time.h"
#include"conio.h"	//kbhit()
#include"graphics.h"
#include"stdlib.h"
#include"string.h"
#pragma comment(lib,"winmm.lib")

int NO=0 ;
int score = 0;	//总分
int rank = 0;	//等级
int show = 0; //排行榜变量，0不显示，1显示
#define BLOCK_COUNT 5
#define BLOCK_WIDTH 5
#define BLOCK_HEIGHT 5

#define UNIT_SIZE 20	//小方块宽度

#define START_X 130		//方块降落框，方块降落起始位置
#define START_Y 30

#define KEY_UP 87		//用户操作
#define KEY_LEFT 65
#define KEY_RIGHT 68
#define KEY_DOWN 83
#define KEY_SPACE 32
#define UP 72
#define LEFT 75
#define RIGHT 77
#define DOWN 80


#define MinX 30		//游戏左上角位置
#define MinY 30

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000    //计时器一秒
#endif
			 
int speed = 500;	//方块降落速度

int NextIndex = -1;		//下一个方块
int BlockIndex = -1;		//当前方块

struct PLAYER
{
	char  name[50];
	char score[20];
};
struct PLAYER Player[100];
struct PLAYER Rank_Player[100];

typedef enum {		//方块朝向
	BLOCK_UP,
	BLOCK_RIGHT,
	BLOCK_LEFT,
	BLOCK_DOWN
}block_dir_t;

typedef enum {		//方块移动方向
	MOVE_DOWN,
	MOVE_LEFT,
	MOVE_RIGHT
}move_dir_t;

//方块颜色
int color[BLOCK_COUNT] = {
	GREEN,
	CYAN,
	MAGENTA,
	YELLOW,
	BROWN
};
int visit[30][15];	//访问数组visit[i][j] = 1表示该位置有方块
int markColor[30][15];	//对应位置颜色
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
   // 田
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

//功能：欢迎页面
int welcome() {
	//初始化画布
	initgraph(550, 660);
	//设置窗口
	HWND window = GetHWnd();
	SetWindowText(window, _T("Tetris"));
	//欢迎界面
	IMAGE picture;
	loadimage(&picture, "D:\\Curriculum\\C\\ClassDesign\\Tetris.jpg", 550, 660);
	putimage(0, 0, &picture);
	
	//鼠标
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
	//延时3秒
	//Sleep(3000);
}
//输入用户名字
void Inputname() {
	InputBox(Player[NO].name, 50, "Please Input Your Name(In English):");
	//_getch(); //按任意键关闭欢迎界面
	closegraph;
}


// 功能：初始化游戏场景
void initGameScene() {
	Inputname();
	char str[16];	//存放分数
	//1.清屏
	cleardevice();
	//2.画场景
	rectangle(29, 29, 340, 635);	//游戏框
	rectangle(370, 60, 515, 205);	//提示框
	//Next
	setfont(24, 0, _T("宋体"));		
	setcolor(WHITE);
	outtextxy(370, 30, _T("Next："));
	//Score
	setcolor(WHITE);					
	outtextxy(370, 280, _T("Score："));
	     //将score写入str
	sprintf_s(str, 16, "%d", score);
	outtextxy(380, 310, str);
	//Rank
	outtextxy(370, 375, _T("Rank："));
		//将rank写入str
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


 // 功能：清空方块提示框里的方块
void clearBlock() {
	setcolor(BLACK);
	setfont(23, 0, "楷体");
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			int x = 390 + j * UNIT_SIZE;
			int y = 70 + i * UNIT_SIZE;
			outtextxy(x, y, "■");
		}
	}
}


//功能：清除降落过程中的方块
void clearBlock(int x, int y, block_dir_t blockDir) {
	setcolor(BLACK);
	//	setfont(23, 0, "楷体");
	int id = BlockIndex * 4 + blockDir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[id][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "■");
			}
		}
	}
}

// 功能：在提示框 与 降落框的起始位置画方块
void drawBlock(int x, int y) {
	setcolor(color[NextIndex]);
	setfont(23, 0, "楷体");
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[NextIndex * 4][i][j] == 1) {
				int x2 = x + j * UNIT_SIZE;
				int y2 = y + i * UNIT_SIZE;
				outtextxy(x2, y2, "■");
			}
		}
	}
}

//功能：绘制下降过程中的方块
void drawBlock(int x, int y, block_dir_t dir) {
	setcolor(color[BlockIndex]);
	setfont(23, 0, "楷体");
	int id = BlockIndex * 4 + dir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			if (block[id][i][j] == 1) {
				//擦除该方块的第i行第j列
				outtextxy(x + j * UNIT_SIZE, y + i * UNIT_SIZE, "■");
			}
		}
	}
}

//功能：方块提示框中产生新方块
void nextblock() {
	clearBlock();
	//产生随机数，随机选择方块
	srand((unsigned)time(NULL));	//使用时间函数的返回值，来作为随机种子
	NextIndex = rand() % BLOCK_COUNT;	//产生0~5的随机数
	drawBlock(390, 70);
}

//功能：判断方块能否向指定方向移动
bool moveable(int x0, int y0, move_dir_t moveDir, block_dir_t blockDir) {
	//计算方块左上角在30×15的游戏区位置（第多少行， 第多少列）
	int x = (y0 - MinY) / UNIT_SIZE;
	int y = (x0 - MinX) / UNIT_SIZE;
	int ret = 1;
	int id = BlockIndex * 4 + blockDir;
	if (moveDir == MOVE_DOWN) {
		for (int i = 0; i < BLOCK_HEIGHT; ++i) {
			for (int j = 0; j < BLOCK_WIDTH; ++j) {
				//向下不能运动的条件：实心方块已经达到底部（x+i+1==30），或者底部已有方块	
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
				//向左不能运动的条件：实心方块已经达到左边界(y+j==0),或者左边已有方块
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
				//向下不能运动的条件：实心方块已经达到右边界（y+j+1>=15)，或者右边已有方块
				if (block[id][i][j] == 1 &&
					(y + j + 1 >= 15 || visit[x + i][y + j + 1] == 1)) {
					ret = 0;
				}
			}
		}
	}
	return ret;
}
//功能：排行榜界面是否显示
void WhetherRankListShow() {
	int* p;
	p = &show;
	*p = 1;
}

//功能：1.排行榜界面绘制 2.保存并排序用户记录（名字+分数）
void RANK_SAVE(){
	FILE* fp;
	if (show == 1) {  //排行榜界面显示
		initgraph(550, 660);
		HWND window = GetHWnd();
		SetWindowText(window, _T("RankList"));
		//文件内容显示
		int i = 0;
		//文件读入
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
		//显示
		setfont(24, 0, _T("宋体"));
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

		//再次初始化
		//memset(Player, 0, sizeof(PLAYER));
		//struct PLAYER Player[100] = { };
		NO = 0;
		//_getch();
		//鼠标点击 "< Return"
		
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
		//写入文档
		fprintf(fp, "%s  %s\n", Player[NO].name, Player[NO].score);
		rewind(fp);
		fclose(fp);
		//读入结构体
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
		//排序
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
		//保存
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

//功能：检测游戏是否结束
void failCheck() {
	//游戏结束条件是顶部新被绘制出的方块就要“固化”，顶部新绘制的方块方向朝上，运动方向朝下
	if (!moveable(START_X, START_Y, MOVE_DOWN, (block_dir_t)BLOCK_UP)) {
		setcolor(WHITE);
		setfont(45, 0, "隶体");
		outtextxy(75, 300, "Game Over！");
		RANK_SAVE();
		Sleep(5000);
		//timer(1);
		//system("pause");
		closegraph();
		//exit(0);
	}
}

//功能：延时等待
void wait(int interval) {
	int count = interval / 10;
	for (int i = 0; i < count; ++i) {
		Sleep(10);
		//如果休眠期间用户有按键，则结束休眠
		if (_kbhit()) {
			return;
		}
	}
}

//功能：判断当前方块能否向指定方向旋转
bool rotatable(int x, int y, block_dir_t dir) {
	//首先判断是否可以继续向下移动
	if (!moveable(x, y, MOVE_DOWN, dir)) {
		return false;
	}
	int x2 = (y - MinY) / UNIT_SIZE;
	int y2 = (x - MinX) / UNIT_SIZE;
	int id = BlockIndex * 4 + dir;
	for (int i = 0; i < BLOCK_HEIGHT; ++i) {
		for (int j = 0; j < BLOCK_WIDTH; ++j) {
			//不能旋转条件：左右边界越界或者已有方块“阻挡”
			if (block[id][i][j] == 1 && (y2 + j < 0 || y2 + j >= 15 || visit[x2 + i][y2 + j] == 1)) {
				return false;
			}
		}
	}
	return true;
}

// 功能：判断方块是否还能继续移动后，固定方块
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

// 功能：读取用户操作，时时更新降落的方块
void move() {
	int x = START_X;	//方块起始位置
	int y = START_Y;
	int k = 0;
	block_dir_t blockDir = (block_dir_t)BLOCK_UP;
	//读取用户操作前判断游戏是否结束
	failCheck();
	//持续向下降落
	while (1) {
		int curSpeed = speed;	//定义当前方块降落速度
		//清除方块
		clearBlock(x, k + y, blockDir);
		//判断选择的方向
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
		//绘制方块
		drawBlock(x, y + k, blockDir);
		//休眠
		wait(curSpeed);
		//方块的固化处理，方块固定后结束循环，当前一个方块的move执行完毕
		if (!moveable(x, y + k, MOVE_DOWN, blockDir)) {
			mark(x, y + k, blockDir);
			break;
		}
	}
}

//功能：绘制刚从顶部降落的方块，更新提示框内的方块，调用方块降落函数move()
void newblock() {
	BlockIndex = NextIndex;
	//绘制刚从顶部下降的方块
	drawBlock(START_X, START_Y);
	//让新出现方块暂停一会
	Sleep(200);
	//timer(0.2);
	//右上角区域绘制下一个方块
	nextblock();
	//方块降落
	move();
}

//功能：消除第i行，并把上面的行都往下移
void down(int x) {
	for (int i = x; i > 0; --i) {
		for (int j = 0; j < 15; ++j) {
			if (visit[i - 1][j] == 1) {
				visit[i][j] = 1;
				markColor[i][j] = markColor[i - 1][j];
				setcolor(markColor[i][j]);
				outtextxy(20 * j + MinX, 20 * i + MinY, "■");
			}
			else {
				visit[i][j] = 0;
				setcolor(BLACK);
				outtextxy(20 * j + MinX, 20 * i + MinY, "■");
			}
		}
	}
	//清除最顶层方格
	setcolor(BLACK);
	for (int j = 0; j < 15; ++j) {
		visit[0][j] = 0;
		outtextxy(20 * j + MinX, MinY, "■");
	}
}

// 功能：更新分数
void addScore(int lines) {
	//char str[32];
	score += lines * 10;
	sprintf_s(Player[NO].score, 32, "%d", score);
	setcolor(WHITE);
	outtextxy(380, 310, Player[NO].score);
}

//功能：更新等级和速度
void updateGrade() {
	//假设10分一级
	rank = score / 10;
	char str[32];
	sprintf_s(str, 32, "%d", rank);
	setcolor(WHITE);
	outtextxy(380, 405, str);
	//更新速度
	if (speed <= 30) {
		speed = 30;
	}
	else {
		speed = 500 - rank * 50;
	}
}

//功能：检查是否有满行方块
void check() {
	int i, j;
	int clearLines = 0;
	for (i = 29; i >= 0; i--) {
		// 检查第i行有没有满
		for (j = 0; j < 15 && visit[i][j]; j++);
		//执行到此处时，有两种情况：
		// 1. 第i行没有满，即表示有空位 此时 j<15
		// 2. 第i行已满了，此时 j>=15
		if (j >= 15) {
			// 此时，第i行已经满了，就需要消除第i行
			down(i);  
			i++;  // 因为最外层的循环中有 i--, 所以我们先i++, 使得下次循环时，再把这一行检查一下
			clearLines++;
		}
	}
	// 更新分数
	addScore(clearLines);
	// 更新等级和速度
	updateGrade();
}




//主函数
int main() {
	//播放背景音乐
	mciSendString(_T("open TetrisBGM2.mp3 alias mymusic"), NULL, 0, NULL);
	//mciSendString(_T("play mymusic"), NULL, 0, NULL);
	mciSendString(_T("play mymusic repeat"), NULL, 0, NULL);
	int welcomeRet;    //welcome( ) 的 return，1进入游戏界面，0进入排行榜界面
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
			//产生新方块
			nextblock();
			//	system("pause");
			Sleep(800);
			//timer(0.8);
			//初始化访问数组
			memset(visit, 0, sizeof(visit));
			while (1) {
				newblock();
				//消除满行，并更新分数和速度
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
