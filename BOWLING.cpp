#include<graphics.h>  //EasyX的图像头文件
#include<math.h>
#include<stdio.h>
#include<conio.h>    //包含输入函数等的头文件
#include<Windows.h>  //交互头文件
#include <stdlib.h>
#include <tchar.h>   //特殊编码头文件
#include<time.h>    //时间头文件
#include<process.h>

# pragma comment(lib,"Winmm.lib")

#define high 600  //画面高700
#define width 750  //画面宽600
#define _CRT_SECURE_NO_WARNINGS

struct player //包含10个玩家的结构体数组
{
	int player_n;  //Player的编号（1到10）
	int tatal_score;    //十轮总分
	int part_score[11];  //每轮的得分
}Player[11];

struct bowling //包含10个保龄球的结构体数组
{
	bool is_up; //是否被打倒
	int bowling_x;  //坐标
	int bowling_y;
}Bowling[11];

int ball_x; int ball_y; //球的坐标
double ball_vx, ball_vy; //球的速度
int radius = 50;  //球的半径
int is_wall = 0;  //球是否进入侧边轨道里
bool pos = 0, pos2 = 0; //球是否被固定位置
int m_x, m_y;//鼠标位置
double sp;   //球随推进的缩小程度
int j;//各人的轮次
int n;//第几名玩家
bool is_skip = 0;   //是否跳过击球


void setup_player()   //初始化玩家信息
{
	for (int i = 1; i <= 10; ++i) {
		Player[i].player_n = i;
		for (int j = 1; j <= 10; ++j) Player[i].part_score[j] = 0;
		Player[i].tatal_score = 0;
	}
}

void setup_bowling()   //还原每个保龄球的位置和立起状态
{
	Bowling[1].is_up = 1; Bowling[1].bowling_x = 340; Bowling[1].bowling_y = 140;
	Bowling[2].is_up = 1; Bowling[2].bowling_x = 360; Bowling[2].bowling_y = 140;
	Bowling[3].is_up = 1; Bowling[3].bowling_x = 380; Bowling[3].bowling_y = 140;
	Bowling[4].is_up = 1; Bowling[4].bowling_x = 400; Bowling[4].bowling_y = 140;
	Bowling[5].is_up = 1; Bowling[5].bowling_x = 350; Bowling[5].bowling_y = 160;
	Bowling[6].is_up = 1; Bowling[6].bowling_x = 370; Bowling[6].bowling_y = 160;
	Bowling[7].is_up = 1; Bowling[7].bowling_x = 390; Bowling[7].bowling_y = 160;
	Bowling[8].is_up = 1; Bowling[8].bowling_x = 360; Bowling[8].bowling_y = 180;
	Bowling[9].is_up = 1; Bowling[9].bowling_x = 380; Bowling[9].bowling_y = 180;
	Bowling[10].is_up = 1; Bowling[10].bowling_x = 370; Bowling[10].bowling_y = 200;
}

void startup()//初始化函数
{
	mciSendString(_T("open background.mp3 alias bkmusic"), NULL, 0, NULL);		//打开背景音乐
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);
	srand((unsigned)time(NULL));  //用时间函数设置伪随机数种子
	setup_player();     //初始化玩家信息
	setup_bowling();    //还原每个保龄球的位置
	initgraph(width, high);//开始界面
	while (1)   //等待空格的输入
	{
		IMAGE pic_bk;
		loadimage(&pic_bk, _T(".\\BKP.png")); //设置背景图
		putimage(0, 0, &pic_bk);   //铺设背景图
		settextstyle(30, 0, _T("黑体"));   //设置字体
		while (!_kbhit())  //等待空格输入
		{
			setbkmode(TRANSPARENT);
			setcolor(YELLOW);
			outtextxy(width * 0.35, high * 0.7, _T("按空格开始游戏！"));
			Sleep(100);
			setcolor(GREEN);
			outtextxy(width * 0.35, high * 0.7, _T("按空格开始游戏！"));
			Sleep(100);
			setcolor(BLACK);
			outtextxy(width * 0.35, high * 0.7, _T("按空格开始游戏！"));
			Sleep(100);
		}
		char input;
		input = _getch();   //得到输入
		if (input == ' ')
		{
			mciSendString(_T("close jpmusic"), NULL, 0, NULL);		//打开开始游戏提示音
			mciSendString(_T("open ready_go.mp3 alias jpmusic"), NULL, 0, NULL);
			mciSendString(_T("play jpmusic"), NULL, 0, NULL);
			break;  //是空格就开始游戏
		}
	}
}

