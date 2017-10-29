#include <iostream>
#include <math.h>
#include <time.h>
#include <windows.h>
using namespace std;

#define SUDOKU_SIZE  9
#define ZERO_SIGN 0
#define OPEN_LIST
#define CLOSED_LIST
#define ROW 3	//行
#define COL 3	//列
#define UP		-3
#define DOWN	 3
#define LEFT	-1
#define RIGHT	 1

const bool k_open_list = 0;
const bool k_closed_list = 1;

enum dirction{	up, down, e_left, e_right,none = 10};

int manhattan[9][9] = //第i个数及其所处不同位置的Manhattan路径长度  
{
	{ 0, 1, 2, 1, 2, 3, 2, 3, 4 },
	{ 1, 0, 1, 2, 1, 2, 3, 2, 3 },
	{ 2, 1, 0, 3, 2, 1, 4, 3, 2 },
	{ 1, 2, 3, 0, 1, 2, 1, 2, 3 },
	{ 2, 1, 2, 1, 0, 1, 2, 1, 2 },
	{ 3, 2, 1, 2, 1, 0, 3, 2, 1 },
	{ 2, 3, 4, 1, 2, 3, 0, 1, 2 },
	{ 3, 2, 3, 2, 1, 2, 1, 0, 1 },
	{ 4, 3, 2, 3, 2, 1, 2, 1, 0 }
};
int position[9] = { -1, 0, 1, 2, 5, 8, 7, 6, 3 };

struct Sudoku
{
	int numbers[ROW][COL];
	unsigned char sum;
	unsigned char value;
	unsigned char depth;
	unsigned char zero_position;
	unsigned char last_position;
	int id;
	dirction dir = none;
	Sudoku* next;
};

Sudoku* g_open_head = new Sudoku;
Sudoku* g_closed_head = new Sudoku;
Sudoku start_sudoku;
Sudoku ultimate_sudoku;
Sudoku k_ultimate_state;

void PrintSudoku(const Sudoku& sudoku);

int JudgeErrorPos(const Sudoku& sourseState, const Sudoku& ultimateState)
{
	int error_rate = 0;
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
	{
			if (sourseState.numbers[i][j] != ultimateState.numbers[i][j])
			{
				error_rate++;
			}
	}
	return error_rate;
}

int Manhattan_distance(const Sudoku& sourseState, const Sudoku& ultimateState)
{
	int error_rate = 0;
	int x1, x2, y1, y2;
	for (int i = 0; i < ROW; i++)
	for (int j = 0; j < COL; j++)
	{
		if (sourseState.numbers[i][j] != ultimateState.numbers[i][j])
		{
			int x = i*COL + j;
			error_rate += manhattan[x][position[sourseState.numbers[i][j]]];
		}
	}
	return error_rate;
}

