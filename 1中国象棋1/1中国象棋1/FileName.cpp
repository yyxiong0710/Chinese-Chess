
#include <graphics.h>
#include <vector>
#include <conio.h>
#include <string>
#include <math.h>
#include <stack>
using namespace std;

#define Row 10
#define Col 9
#define MaxContain 6
int Width = 600;
int startx = 50;
int starty = 30;
float unitwidth = (Width - 2 * startx) / (Col - 1);
int unitheight = unitwidth;
int Height = (Row - 1) * unitheight + 2 * starty;
int chessradius = unitwidth / 2.3;
bool redplay = true;
enum {
	fuck, car, horse, king, elephant, cannon, soldier, protector
};
struct Situation {
	int chessboard[Row][Col];
	bool redplay;
};
Situation sit;
stack<Situation>  backsit;
const int startchessboard[Row][Col] = {
	{-car,-horse,-elephant,-protector,-king,-protector,-elephant,-horse,-car},
	{0,0,0,0,0,0,0,0,0},
	{0,-cannon,0,0,0,0,0,-cannon,0},
	{-soldier,0,-soldier,0,-soldier,0,-soldier,0,-soldier},
	{0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0},
	{soldier,0,soldier,0,soldier,0,soldier,0,soldier},
	{0,cannon,0,0,0,0,0,cannon,0},
	{0,0,0,0,0,0,0,0,0},
	{car,horse,elephant,protector,king,protector,elephant,horse,car},
};

string blackcode[8] = { "b","车","马","将","象","炮","卒","士" };
string redcode[8] = { "b","车","马", "帅", "相", "炮", "兵", "士" };

int endgame = 0;
int choose = 0;
int ischoose[2] = { -1,-1 };

void startupchessboard(Situation* a) {
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			a->chessboard[i][j] = startchessboard[i][j];
		}
	}
	a->redplay = true;
}

void startup() {
	initgraph(Width, Height);
	setbkmode(TRANSPARENT);
	setbkcolor(RGB(255, 220, 100));
	cleardevice();
	startupchessboard(&sit);
	BeginBatchDraw();
}