void setup()  //把所有的参数还原
{
	is_wall = 0;
	ball_x = width / 2;
	ball_y = high * 0.85;
	setup_bowling();
	ball_vx = 0;
	ball_vy = 0;
	pos = 0; pos2 = 0;
	radius = 60; sp = 0;
}

void preshow(int i)//第i名玩家准备
{
	is_skip = 0;
	BeginBatchDraw();  //开始作图
	ball_x = width / 2;   //设置球的开始位置
	ball_y = high * 0.85;
	sp = 0;
	IMAGE ball1;
	IMAGE ball2;
	loadimage(&ball1, _T("ball1.png"), 80, 80); //设置背景图
	loadimage(&ball2, _T("ball2.png"), 80, 80); //设置背景图
	IMAGE pic_prebk;
	loadimage(&pic_prebk, _T("Pre.png"), width, high); //设置背景图
	while (1)  //等待读入空格或e键
	{
		putimage(0, 0, &pic_prebk);
		setcolor(YELLOW);
		settextstyle(30, 0, _T("黑体"));
		TCHAR s[5];
		swprintf_s(s, _T("%d"), n);
		while (!_kbhit())  //当没有键盘输入
		{
			settextstyle(30, 0, _T("黑体"));
			setcolor(YELLOW);  //交替显示文字
			outtextxy(width * 0.35, high * 0.4, _T("您是player")); //展示预开始画面
			outtextxy(width * 0.55, high * 0.4, s);
			outtextxy(width * 0.2, high * 0.6, _T("请按空格开始您的保龄球之旅！"));
			Sleep(100);
			setcolor(GREEN);
			outtextxy(width * 0.35, high * 0.4, _T("您是player")); //展示预开始画面
			outtextxy(width * 0.55, high * 0.4, s);
			outtextxy(width * 0.2, high * 0.6, _T("请按空格开始您的保龄球之旅！"));
			Sleep(100);
			setcolor(RED);
			outtextxy(width * 0.35, high * 0.4, _T("您是player")); //展示预开始画面
			outtextxy(width * 0.55, high * 0.4, s);
			outtextxy(width * 0.2, high * 0.6, _T("请按空格开始您的保龄球之旅！"));
			outtextxy(width * 0.2, high * 0.7, _T("如果您想跳过，请按e键！"));
			settextstyle(20, 0, _T("黑体"));
			setcolor(YELLOW);
			outtextxy(width * 0.1, high * 0.8, _T("先用鼠标控制球的位置，确定后点击左键确认位置"));
			outtextxy(width * 0.1, high * 0.85, _T("再用鼠标控制球的方向和速度大小，确定后点击空格开球，祝您好运！"));
			Sleep(100);
			FlushBatchDraw();
		}
		char input;
		input = _getch();
		if (input == ' ') break;  //读到空格进入下个阶段
		if (input == 'e')   //读入e跳过击球过程
		{
			is_skip = 1;
			break;
		}
	}
}

int pause_menu()   //暂停函数
{
	IMAGE pause_bk;
	loadimage(&pause_bk, _T("over_bk.jpg"), width, high); //设置背景图
	putimage(0, 0, &pause_bk);  //设置暂停背景
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(50, 0, _T("黑体"));
	outtextxy(width * 0.3, high * 0.05, L"按q继续游戏");
	outtextxy(width * 0.3, high * 0.13, L"按s退出");
	FlushBatchDraw();
	Sleep(2);
	char input;
	while (_kbhit())  //等待输入
	{
		input = _getch();
		if (input == 'q')  //输入q结束暂停
			return 1;
		else if (input == 's')  //输入s结束程序
			exit(0);
	}
}

