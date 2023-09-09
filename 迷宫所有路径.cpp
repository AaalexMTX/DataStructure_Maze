//��  1.����ĩ�������ǰһ���ڵ� �ĺ��� -- ʵ�ֻ���ʹ��β��βɾ���ٶȿ�����
// -  2.ͷ��ͷɾ ���������dfs�н�  1.����������� �� 2.��ӡ·��
//��  3��d���Ϸ���ָʾ������
//��  4��¼�Թ�����
//��  5�ֶ������Թ�
//��  6.�ļ���д ��¼/�Թ�
//    �ļ������� �Թ��Ĵ�Сƥ�� 
//�� �Թ����·�����Ž�
//��  �����޽��Թ��ķ���


#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<queue>
using namespace std;
#define N 30
int m, n,total;		//��¼�Թ�����������ܳ���
int stx, sty, edx, edy;		//��� �յ�����
char Maze[N][N];	//��¼�Թ���ͼ
int repeatMaze[N][N];	//��¼���ʼ�¼
int dx[4] = { 0,1,0,-1 };	
int dy[4] = { 1,0,-1,0 };	//��������
//�����¼·��
typedef struct LNode {
	int x, y, length;
	string d;
	struct LNode *next;
}LN, *linkList;
LNode BfsPath[N][N];	//BFS�����ڻ���·��
queue<LNode> BfsQueue;	//BFS����

bool connectCheck = false;	//�Թ���ͨ�Լ��ı��
//βɾ
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
	}	//˫ָ���ҵ�ĩ�˽ڵ��ǰһ���ڵ�
}
//β��
void tailInsert(linkList h, linkList EndNode) {
	if (h == NULL) { h = EndNode; }
	linkList p = h;
	while (p->next != NULL) {
		p = p->next;
	}
	p->next = EndNode;
	//free(EndNode);  ���˾ͱ�������
}
//dfs����Թ�·��
void printPath(linkList h) {
	linkList CIR = new LNode();	//�����������Ƿ��� ��·���ڵ�
	for (int i = 1; i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			bool checkNode = false;	//ȷ��·����ͼ�Ĵ�ӡ���ݣ������ͷ����0/1��
			CIR = h;
			while (CIR != NULL) {
				if (CIR->x == i && CIR->y == j) {
					checkNode = true;
					break;
				}
				CIR = CIR->next;
			}
			if (checkNode) {
				cout << CIR->d;		//���õ�Ϊ�ڽ��·���� ��ӡ��ͷ
			}
			else { 
				cout << Maze[i][j] << " ";
			}
		}
		cout << endl;
	}
}
//�����Ԫ��
void printTrituple(linkList h) {
	if (h == NULL ) { return; }
	linkList s = h; 
	cout << "��·������Ϊ" << h->length << endl;
	while (s->next != NULL) {
		cout << "(" << s->x << "," << s->y << "," << s->d << ")--";
		s = s->next;
	}
	cout << "(" << s->x << "," << s->y << "," << s->d << ")"<<endl; 
}
//��������·�� -- β��βɾ
void dfsMaze(int x, int y, linkList h, int i) {
	string d;
	repeatMaze[x][y] = 1;	//ר�������������Ϊ�ѷ���
	if (x == edx && y == edy) {
		for (int i = 2;i < n;i += 2) { cout << " "; }
		cout << "�ⷨ--" << ++total<<endl;
		printPath(h);
		printTrituple(h);
		cout << endl;
		return;
	}	//�����յ� ��ӡ·����ͼʾֱ�ӽ���
	for (int i = 0;i < 4;i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];		//�����ĸ�����
		if (nx > 0 && nx <= m && ny > 0 && ny <= n 
			&& repeatMaze[nx][ny] == 0 && Maze[nx][ny] == '0') {
			switch (i) {
			case 0:d = "��";break;
			case 1:d = "��";break;
			case 2:d = "��";break;
			case 3:d = "��";break;
			}
			linkList s = new LNode();
			*s = { nx, ny, 1, d, NULL };
			(h->length)++;
			tailInsert(h, s);

			dfsMaze(nx, ny, h, i);	//�ݹ�
			
			h->length--;	//����
			repeatMaze[nx][ny] = 0;
			tailDelete(h);
		}
	}
}
//���Ѽ��·��������
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
//���ݴ�ӡ·��
void bfsPathIntoList(int x, int y,linkList h) {
	if (x == stx && y == sty) { return; }
	else {
		h->length++;	//��¼bfs·������
		bfsPathIntoList(BfsPath[x][y].x, BfsPath[x][y].y, h);
		if (BfsPath[x][y].x == stx && BfsPath[x][y].y == sty) { return; }	//����bfs�����յ�������
		linkList END = new LNode();											//����Ȼ��� ��β�������� 
		*END = { BfsPath[x][y].x, BfsPath[x][y].y, 1, BfsPath[x][y].d, NULL };//��֤�������ʽ����С����
		tailInsert(h, END);
	}
}
//�������·��
void bfsMaze(int x, int y, linkList h, int i) {
	linkList END = new LNode();
	*END = { x,y,1,"x",NULL };
	BfsQueue.push(*END);
	repeatMaze[x][y] = 1;	//�����Ӳ���Ϊ�ѷ���

	while (!BfsQueue.empty()) {
		LNode queueHeadNode = BfsQueue.front();	//ȡ��ͷ�ڵ�
		if (queueHeadNode.x == edx && queueHeadNode.y == edy) {
			bfsPathIntoList(queueHeadNode.x, queueHeadNode.x, h);	//���ݽ�·����������
			tailInsert(h, &queueHeadNode);				//�յ��޷��ڻ����в���
			printPath(h);								//��ӡ��Ԫ��·��
			printTrituple(h);							//��ӡ·��ͼʾ
			h->next = NULL;								//�������
			break;
		}
		BfsQueue.pop();	 //������ͷ�ڵ�
		for (int i = 0;i < 4;i++) {
			int nx = queueHeadNode.x + dx[i];
			int ny = queueHeadNode.y + dy[i];
			if (nx <= 0 || nx > m || ny <= 0 ||
				ny > n || repeatMaze[nx][ny] != 0 || Maze[nx][ny] != '0')continue;
			linkList end = new LNode();
			string d;
			switch (i) {
			case 0:d = "��";break;
			case 1:d = "��";break;
			case 2:d = "��";break;
			case 3:d = "��";break;
			default:d = "x ";break;
			}
			*end = { nx,ny,1,d,NULL };
			BfsQueue.push(*end);				//��δԽ�硢�ɷ��ʡ�δ���ʽڵ����
			BfsPath[nx][ny].x = queueHeadNode.x;//���õ� ����һ�����BfsPath���ڻ���·��
			BfsPath[nx][ny].y = queueHeadNode.y;
			BfsPath[nx][ny].d = d;
			repeatMaze[nx][ny] = 1;				//�����ظ�����
		}
	}
}
//��������Թ�
void randmeMaze() {
	srand(time(NULL));
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			int temp = rand() % 2;	//��������������
			Maze[i][j] = temp ? '1' : '0';
		}
	}
	Maze[stx][sty] = '0';
	Maze[edx][edy] = '0';
}
//�����ʼ���Թ�
void initAutoMaze() {
	connectCheck = false;
	do {
		randmeMaze();
		dfsCheckMaze(stx, sty);
	} while (!connectCheck);
}
//�ֶ���ʼ���Թ�
void initHandMaze() {
	cout << "����" << m << "*" << n
		<< "�Ĵ�С�����Թ�(1Ϊ�ϰ� 0Ϊͨ·)"<<endl;
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			cin >> Maze[i][j];
		}
	}
	Maze[stx][sty] = '0';
	Maze[edx][edy] = '0';
}
//��ʼ����¼�Թ����ʼ�¼������ (�����Ϊ�ѷ���)
void initrepeatMaze() {
	memset(repeatMaze, 0, sizeof(repeatMaze));
	LNode blank = { -1, -1, 0, ".", NULL };
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < N;j++) {
			BfsPath[i][j] = blank;
		}
	}
}
//����ͼ���� ��posΪ��ǵ�λ��
void writeIntoFile(char pos) {
	FILE *fp = fopen("�Թ���ͼ��¼.txt","a+");	//׷��д�룡��
	if (fp == NULL) {
		perror("�ļ������쳣:");
		exit(0);
	}
	char ch;	//�Թ���־ ʵ�ֶ���Թ��Ĵ洢
	while (!feof(fp)) {
		ch = fgetc(fp);
		if (ch == pos) {
			cout << "��λ���Ѵ��ڵ�ͼ �����´���"<<endl;
			fclose(fp);
			return;
		}
	}		//�������ñ�־�Ѵ��� �޷�������д������
	fputc(pos, fp);		//��־д��
	fputc('\n', fp);	//��־Я�뻻�з�	
	for (int i = 1;i <= m;i++) {
		for (int j = 1;j <= n;j++) {
			fputc(Maze[i][j], fp);//�Դ˽���ͼ��01д���ļ�
		}
		fputc('\n', fp);
	}
	fputc('\n', fp);
	fclose(fp);
}
//���� �ļ�����posΪ���λ�õ��Թ�
void readPosFile(char pos) {
	FILE *fp = fopen("�Թ���ͼ��¼.txt", "r");
	if (fp == NULL) {
		perror("�ļ���ȡ�쳣:");
		exit(0);
	}
	char ch;
	while (!feof(fp)) {	//��λ��ָ��δ�ﵽ�ļ�β
		ch = fgetc(fp);	//��һ���ַ�
		if (ch == pos) {//ֱ�������ǳ�
			fgetc(fp);
			//����־λ�����������Թ�
			for (int i = 1;i <= m;i++) {
				for (int j = 1;j <= n;j++) {
					ch = fgetc(fp);
					if (ch == '\n') {
						ch = fgetc(fp);
					}	//�ѻ��з�����
					Maze[i][j] = ch;
				}
			}
			cout << "�Թ����³ɹ�" << endl;
			return;
		}
	}
	cout << "�ñ�־λ�²�δ�洢�Թ�";
	fclose(fp);
}
int main() {
	int choice = -1;	//�˵�ѡ��
	char pos;			//�ļ���ȡ���Թ���־

	cout << "�����Թ���С m�� n��: " ;
	cin >> m >> n;
	cout << "������� �յ�����: ";
	cin >> stx >> sty >> edx >> edy;

	linkList head = new LNode();//��Ԫ������ͷָ��
	*head = { stx, sty, 0, "x ", NULL };

	do {
		cout <<endl << "\t�˵�" << endl;
		cout << "---------------------------" << endl;
		cout << " 1.�����ʼ���Թ�" << endl;
		cout << " 2.�ֶ������Թ���ͼ" << endl;
		cout << " 3.���ļ��ж����Թ�" << endl;
		cout << " 4.�Թ���ͼչʾ" << endl;
		cout << " 5.����Թ����п��ܵ�·��" << endl;
		cout << " 6.������·��" << endl;
		cout << " 7.�����Թ���С �� ��ʼ����" << endl;
		cout << " 8.����ͼ�����ļ�" << endl;
		cout << "---------------------------" << endl << endl;

		cout << "�������ѡ��: ";
		cin >> choice;
		switch (choice) {
		case 1:	
			initAutoMaze();
			cout << "�����ʼ���ɹ�" << endl;
			break;
		case 2:
			initHandMaze();
			cout << "�ֶ������ʼ���ɹ�" << endl;
			break;
		case 3:
			cout << "���� �Թ��ı�־λ (��01��ĵ��ַ�): ";
			cin >> pos;
			readPosFile(pos);
			break;
		case 4:
			cout << "��ʼ���Թ�Ϊ:" << endl;
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
			if (!connectCheck) { cout << "���Թ��޽�" << endl; break; }
			cout << "�Թ�����·��Ϊ:" << endl;
			total = 0;
			dfsMaze(stx, sty, head, -1);
			break;
		case 6:
			cout << "�Թ������·��Ϊ: " << endl;
			bfsMaze(stx, sty, head, -1);
			initrepeatMaze();
			break;
		case 7:
			cout << "���������Թ���С m�� n��:";
			cin >> m >> n;
			cout << "��������յ� ���꣺";
			cin >> stx >> sty >> edx >> edy;
			*head = { stx, sty, 0, "x ", NULL };
			cout << "�޸ĳɹ�" << endl;
			break;
		case 8:
			cout << "����洢λ��(��01��ĵ��ַ�): ";
			cin >> pos;
			writeIntoFile(pos);
			cout << "�ļ�д��ɹ�" << endl;
			break;
		default:
			break;
		}

	}while(choice != 0);
	return 0;
}