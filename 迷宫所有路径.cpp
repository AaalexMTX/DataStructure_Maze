//√  1.查找末端链表的前一个节点 的函数 -- 实现回溯使用尾插尾删（速度可能慢
// -  2.头插头删 考虑如何在dfs中将  1.逆序链表输出 并 2.打印路线
//√  3给d加上方向指示的作用
//√  4记录迷宫长度
//√  5手动输入迷宫
//√  6.文件读写 记录/迷宫
//    文件读入中 迷宫的大小匹配 
//√ 迷宫最短路径最优解
//√  输入无解迷宫的反馈


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<queue>
using namespace std;
#define N 30
int m, n,total;		//记录迷宫长、宽、解的总长度
int stx, sty, edx, edy;		//起点 终点坐标
char Maze[N][N];	//记录迷宫地图
int repeatMaze[N][N];	//记录访问记录
int dx[4] = { 0,1,0,-1 };	
int dy[4] = { 1,0,-1,0 };	//搜索方向
//链表记录路径
typedef struct LNode {
	int x, y, length;
	string d;
	struct LNode *next;
}LN, *linkList;
LNode BfsPath[N][N];	//BFS中用于回溯路径
queue<LNode> BfsQueue;	//BFS队列

bool connectCheck = false;	//迷宫联通性检测的标记
//尾删
void tailDelete(linkList h) {
	if (h ==NULL || h->next == NULL) {
		return;
	}else {
		linkList prev = NULL;
		linkList tail = h;
		while (tail->next != NULL) {
			prev = tail;
			tail = tail->next;
		}
		prev->next = NULL;
		free(tail);
	}	//双指针找到末端节点的前一个节点
}
//尾插
void tailInsert(linkList h, linkList EndNode) {
	if (h == NULL) { h = EndNode; }
	linkList p = h;
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = EndNode;
	//free(EndNode);  加了就报错？？？
}
//dfs输出迷宫路径
void printPath(linkList h) {
	linkList CIR = new LNode();	//遍历链表找是否有 解路径节点
	for (int i = 1; i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			bool checkNode = false;	//确定路径地图的打印内容（方向箭头还是0/1）
			CIR = h;
			while (CIR != NULL) {
				if (CIR->x == i && CIR->y == j) {
					checkNode = true;
					break;
				}
				CIR = CIR->next;
			}
			if (checkNode) {
				cout << CIR->d;		//若该点为在解的路径上 打印箭头
			}
			else { 
				cout << Maze[i][j] << " ";
			}
		}
		cout << endl;
	}
}
//输出三元组
void printTrituple(linkList h) {
	if (h == NULL ) { return; }
	linkList s = h; 
	cout << "该路径长度为" << h->length << endl;
	while (s->next != NULL) {
		cout << "(" << s->x << "," << s->y << "," << s->d << ")--";
		s = s->next;
	}
	cout << "(" << s->x << "," << s->y << "," << s->d << ")"<<endl; 
}
//深搜所有路径 -- 尾插尾删
void dfsMaze(int x, int y, linkList h, int i) {
	string d;
	repeatMaze[x][y] = 1;	//专门用来将起点置为已访问
	if (x == edx && y == edy) {
		for (int i = 2;i < n;i += 2) { cout << " "; }
		cout << "解法--" << ++total<<endl;
		printPath(h);
		printTrituple(h);
		cout << endl;
		return;
	}	//若至终点 打印路径与图示直接结束
	for (int i = 0;i < 4;i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];		//遍历四个方向
		if (nx > 0 && nx <= m && ny > 0 && ny <= n 
			&& repeatMaze[nx][ny] == 0 && Maze[nx][ny] == '0') {
			switch (i) {
			case 0:d = "→";break;
			case 1:d = "↓";break;
			case 2:d = "←";break;
			case 3:d = "↑";break;
			}
			linkList s = new LNode();
			*s = { nx, ny, 1, d, NULL };
			(h->length)++;
			tailInsert(h, s);

			dfsMaze(nx, ny, h, i);	//递归
			
			h->length--;	//回溯
			repeatMaze[nx][ny] = 0;
			tailDelete(h);
		}
	}
}
//深搜检测路径可行性
void dfsCheckMaze(int x, int y) {
	if (x == edx && y == edy) {
		connectCheck = true;
		return ;
	}
	for (int i = 0;i < 4;i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx > 0 && nx <= m && ny > 0 && ny <= n
	&& repeatMaze[nx][ny] == 0 && Maze[nx][ny] == '0') {
			repeatMaze[nx][ny] = 1;
			dfsCheckMaze(nx, ny);
			repeatMaze[nx][ny] = 0;
		}
	}
}
//回溯打印路径
void bfsPathIntoList(int x, int y,linkList h) {
	if (x == stx && y == sty) { return; }
	else {
		h->length++;	//记录bfs路径长度
		bfsPathIntoList(BfsPath[x][y].x, BfsPath[x][y].y, h);
		if (BfsPath[x][y].x == stx && BfsPath[x][y].y == sty) { return; }	//由于bfs是由终点回溯起点
		linkList END = new LNode();											//因此先回溯 后尾插入链表 
		*END = { BfsPath[x][y].x, BfsPath[x][y].y, 1, BfsPath[x][y].d, NULL };//保证链表的形式是由小到大
		tailInsert(h, END);
	}
}
//宽搜最短路径
void bfsMaze(int x, int y, linkList h, int i) {
	linkList END = new LNode();
	*END = { x,y,1,"x",NULL };
	BfsQueue.push(*END);
	repeatMaze[x][y] = 1;	//起点入队并置为已访问

	while (!BfsQueue.empty()) {
		LNode queueHeadNode = BfsQueue.front();	//取队头节点
		if (queueHeadNode.x == edx && queueHeadNode.y == edy) {
			bfsPathIntoList(queueHeadNode.x, queueHeadNode.x, h);	//回溯将路径存入链表
			tailInsert(h, &queueHeadNode);				//终点无法在回溯中插入
			printPath(h);								//打印三元组路径
			printTrituple(h);							//打印路径图示
			h->next = NULL;								//清空链表
			break;
		}
		BfsQueue.pop();	 //弹出队头节点
		for (int i = 0;i < 4;i++) {
			int nx = queueHeadNode.x + dx[i];
			int ny = queueHeadNode.y + dy[i];
			if (nx <= 0 || nx > m || ny <= 0 ||
				ny > n || repeatMaze[nx][ny] != 0 || Maze[nx][ny] != '0')continue;
			linkList end = new LNode();
			string d;
			switch (i) {
			case 0:d = "→";break;
			case 1:d = "↓";break;
			case 2:d = "←";break;
			case 3:d = "↑";break;
			default:d = "x ";break;
			}
			*end = { nx,ny,1,d,NULL };
			BfsQueue.push(*end);				//将未越界、可访问、未访问节点如队
			BfsPath[nx][ny].x = queueHeadNode.x;//将该点 的上一点存入BfsPath用于回溯路径
			BfsPath[nx][ny].y = queueHeadNode.y;
			BfsPath[nx][ny].d = d;
			repeatMaze[nx][ny] = 1;				//避免重复访问
		}
	}
}
//随机生成迷宫
void randmeMaze() {
	srand(time(NULL));
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			int temp = rand() % 2;	//随机序列随机生成
			Maze[i][j] = temp ? '1' : '0';
		}
	}
	Maze[stx][sty] = '0';
	Maze[edx][edy] = '0';
}
//随机初始化迷宫
void initAutoMaze() {
	connectCheck = false;
	do {
		randmeMaze();
		dfsCheckMaze(stx, sty);
	} while (!connectCheck);
}
//手动初始化迷宫
void initHandMaze() {
	cout << "按照" << m << "*" << n
		<< "的大小输入迷宫(1为障碍 0为通路)"<<endl;
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			cin >> Maze[i][j];
		}
	}
	Maze[stx][sty] = '0';
	Maze[edx][edy] = '0';
}
//初始化记录迷宫访问记录的数组 (起点置为已访问)
void initrepeatMaze() {
	memset(repeatMaze, 0, sizeof(repeatMaze));
	LNode blank = { -1, -1, 0, ".", NULL };
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < N;j++) {
			BfsPath[i][j] = blank;
		}
	}
}
//将地图存入 以pos为标记的位置
void writeIntoFile(char pos) {
	FILE *fp = fopen("迷宫地图记录.txt","a+");	//追加写入！！
	if (fp == NULL) {
		perror("文件开启异常:");
		exit(0);
	}
	char ch;	//迷宫标志 实现多个迷宫的存储
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == pos) {
			cout << "该位置已存在地图 请重新存入"<<endl;
			fclose(fp);
			return;
		}
	}		//检索若该标志已存在 无法覆盖已写入内容
	fputc(pos, fp);		//标志写入
	fputc('\n', fp);	//标志携入换行符	
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			fputc(Maze[i][j], fp);//以此将地图中01写入文件
		}
		fputc('\n', fp);
	}
	fputc('\n', fp);
	fclose(fp);
}
//读出 文件中以pos为标记位置的迷宫
void readPosFile(char pos) {
	FILE *fp = fopen("迷宫地图记录.txt", "r");
	if (fp == NULL) {
		perror("文件读取异常:");
		exit(0);
	}
	char ch;
	while (!feof(fp)) {	//若位置指针未达到文件尾
		ch = fgetc(fp);	//读一个字符
		if (ch == pos) {//直到到达标记出
			fgetc(fp);
			//将标志位后的数组存入迷宫
			for (int i = 1;i <= m;i++) {
				for (int j = 1;j <= n;j++) {
					ch = fgetc(fp);
					if (ch == '\n') {
						ch = fgetc(fp);
					}	//把换行符消掉
					Maze[i][j] = ch;
				}
			}
			cout << "迷宫更新成功" << endl;
			return;
		}
	}
	cout << "该标志位下并未存储迷宫";
	fclose(fp);
}
int main() {
	int choice = -1;	//菜单选项
	char pos;			//文件读取中迷宫标志

	cout << "输入迷宫大小 m行 n列: " ;
	cin >> m >> n;
	cout << "输入起点 终点坐标: ";
	cin >> stx >> sty >> edx >> edy;

	linkList head = new LNode();//三元组链表头指针
	*head = { stx, sty, 0, "x ", NULL };

	do {
		cout <<endl << "\t菜单" << endl;
		cout << "---------------------------" << endl;
		cout << " 1.随机初始化迷宫" << endl;
		cout << " 2.手动输入迷宫地图" << endl;
		cout << " 3.从文件中读出迷宫" << endl;
		cout << " 4.迷宫地图展示" << endl;
		cout << " 5.输出迷宫所有可能的路径" << endl;
		cout << " 6.输出最短路径" << endl;
		cout << " 7.更改迷宫大小 和 起始坐标" << endl;
		cout << " 8.将地图存入文件" << endl;
		cout << "---------------------------" << endl << endl;

		cout << "输入操作选项: ";
		cin >> choice;
		switch (choice) {
		case 1:	
			initAutoMaze();
			cout << "随机初始化成功" << endl;
			break;
		case 2:
			initHandMaze();
			cout << "手动输入初始化成功" << endl;
			break;
		case 3:
			cout << "输入 迷宫的标志位 (除01外的单字符): ";
			cin >> pos;
			readPosFile(pos);
			break;
		case 4:
			cout << "初始化迷宫为:" << endl;
			for (int i = 1;i <= m;i++) {
				for (int j = 1;j <= n;j++) {
					cout << Maze[i][j] << " ";
				}
				cout << endl;
			}
			break;
		case 5:
			connectCheck = false;
			dfsCheckMaze(stx, sty);
			if (!connectCheck) { cout << "该迷宫无解" << endl; break; }
			cout << "迷宫所有路径为:" << endl;
			total = 0;
			dfsMaze(stx, sty, head, -1);
			break;
		case 6:
			cout << "迷宫的最短路径为: " << endl;
			bfsMaze(stx, sty, head, -1);
			initrepeatMaze();
			break;
		case 7:
			cout << "重新输入迷宫大小 m行 n列:";
			cin >> m >> n;
			cout << "输入起点终点 坐标：";
			cin >> stx >> sty >> edx >> edy;
			*head = { stx, sty, 0, "x ", NULL };
			cout << "修改成功" << endl;
			break;
		case 8:
			cout << "输入存储位置(除01外的单字符): ";
			cin >> pos;
			writeIntoFile(pos);
			cout << "文件写入成功" << endl;
			break;
		default:
			break;
		}

	}while(choice != 0);
	return 0;
}