void show() //展示画面
{
	IMAGE _bowling, N_bowling;
	loadimage(&_bowling, _T("new_bowling.png"), 20, 60); //设置背景图
	loadimage(&N_bowling, _T("~bowling.jpg"), 20, 60); //设置背景图
	IMAGE pic_gmbk;
	loadimage(&pic_gmbk, _T("GM_bk.png"), width, high); //设置背景图
	putimage(0, 0, &pic_gmbk);
	IMAGE ball1;
	IMAGE ball2;
	IMAGE N_ball;
	if (50 - sp >= 10)
	{
		loadimage(&ball1, _T("ball1.png"), 50 - sp, 50 - sp); //设置背景图
		loadimage(&ball2, _T("ball2.png"), 50 - sp, 50 - sp); //设置背景图
		loadimage(&N_ball, _T("~BALL.jpg"), 50 - sp, 50 - sp); //设置背景遮罩图
	}
	if (ball_y >= 140)  //当球的y坐标大于140才显示（否则已经进入洞中）
	{
		Sleep(1);
		putimage(ball_x - 40, ball_y, &N_ball, NOTSRCERASE);  //设置遮罩图
		putimage(ball_x - 40, ball_y, &ball1, SRCERASE);   //展示背景图
		FlushBatchDraw();
	}
	for (int i = 1; i <= 10; ++i)  //显示十个保龄球
	{
		if (Bowling[i].is_up == 1)  //保龄球没有被打倒才显示
		{
			putimage(Bowling[i].bowling_x, Bowling[i].bowling_y, &N_bowling, NOTSRCERASE);
			putimage(Bowling[i].bowling_x, Bowling[i].bowling_y, &_bowling, SRCERASE);
		}
	}
	TCHAR s[5], scoret[5], score[5];
	swprintf_s(s, _T("%d"), j);  //把轮次，此轮得分，总得分写入字符串
	setcolor(LIGHTGREEN);
	swprintf_s(scoret, _T("%d"), Player[n].part_score[j]);
	swprintf_s(score, _T("%d"), Player[n].tatal_score);
	settextstyle(20, 0, _T("黑体"));
	outtextxy(80 + width * 0.1, 30, _T("当前为第"));  //输出轮次，此轮得分，总得分
	outtextxy(80 + width * 0.1 + 80, 30, s);
	outtextxy(80 + width * 0.1 + 100, 30, _T("轮"));
	outtextxy(80 + width * 0.1 + 130, 30, _T("此轮得分为:"));
	outtextxy(80 + width * 0.1 + 240, 30, scoret);
	outtextxy(80 + width * 0.1 + 270, 30, _T("您的总得分为:"));
	outtextxy(80 + width * 0.1 + 400, 30, score);
	FlushBatchDraw();
}

void update_without_input()  //处理和输入无关的更新
{
	if (pos2 != 1) return;  //只有pos2状态为1才处理这个函数
	if (840 - 2 * ball_x - ball_y >= 0 && is_wall == 0)  //如果掉入左侧轨道，只能沿着轨道走
		is_wall = 1;
	if (ball_y - 2 * ball_x + 720 <= 0 && is_wall == 0)  //如果掉入右侧轨道，只能沿着轨道走
		is_wall = 2;
	if (is_wall == 1)   //掉入左侧轨道后，速度大小方向恒定
	{
		ball_vx = 0.4472;
		ball_vy = 0.8944;
	}
	if (is_wall == 2)  //掉入右侧轨道后，速度大小方向恒定
	{
		ball_vx = -0.4;
		ball_vy = 0.8944;
	}
	ball_x = (ball_x + ball_vx * 7);  //小球位置因为有速度而变化
	ball_y = (ball_y - ball_vy * 7);
	if (pos2 == 1 && rand() % 2 && ball_y + rand() % 200 <= 700) //随着时间与y坐标变化，小球大小缩小
		sp += 1, radius -= 1;
	for (int i = 1; i <= 10; ++i) //判断球与10个保龄球有没有相撞算法
	{
		double dis1 = sqrt(pow(ball_x - Bowling[i].bowling_x, 2) + pow(Bowling[i].bowling_y + 60 - ball_y, 2));
		double dis2 = sqrt(pow(ball_x - Bowling[i].bowling_x - 20, 2) + pow(Bowling[i].bowling_y + 60 - ball_y, 2));
		//记录每个保龄球和球的两个距离
		if (dis1 + 5 <= radius && dis2 + 5 <= radius && Bowling[i].is_up == 1 && rand() % 4 == 1)
			//如果距离小于半径，有1/4的几率保龄球倒下（这个算法保证了接触时间越大，小球被击倒的概率越大）
		{
			Bowling[i].is_up = 0;  //保龄球倒下
			Player[n].part_score[j]++;  //本轮得分和总分都加一
			Player[n].tatal_score++;
		}
	}
}