int GenerateNodeId(const Sudoku& node)
{
	int multiplier = 1;
	int sum = 0;
	for (int i = 0; i < ROW; i++)
	for (int j = 0; j< COL; j++)
	{
		sum = node.numbers[i][j] * multiplier+sum;
		multiplier = multiplier * 10;
	}
	return sum;
}
void InitSudoku(Sudoku* startState, Sudoku* ultimateState)
{
	unsigned int test_start[3][3] = { { 3, 7, 2 }, { 8, 1, 5 }, { 4, 6, 0 } };
	unsigned int test_ult[3][3] = { { 1, 2, 3 }, { 8, 0, 4 }, { 7, 6, 5 } };
	cout << "Please input the initial state:\n";
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			startState->numbers[i][j] = test_start[i][j];
		}
	}
	cout << "Please input the ultimate state:\n";
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			ultimateState->numbers[i][j] = test_ult[i][j];
		}
	}
	startState->value = Manhattan_distance(*startState, *ultimateState);
	startState->id = GenerateNodeId(*startState);
	startState->depth = 0;
	startState->sum = startState->value + startState->depth;
	startState->next = NULL;

	ultimateState->id = GenerateNodeId(*ultimateState);
	ultimateState->value = -1;
	ultimateState->depth = 0;
	ultimateState->sum = ultimateState->value + ultimateState->depth;
	ultimateState->next = NULL;

	PrintSudoku(*startState);
	PrintSudoku(*ultimateState);
}
void PrintSudoku(const Sudoku& sudoku)
{
	for (int i = 0; i < COL; i++)
	{
		for (int j = 0; j < ROW; j++)
		{
			cout << sudoku.numbers[i][j]<<" ";
		}
		cout << endl;
	}
	cout << endl;
}
int IsOpenNil(const Sudoku* openHead)
{
	if (openHead->next == NULL)
		return 1;
	return 0;
}
int IsSecurityNode(Sudoku* node, Sudoku* g_openHead, Sudoku* g_closeHead)
{
	Sudoku* pLink = g_openHead;
	Sudoku* pnode = NULL;
	while (pLink->next != NULL)
	{
		if (pLink->next->id == node->id)
		{
			if (node->sum < pLink->next->sum)
			{
				node->next = pLink->next->next;
				free(pLink->next);
				pLink->next = node;
				return 0;
			}
		}
		pLink = pLink->next;
	}
	pLink = g_closeHead;
	while (pLink->next != NULL)
	{
		if (pLink->next->id == node->id)
		{
			return 0;
		}
		pLink = pLink->next;
	}
	return 1;
}
void MakeSudoku(const Sudoku& fatherNode, Sudoku* g_openHead, const int zeroPos, const int lastPos,dirction dirc)
{
	Sudoku* node = new Sudoku;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			node->numbers[i][j] = fatherNode.numbers[i][j];
		}
	}
	int fx = zeroPos / ROW;
	int fy = zeroPos % COL;

	int sx = lastPos / ROW;
	int sy = lastPos % COL;

	node->dir = dirc;
	int swap = node->numbers[fx][fy];
	node->numbers[fx][fy]  = node->numbers[sx][sy];
	node->numbers[sx][sy] = swap;
	
	node->value = Manhattan_distance(*node, ultimate_sudoku);
	node->id = GenerateNodeId(*node);
	node->depth = fatherNode.depth + 1;
	node->sum = node->value + node->depth;
	node->zero_position = zeroPos;
	node->last_position = lastPos;

	if (1 == IsSecurityNode(node, g_openHead, g_closed_head))
	{
		node->next = g_openHead->next;
		g_openHead->next = node;

	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			if (node->numbers[i][j] < 0)
				system("pause");
		}
	}
}
int GetDirctionValue(dirction dir)
{
	switch (dir)
	{
	case up:	return	UP;
	case down:	return	DOWN;
	case e_left:return	LEFT;
	case e_right:return	RIGHT;
	default:	 return 0;
	}
}
void GenerateNode(const Sudoku* fathernode,Sudoku* openHead)
{
	int zero_pos;
	int reversnode = fathernode->dir;
	int flag = 0;
	for (int i = 0; i < COL; i++)
	{

		for (int j = 0; j < ROW; j++)
		{
			if (fathernode->numbers[i][j] == ZERO_SIGN)
			{
				zero_pos = i*COL + j;
				flag = 1;
				break;
			}
			if (fathernode->numbers[i][j] < 0)
				PrintSudoku(*fathernode);
		}
		if (flag == 1)
			break;
	}
	reversnode = zero_pos + GetDirctionValue(fathernode->dir);
	//reversnode = 父节点的zero坐标 + dirction坐标值
	if ((zero_pos + 1 < SUDOKU_SIZE)&&(zero_pos + 1) / ROW == (zero_pos / ROW) && (zero_pos + 1 != reversnode))
	{
		MakeSudoku(*fathernode, openHead, zero_pos, zero_pos + 1, e_left);
	}
	if (((zero_pos  - 1)>=0) &&(zero_pos - 1) / ROW == (zero_pos / ROW) && (zero_pos - 1 != reversnode))
	{
		MakeSudoku(*fathernode, openHead, zero_pos, zero_pos - 1, e_right);
	}
	if (((zero_pos + ROW) < SUDOKU_SIZE) && (zero_pos + ROW != reversnode))
	{
		MakeSudoku(*fathernode, openHead, zero_pos, zero_pos + ROW, up);
	}
	if (((zero_pos - ROW) >= 0) && (zero_pos - ROW != reversnode))
	{
		MakeSudoku(*fathernode, openHead, zero_pos, zero_pos - ROW, down);
	}
}
int PutInClose(Sudoku* node, Sudoku* g_openHead, Sudoku* g_closeHead)
{
	Sudoku* pOpen = g_openHead;
	Sudoku* pClose = g_closeHead;
	while (pOpen->next !=node)
	{
		pOpen = pOpen->next;
	}
	pOpen->next = pOpen->next->next;
	
	//put in the head of close
	node->next = g_closeHead->next;
	g_closeHead->next = node;
	
	return 1;
}
void PutInList(Sudoku* node, Sudoku* g_openHead, Sudoku* g_closeHead,const bool& list)
{
	Sudoku* pList = NULL;
	if (list == 0)
		pList = g_openHead;
	else
		pList = g_closeHead;
	
	pList->next = node;
	node->next = NULL;
}