void showchessboard() {
	setlinecolor(BLACK);
	for (int i = 0; i < Col; i++) {
		float x = startx + i * unitwidth;
		if (i == 0 || i == Col - 1) {
			line(x, starty, x, Height - starty);
		}
		else {
			line(x, starty, x, starty + (Row / 2 - 1) * unitheight);
			line(x, Height - starty, x, Height - starty - (Row / 2 - 1) * unitheight);
		}
	}
	for (int i = 0; i < Row; i++) {
		int y = starty + i * unitheight;
		line(startx, y, Width - startx - 4, y);
	}

	line(startx + 3 * unitwidth, starty, Width - startx - 3 * unitwidth - 4, starty + 2 * unitheight);
	line(startx + 3 * unitwidth, starty + 2 * unitheight, Width - startx - 3 * unitwidth - 4, starty);

	line(startx + 3 * unitwidth, Height - starty - 2 * unitheight, Width - startx - 3 * unitwidth - 4, Height - starty);
	line(startx + 3 * unitwidth, Height - starty, Width - startx - 3 * unitwidth - 4, Height - starty - 2 * unitheight);

	settextstyle(0.6 * unitwidth, 0, _T("宋体"));
	settextcolor(BLACK);
	RECT r = { startx,starty + 4 * unitheight,Width - startx,starty + 5 * unitheight };
	drawtext(_T("楚河            汉界"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void showchess(int x, int y, int id, bool isred) {
	setlinecolor(BLACK);
	setfillcolor(WHITE);
	fillcircle(x, y, chessradius);
	settextstyle(0.5 * unitwidth, 0, _T("宋体"));
	settextcolor(isred ? RED : BLACK);
	RECT r = { x - chessradius,y - chessradius,x + chessradius,y + chessradius };
	if (isred) {
		switch (id) {
		case car:
			drawtext(_T("车"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case horse:
			drawtext(_T("马"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case king:
			drawtext(_T("帅"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case elephant:
			drawtext(_T("相"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case cannon:
			drawtext(_T("炮"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case protector:
			drawtext(_T("士"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case soldier:
			drawtext(_T("兵"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		default:
			break;
		}
	}
	else {
		switch (id) {
		case car:
			drawtext(_T("车"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case horse:
			drawtext(_T("马"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case king:
			drawtext(_T("将"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case elephant:
			drawtext(_T("象"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case cannon:
			drawtext(_T("炮"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case protector:
			drawtext(_T("士"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		case soldier:
			drawtext(_T("卒"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			break;
		default:
			break;
		}
		/*string s1 = blackcode[id];
		RECT r = { x - chessradius,y - chessradius,x + chessradius,y + chessradius };
		TCHAR s[10];
		swprintf_s(s, _T("%s"), "a");
		drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
	}
}

void showallchess() {
	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Col; j++) {
			int x = startx + j * unitwidth;
			int y = starty + i * unitheight;
			bool isred = true;
			int chessval = sit.chessboard[i][j];
			if (sit.chessboard[i][j] < 0) {
				isred = false;
				chessval = -chessval;
			}
			if (sit.chessboard[i][j] != 0) {
				showchess(x, y, chessval, isred);
			}
			if (ischoose[0] != -1) {
				int x = startx + ischoose[1] * unitwidth;
				int y = starty + ischoose[0] * unitheight;
				setlinecolor(GREEN);
				rectangle(x - chessradius - 2, y - chessradius - 2, x + chessradius + 2, y + chessradius + 2);
			}
		}
	}
}

void showwhoplay() {
	COLORREF c = sit.redplay ? RED : BLACK;
	setfillcolor(c);
	setlinecolor(c);
	int y = starty + (4.5) * unitheight;
	int x = Width - 0.5 * startx;
	fillrectangle(x - 0.2 * unitwidth, y - 0.2 * unitheight, x + 0.2 * unitwidth, y + 0.2 * unitheight);
}

void showend() {
	if (endgame == 1) {
		settextstyle(1.5 * unitwidth, 0, _T("宋体"));
		settextcolor(BLACK);
		RECT r = { 0,0,Width,Height };
		drawtext(_T("黑方胜利"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	else if (endgame == -1) {
		settextstyle(1.5 * unitwidth, 0, _T("宋体"));
		settextcolor(RED);
		RECT r = { 0,0,Width,Height };
		drawtext(_T("红方胜利"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
}

void show() {
	cleardevice();
	showchessboard();
	showallchess();
	showend();
	showwhoplay();
	FlushBatchDraw();
}

int* min_c(int coordinate[][2], int x, int y) {
	int d;
	int min;
	for (int i = 0; i < 4; i++) {
		if (i == 0) {
			d = (x - (coordinate[i][0] * unitwidth + startx)) * (x - (coordinate[i][0] * unitwidth + startx)) + (y - (coordinate[i][1] * unitheight + starty)) * (y - (coordinate[i][1] * unitheight + starty));
			min = i;
		}
		else {
			if (d > (x - (coordinate[i][0] * unitwidth + startx)) * (x - (coordinate[i][0] * unitwidth + startx)) + (y - (coordinate[i][1] * unitheight + starty)) * (y - (coordinate[i][1] * unitheight + starty))) {
				d = (x - (coordinate[i][0] * unitwidth + startx)) * (x - (coordinate[i][0] * unitwidth + startx)) + (y - (coordinate[i][1] * unitheight + starty)) * (y - (coordinate[i][1] * unitheight + starty));
				min = i;
			}
		}
	}
	return coordinate[min];
}

bool issamequality(int a, int b) {
	if (a < 0 && b < 0 || a>0 && b>0) {
		return true;
	}
	return false;
}

bool carmove(int x, int y, int go_x, int go_y, bool isred) {
	int val;
	if (isred) {
		val = 1;
	}
	else {
		val = -1;
	}

	if (go_x != x && go_y != y) {
		return false;
	}
	else if (go_x == x) {
		if (go_y > y) {
			for (int i = y + 1; i < go_y; i++) {
				if (sit.chessboard[i][x] != 0) {
					return false;
				}
			}
			if (issamequality(sit.chessboard[go_y][x], val)) {
				return false;
			}
			else {
				return true;
			}
		}
		else if (go_y < y) {
			for (int i = y - 1; i > go_y; i--) {
				if (sit.chessboard[i][x] != 0) {
					return false;
				}
			}
			if (issamequality(sit.chessboard[go_y][x], val)) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	else if (go_y == y) {
		if (go_x > x) {
			for (int i = x + 1; i < go_x; i++) {
				if (sit.chessboard[y][i] != 0) {
					return false;
				}
			}
			if (issamequality(sit.chessboard[y][go_x], val)) {
				return false;
			}
			else {
				return true;
			}
		}
		else if (go_x < x) {
			for (int i = x - 1; i > go_x; i--) {
				if (sit.chessboard[y][i] != 0) {
					return false;
				}
			}
			if (issamequality(sit.chessboard[y][go_x], val)) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	return false;
}

bool horsemove(int x, int y, int go_x, int go_y, bool isred) {
	int val;
	if (isred) {
		val = 1;
	}
	else {
		val = -1;
	}

	if ((abs(x - go_x) == 1) && (abs(y - go_y) == 2) || (abs(x - go_x) == 2) && (abs(y - go_y) == 1)) {
		if ((abs(x - go_x) == 1) && (abs(y - go_y) == 2)) {
			if (go_y < y) {
				if (sit.chessboard[y - 1][x] != 0) {
					return false;
				}
			}
			else if (go_y > y) {
				if (sit.chessboard[y + 1][x] != 0) {
					return false;
				}
			}
		}
		else {
			if (go_x > x) {
				if (sit.chessboard[y][x + 1] != 0) {
					return false;
				}
			}
			else if (go_x < x) {
				if (sit.chessboard[y][x - 1] != 0) {
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

bool kingmove(int x, int y, int go_x, int go_y, bool isred) {
	if (go_x >= 3 && go_x <= 5) {
		if (isred && go_y >= Row - 3 && go_y <= Row - 1 || !isred && go_y >= 0 && go_y <= 2) {
			if (abs(go_x - x) == 1 && go_y == y || abs(go_y - y) == 1 && go_x == x) {
				return true;
			}
		}
	}

	return false;
}

bool soldiermove(int x, int y, int go_x, int go_y, bool isred) {
	if (isred) {
		if (y >= 5 && y - go_y == 1) {
			return true;
		}
		else if ((y < 5) && (y - go_y == 1 || abs(x - go_x) == 1)) {
			return true;
		}
	}
	else {
		if (y < 5 && go_y - y == 1) {
			return true;
		}
		else if (y >= 5 && (go_y - y == 1 || abs(x - go_x) == 1)) {
			return true;
		}
	}

	return false;
}

bool elephantmove(int x, int y, int go_x, int go_y, bool isred) {
	if (abs(go_x - x) == 2 && abs(go_y - y) == 2) {
		if (go_x < x) {
			if (go_y < y) {
				if (sit.chessboard[y - 1][x - 1] == 0) {
					return true;
				}
			}
			else if (go_y > y) {
				if (sit.chessboard[y + 1][x - 1] == 0) {
					return true;
				}
			}
		}
		else if (go_x > x) {
			if (go_y < y) {
				if (sit.chessboard[y - 1][x + 1] == 0) {
					return true;
				}
			}
			else if (go_y > y) {
				if (sit.chessboard[y + 1][x + 1] == 0) {
					return true;
				}
			}
		}
	}

	return false;
}

bool protectormove(int x, int y, int go_x, int go_y, bool isred) {
	if (go_x >= 3 && go_x <= 5) {
		if (isred && go_y >= Row - 3 && go_y <= Row - 1 || !isred && go_y >= 0 && go_y <= 2) {
			if (abs(go_x - x) == 1 && abs(go_y - y) == 1 || abs(go_y - y) == 1 && abs(go_x - x) == 1) {
				return true;
			}
		}
	}

	return false;
}

bool cannonmove(int x, int y, int go_x, int go_y, bool isred) {
	if (go_x != x && go_y != y) {
		return false;
	}

	if (go_x == x) {
		if (sit.chessboard[go_y][go_x] == 0) {
			if (go_y < y) {
				for (int i = y - 1; i > go_y; i--) {
					if (sit.chessboard[i][x] != 0) {
						return false;
					}
				}
			}
			else if (go_y > y) {
				for (int i = y + 1; i < go_y; i++) {
					if (sit.chessboard[i][x] != 0) {
						return false;
					}
				}
			}
			return true;
		}
		else {
			int cnt = 0;
			if (go_y < y) {
				for (int i = y - 1; i > go_y; i--) {
					if (sit.chessboard[i][x] != 0) {
						cnt++;
					}
				}
			}
			else if (go_y > y) {
				for (int i = y + 1; i < go_y; i++) {
					if (sit.chessboard[i][x] != 0) {
						cnt++;
					}
				}
			}
			if (cnt == 1) {
				return true;
			}
		}
	}
	else if (go_y == y) {
		if (sit.chessboard[go_y][go_x] == 0) {
			if (go_x < x) {
				for (int i = x - 1; i > go_x; i--) {
					if (sit.chessboard[y][i] != 0) {
						return false;
					}
				}
			}
			else if (go_x > x) {
				for (int i = x + 1; i < go_x; i++) {
					if (sit.chessboard[y][i] != 0) {
						return false;
					}
				}
			}
			return true;
		}
		else {
			int cnt = 0;
			if (go_x < x) {
				for (int i = x - 1; i > go_x; i--) {
					if (sit.chessboard[y][i] != 0) {
						cnt++;
					}
				}
			}
			else if (go_x > x) {
				for (int i = x + 1; i < go_x; i++) {
					if (sit.chessboard[y][i] != 0) {
						cnt++;
					}
				}
			}
			if (cnt == 1) {
				return true;
			}
		}
	}
	return false;
}

bool chessmove(int id, int x, int y, int go_x, int go_y, bool isred) {

	switch (id) {

	case car:
		if (carmove(x, y, go_x, go_y, isred)) return true;
		break;

	case horse:
		if (horsemove(x, y, go_x, go_y, isred)) return true;
		break;

	case king:
		if (kingmove(x, y, go_x, go_y, isred)) return true;
		break;
	case elephant:
		if (elephantmove(x, y, go_x, go_y, isred)) return true;
		break;
	case cannon:
		if (cannonmove(x, y, go_x, go_y, isred)) return true;
		break;
	case protector:
		if (protectormove(x, y, go_x, go_y, isred)) return true;
		break;
	case soldier:
		if (soldiermove(x, y, go_x, go_y, isred)) return true;
		break;
	default:
		break;
	}
}

int isend() {
	bool redking = false;
	bool blackking = false;
	for (int i = 3; i <= 5; i++) {
		for (int j = 0; j <= 2; j++) {
			if (sit.chessboard[j][i] == -king) {
				blackking = true;
			}
		}
	}
	for (int i = 3; i <= 5; i++) {
		for (int j = Row - 1; j >= Row - 3; j--) {
			if (sit.chessboard[j][i] == king) {
				redking = true;
			}
		}
	}

	if (!redking) {
		return 1;
	}
	else if (!blackking) {
		return -1;
	}
	return 0;
}

//回撤
void withdraw() {
	if (backsit.empty()) {
		return;
	}
	ischoose[0] = -1;
	sit = backsit.top();
	backsit.pop();
}

void savechessboard(Situation s) {
	backsit.push(s);
}

void update() {
	MOUSEMSG m;
	if (MouseHit()) {
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {
			int jx = (m.x - startx) / unitwidth;
			int iy = (m.y - starty) / unitheight;
			if (jx < 0 || iy < 0 || jx >= Col || iy >= Row) {
				return;
			}
			int near_coordinate[4][2];
			near_coordinate[0][0] = jx;
			near_coordinate[0][1] = iy;
			near_coordinate[1][0] = jx + 1;
			near_coordinate[1][1] = iy;
			near_coordinate[2][0] = jx;
			near_coordinate[2][1] = iy + 1;
			near_coordinate[3][0] = jx + 1;
			near_coordinate[3][1] = iy + 1;

			int* smallest_coordinate = min_c(near_coordinate, m.x, m.y);

			int d = (m.x - (smallest_coordinate[0] * unitwidth + startx)) * (m.x - (smallest_coordinate[0] * unitwidth + startx)) + (m.y - (smallest_coordinate[1] * unitheight + starty)) * (m.y - (smallest_coordinate[1] * unitheight + starty));
			if (d <= chessradius * chessradius) {
				int sx = smallest_coordinate[0];
				int sy = smallest_coordinate[1];
				if (sit.redplay) {
					if (sit.chessboard[sy][sx] > 0) {
						choose = sit.chessboard[sy][sx];
						ischoose[0] = sy;
						ischoose[1] = sx;
					}
					else if (choose != 0 && sit.chessboard[sy][sx] <= 0) {
						if (chessmove(choose, ischoose[1], ischoose[0], sx, sy, sit.redplay)) {
							savechessboard(sit);
							sit.chessboard[sy][sx] = choose;
							sit.chessboard[ischoose[0]][ischoose[1]] = 0;

							choose = 0;
							sit.redplay = !sit.redplay;
							ischoose[0] = -1;
						}
					}
				}
				else {
					if (sit.chessboard[sy][sx] < 0) {
						choose = sit.chessboard[sy][sx];
						ischoose[0] = sy;
						ischoose[1] = sx;
					}
					else if (choose != 0 && sit.chessboard[sy][sx] >= 0) {
						if (chessmove(-choose, ischoose[1], ischoose[0], sx, sy, sit.redplay)) {
							savechessboard(sit);
							sit.chessboard[sy][sx] = choose;
							sit.chessboard[ischoose[0]][ischoose[1]] = 0;

							choose = 0;
							sit.redplay = !sit.redplay;
							ischoose[0] = -1;
						}
					}
				}
			}

		}
		else if (m.uMsg == WM_RBUTTONDOWN) {
			withdraw();
		}
	}
}

int main() {
	startup();
	while (1) {
		show();
		if (endgame != 0) {
			break;
		}
		update();
		endgame = isend();
	}
	_getch();
	return 0;
}