void lineArrow(int x1, int y1, int x2, int y2)  //绘制箭头函数算法，臂长为12，张角为60°
  //这个算法用到了数学里复数的辐角和模等概念（把坐标映射到复平面里）
{
	setcolor(RED);
	setlinestyle(0, 5);
	line(x1, y1, x2, y2);
	double distance = sqrt((y1 - y2) * (y1 - y2) + (x1 - x2) * (x1 - x2));  //计算距离，即为此复数的模
	double tmpx = double(x1 + (x2 - x1) * (1 - (12 * sqrt(3) / 2) / distance));   //辐角1
	double tmpy = double(y1 + (y2 - y1) * (1 - (12 * sqrt(3) / 2) / distance));   //辐角2
	if (y1 == y2)  //如果箭头是垂直的
	{
		setlinestyle(0, 3);
		line(x2, y2, int(tmpx), int(tmpy + 6));  //绘制箭头的两个臂展
		line(x2, y2, int(tmpx), int(tmpy - 6));
	}
	else
	{
		double k = (double(x2) - double(x1)) / (double(y1) - double(y2)); //斜率
		double increX = 6 / sqrt(k * k + 1);
		double increY = 6 * k / sqrt(k * k + 1);
		line(x2, y2, int(tmpx + increX), int(tmpy + increY));   //绘制箭头的两个臂展
		line(x2, y2, int(tmpx - increX), int(tmpy - increY));
	}
}

void update_with_input()   //处理和输入有关的更新
{
	MOUSEMSG m;
	while (MouseHit())  //如果有鼠标输入
	{
		m = GetMouseMsg();  //记录鼠标输入
		m_x = m.x;
		m_y = m.y;
		if (m.uMsg == WM_MOUSEMOVE && pos == 0 && m.x > 200 && m.x < 580)
			ball_x = m.x;   //球的x坐标随着鼠标变动而变动（x的坐标带有限制）
		if (m.uMsg == WM_LBUTTONDOWN)  //如果左键被按下
			pos = 1;   //固定小球位置
	}
	if (pos2 == 0)  //小球位置固定后
	{
		while (_kbhit())  //等待空格读入
		{
			char input;
			input = _getch();
			if (input == ' ')
			{
				mciSendString(_T("close rmusic"), NULL, 0, NULL);		//打开滚动配音
				mciSendString(_T("open rolling.mp3 alias rmusic"), NULL, 0, NULL);
				mciSendString(_T("play rmusic"), NULL, 0, NULL);
				pos2 = 1;  //小球开始发射，pos2置为1
				double dex = (double)(m_y - 10) - ball_y; //设定小球的速度大小，随箭头长度决定
				double dey = (double)(m_x + 10) - ball_x;
				double len = sqrt(dex * dex + dey * dey);
				if (m_x + 10 == ball_x)  //如果速度垂直
				{
					ball_vx = 0;
					ball_vy = 1;
				}
				else  //设定速度的方向和大小
				{
					double tan_x = fabs((double)(m_y - 10) - ball_y) / fabs((double)(m_x + 10) - ball_x);
					double sin_x = 0.0, cos_x = 0.0; //求出速度的各个三角函数值
					sin_x = tan_x / (sqrt(1 + tan_x * tan_x));
					cos_x = 1.0 / (sqrt(1 + tan_x * tan_x));
					double sign = 1;
					if (m_x < ball_x) sign = -1; //设定小球是向左还是向右走
					ball_vx = cos_x * sign * len / 200;
					ball_vy = sin_x * len / 200;
				}
			}
			else if (input == 'q')
			{
				while (1)
				{
					pause_menu();
					if (pause_menu() == 1)
						break;
				}
			}
		}
		if (pos2 == 1 || pos == 0) return;
		int flag = 0;
		while (MouseHit())  //当有鼠标信息
		{
			m = GetMouseMsg();
			m_x = m.x;
			m_y = m.y;
			lineArrow(ball_x, ball_y, m_x + 10, m_y - 10);//绘制随鼠标变化而变化的箭头
			flag = 1;
		}
		if (flag == 0)  //如果没有鼠标信息，箭头保持不变
			lineArrow(ball_x, ball_y, m_x + 10, m_y - 10);
		FlushBatchDraw();  //输出
	}
}