Sudoku* FindMinSum(Sudoku* g_openHead)
{
	int sum = 0xffff;
	Sudoku* minNode = NULL;
	Sudoku* open = NULL;
	minNode = g_openHead->next;

	open = g_openHead->next->next;
	while (open != NULL)
	{
		if (minNode->sum > open->sum)
		{
			minNode = open;
		}
		open = open->next;
	}
	return minNode;
}
int IsUltimateState(const Sudoku* node)
{
	if (node->id == ultimate_sudoku.id)
		return 1;
	return 0;
}
void PrintSuccessWay(Sudoku* g_closeHead)
{
	Sudoku* pLink = g_closeHead->next;
	Sudoku* node = new Sudoku;

	g_closeHead->next = NULL;
	node = pLink;
	Sudoku* pnode = NULL;
	Sudoku*	pnext = NULL;
	Sudoku* now = NULL;
	Sudoku* next = NULL;

	int sucessArry[50];
	int successNumber = 0;

	int zx, zy;		//zero-x,y
	int rx, ry;		//reverse-x,y

	sucessArry[successNumber++] = node->id;
	//node->next = NULL;

	pLink = pLink->next;
	pnext = pLink->next;
	node->next = g_closeHead;
	while (pnext != NULL)
	{
		zx = node->zero_position / ROW; zy = node->zero_position % COL;
		rx = node->last_position / ROW; ry = node->last_position% COL;

		int x = node->numbers[zx][zy] * pow(10, node->zero_position);
		int y = node->numbers[zx][zy] * pow(10, node->last_position);
		node->id = node->id - x + y;
		sucessArry[successNumber++] = node->id;

		while (node->id != pLink->id && pnext != NULL)
		{
			delete(pLink);
			pLink = pnext;
			pnext = pLink->next;
		}
		pLink->next = node;
		node = pLink;
		if (pnext != NULL){
			pLink = pnext;
			pnext = pLink->next;
		}
	}
	int step = 1;
	cout << "step: " << step++ << endl;
	PrintSudoku(*pLink);
	node = node->next;
	while (node->next != NULL)
	{
		cout << "step: " << step++ << endl;
		PrintSudoku(*node);
		now = node;
		node = node->next;
		delete(now);
	} 
	now = g_open_head->next;
	next = now->next;
	while (next != NULL)
	{
		delete(now);
		now = next;
		next = next->next;
	}
}
void test()
{
	Sudoku* p = g_open_head; 

	while (p !=NULL)
	{
		for (int i = 0; i < COL; i++)
		for (int j = 0; j < ROW; j++)
		{
			if (p->numbers[i][j] < 0)
				system("pause");
		}
	}
}
void PlaySudoku(Sudoku* startSudoku, Sudoku* ultimateSudoku)
{
	Sudoku* minNode = NULL;
	minNode = startSudoku;
	PutInList(startSudoku, g_open_head, g_closed_head ,k_open_list);
	while (0 == IsUltimateState(minNode))
	{
		minNode = FindMinSum(g_open_head);
		//cout << int(minNode->depth) << " " << int(minNode->value)<<" "<<int(minNode->id)<<" \n";	
		PutInClose(minNode, g_open_head, g_closed_head); 
		GenerateNode(minNode, g_open_head);
	}
	minNode = g_closed_head;
	PrintSuccessWay(minNode);
}
int main()
{
	double start = GetTickCount();
	g_open_head->next = NULL;
	g_closed_head->next = NULL;

	InitSudoku(&start_sudoku, &ultimate_sudoku);
	PlaySudoku(&start_sudoku, &ultimate_sudoku);
	
	free(g_open_head);
	free(g_closed_head);

	double  end = GetTickCount();
	cout << "GetTickCount:" << end - start << endl;

	return 0;
}