void mid_set()  //初始化保龄球等部分参数，判断是否第十轮加两场
{
	ball_x = width / 2;
	ball_y = high * 0.85;
	ball_vx = 0;
	ball_vy = 0;
	pos = 0; pos2 = 0;
	radius = 60; sp = 0;
	is_wall = 0;
	if (Player[n].part_score[j] == 10 && j == 10)  //如果第十轮上半场就打了10分，加两场
	{
		setup();
		for (int x = 0; x <= 1; ++x)
		{
			setup();
			while (1)
			{
				show();
				update_without_input();
				update_with_input();
				if (pos2 == 1 && ((ball_vx <= 0.05 && ball_vy <= 0.05) || ball_y <= 161 || radius <= 20)) break;
			}
			mid_set();
			while (1)
			{
				show();
				update_without_input();
				update_with_input();
				if (pos2 == 1 && (ball_vx <= 0.05 && ball_vy <= 0.05 || ball_y <= 161 || radius <= 20)) break;
			}
		}
	}
}

void over_check()   //如果第十轮第二场一共打了10分，加1场
{
	if (Player[n].part_score[j] == 10 && j == 10)  //如果第十轮打了10分，加1场
	{
		setup();
		while (1)
		{
			show();
			update_without_input();
			update_with_input();
			if (pos2 == 1 && ((ball_vx <= 0.05 && ball_vy <= 0.05) || ball_y <= 161 || radius <= 20)) break;
		}
		mid_set();
		while (1)
		{
			show();
			update_without_input();
			update_with_input();
			if (pos2 == 1 && (ball_vx <= 0.05 && ball_vy <= 0.05 || ball_y <= 161 || radius <= 20)) break;
		}
	}
}

void gameover()    //游戏结束的输出
{
	if (is_skip == 1) Player[n].tatal_score = ((rand()) * n) % 50 + 50; //如果跳过击球过程，给出随机分数
	while (1)
	{
		BeginBatchDraw();
		IMAGE over_bk;
		loadimage(&over_bk, _T("over_bk.jpg"), width, high); //设置背景图
		putimage(0, 0, &over_bk);
		TCHAR s[10];
		swprintf_s(s, _T("%d"), Player[n].tatal_score);  //把玩家的总得分输入字符串
		settextstyle(30, 0, _T("黑体"));
		setcolor(GREEN);
		outtextxy(180, high / 2, _T("您的总得分为:"));  //输出玩家的总得分
		outtextxy(400, high / 2, s);
		if (n != 10)
			outtextxy(180, high / 2 + 30, _T("按空格以继续下一位玩家！"));
		else
			outtextxy(180, high / 2 + 30, _T("按空格以显示所有玩家得分！"));
		FlushBatchDraw();
		if (_kbhit())   //等待空格输入
		{
			char input = _getch();
			if (input == ' ')
				break;
		}
	}
}

void show_score()  //展示所有人的分数
{
	while (1)
	{
		BeginBatchDraw();
		IMAGE bk;
		loadimage(&bk, _T("score_bk.jpg"), width, high); //设置背景图
		setcolor(YELLOW);
		settextstyle(30, 0, _T("黑体"));
		putimage(0, 0, &bk);
		outtextxy(180, 30, _T("按空格进入最后的颁奖界面！"));
		for (int i = 1; i <= 10; ++i)  //输出排序后的各人分数
		{
			TCHAR s[5];
			TCHAR st[5];
			swprintf_s(s, _T("%d"), Player[i].player_n);
			swprintf_s(st, _T("%d"), Player[i].tatal_score);
			outtextxy(200 + 50 + 180, 30 + 40 * i, _T("Player"));
			outtextxy(200 + 50 + 250 + 20, 30 + 40 * i, s);
			outtextxy(200 + 50 + 280 + 50, 30 + 40 * i, st);
			outtextxy(200 + 50 + 310 + 50, 30 + 40 * i, _T("分"));
		}
		FlushBatchDraw();
		if (_kbhit())  //等待空格的读入
		{
			char input = _getch();
			if (input == ' ')  break;
		}
	}
}

void show_podium() //展示颁奖画面
{
	while (1)
	{
		mciSendString(_T("close bkmusic"), NULL, 0, NULL);			//打开获奖音乐
		mciSendString(_T("open prize.mp3 alias spmusic"), NULL, 0, NULL);
		mciSendString(_T("play spmusic repeat"), NULL, 0, NULL);
		IMAGE bk;
		loadimage(&bk, _T("podium.jpg"), width, high); //设置背景图
		setcolor(BLACK);
		settextstyle(30, 0, _T("黑体"));
		outtextxy(180, 30, _T("按空格退出！"));
		putimage(0, 0, &bk);
		settextcolor(YELLOW);
		settextstyle(40, 0, _T("黑体"));
		TCHAR s[5];
		for (int i = 1; i < 4; i++)  //仅输出前三名
		{
			swprintf_s(s, _T("%d"), Player[i].player_n); //把玩家的编号转为字符串
			if (i == 1) {
				outtextxy(width * 0.4, high * 0.55, _T("player"));
				outtextxy(width * 0.6, high * 0.55, s);
			}
			else if (i == 2) {
				outtextxy(width * 0.1, high * 0.65, _T("player"));
				outtextxy(width * 0.3, high * 0.65, s);
			}
			else {
				outtextxy(width * 0.65, high * 0.75, _T("player"));
				outtextxy(width * 0.85, high * 0.75, s);
			}
		}
		FlushBatchDraw();
		if (_kbhit())//等待空格输入
		{
			char input = _getch();
			if (input == ' ')  break;
		}
	}
}

int cmp(const void* a, const void* b)  //在qsort内部的结构体排序函数
{
	int mark1 = ((struct player*)a)->tatal_score;
	int mark2 = ((struct player*)b)->tatal_score;
	return mark1 < mark2 ? 1 : -1;
}

void rwtxt()  //把数据写入文件
{
	FILE* fp;				//指向打开文件
	int i, error;
	error = fopen_s(&fp, "demo.txt", "w+");
	if (error != 0) {
		exit(0);
	}
	for (i = 1; i <= 10; i++)
	{
		fprintf(fp, "player%d : %d\n", Player[i].player_n, Player[i].tatal_score);//写入得分并覆盖先前的无用数据
	}
}

int main()
{
	startup();  //先初始化与游戏开始界面
	for (n = 1; n <= 10; ++n)  //10个玩家依次开始
	{
		BeginBatchDraw();
		preshow(n);  //玩家准备
		if (is_skip == 1)  //如果跳过击球
		{
			gameover();
			continue;
		}
		BeginBatchDraw();
		for (j = 1; j <= 10; ++j) //10轮依次开始
		{
			setup();  //初始化保龄球等参数
			while (1)  //开始前半轮
			{
				show();
				update_without_input();
				update_with_input();
				if (pos2 == 1 && ((ball_vx <= 0.05 && ball_vy <= 0.05) || ball_y <= 161 || radius <= 20)) break;
				//当球大小或速度过小，或球进洞，此轮结束
			}
			mid_set(); //初始化保龄球等参数，判断是否第十轮加两场
			if (Player[n].part_score[j] == 10) continue; //如果第一轮就打完了，第二轮跳过
			while (1) //开始第二轮
			{
				show();
				update_without_input();
				update_with_input();
				if (pos2 == 1 && (ball_vx <= 0.05 && ball_vy <= 0.05 || ball_y <= 161 || radius <= 20)) break;
				//当球大小或速度过小，或球进洞，此轮结束
			}
			over_check();//如果第十轮第二场一共打了10分，加1场
		}
		gameover(); //输出玩家的此轮得分
	}
	qsort(Player + 1, 10, sizeof(struct player), cmp);  //对玩家分数排序
	rwtxt();
	show_score();  //展示玩家分数
	mciSendString(_T("close bkmuisc"), NULL, 0, NULL);
	show_podium();  //颁奖函数
	EndBatchDraw();  //结束作图
	closegraph();
	return 0;
}