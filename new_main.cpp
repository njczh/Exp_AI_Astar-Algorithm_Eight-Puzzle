#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int total = 0;
typedef int State[9];
State goal = { 1,2,3,8,0,4,7,6,5 };

struct Node
{
	State state;
	int diff;
	int depth;
	int hash;
	int fa_hash;
	int nd;
};

vector<Node> Open;
vector<Node> Close;

const int move_x[] = { -1,1,0,0 };
const int move_y[] = { 0,0,-1,1 };

const int dis[9][9] = {
{0, 1, 2, 1, 2, 3, 2, 3, 4},
{1, 0, 1, 2, 1, 2, 3, 2, 3},
{2, 1, 0, 3, 2, 1, 4, 3, 2},
{1, 2, 3, 0, 1, 2, 1, 2, 3},
{2, 1, 2, 1, 0, 1, 2, 1, 2},
{3, 2, 1, 2, 1, 0, 3, 2, 1},
{2, 3, 4, 1, 2, 3, 0, 1, 2},
{3, 2, 3, 2, 1, 2, 1, 0, 1},
{4, 3, 2, 3, 2, 1, 2, 1, 0},
};

void InitNode(Node &node) { 
	memset(&node, 0, sizeof(node));
	node.hash = node.state[0];
	for (int i = 1; i < 9; i++)
		node.hash = (node.hash * 10 + node.state[i]);
	node.nd = -1;
}

int IsThereASolution(Node &init_node)
{
	int inverse_1 = 0;
	for (int i = 1; i < 9; i++)
		for (int j = 0; j < i; j++) {
			if (init_node.state[i] < init_node.state[j])inverse_1++;
		}
	if (inverse_1 % 2) return 0;
	else return 1;
}

void CountDiff(Node &node)
{
	node.diff = 0;
	for (int i = 0; i < 9; i++)
		if (goal[i] != node.state[i])
			//node.diff += dis[goal[i]][node.state[i]];
			node.diff++;
}

bool MySort(const Node &node1, const Node &node2)
{
	return (node1.depth + node1.diff) > (node2.depth + node2.diff);
}

void CreateExpandingNode(const Node &c_node)
{
	int z;
	for (z = 0; z < 9; z++) if (!c_node.state[z]) break;
	int x = z / 3, y = z % 3;
	for (int d = 0; d < 4; d++)
	{
		Node temp;
		if (c_node.nd == d) continue;
		
		bool flag = false;

		int newx = x + move_x[d];
		int newy = y + move_y[d];
		int newz = newx * 3 + newy;
		if (!(newx >= 0 && newx < 3 && newy >= 0 && newy < 3)) continue;

		switch (d)
		{
		case 0: temp.nd = 1; break;
		case 1: temp.nd = 0; break;
		case 2: temp.nd = 3; break;
		case 3: temp.nd = 2; break;
		}

		memcpy(&temp, &c_node, sizeof(State));
		temp.state[newz] = c_node.state[z];
		temp.state[z] = c_node.state[newz];
		CountDiff(temp);

		temp.depth = c_node.depth + 1;
		temp.fa_hash = c_node.hash;
				
		temp.hash = temp.state[0];
		for (int i = 1; i < 9; i++)
			temp.hash = (temp.hash * 10 + temp.state[i]);

///*debug*/	for (int i = 0; i < 9; i++) { if (i % 3 == 0)cout << endl; cout << temp.state[i] << " ";  }
///*debug*/	cout << "\nd=" << d << " depth=" << temp.depth << " diff=" << temp.diff << " hash=" << temp.hash <<" fa_hash="<<temp.fa_hash<< endl;

		
		for (vector<Node>::iterator iter = Open.begin(); iter < Open.end(); iter++)
			if (iter->hash == temp.hash)
			{
				if (temp.depth < iter->depth)
				{
					iter->diff = temp.diff;
					iter->depth = temp.depth;
					iter->fa_hash = temp.fa_hash;
					flag = true;
					break;
				}
				else { flag = true; break; }
			}

		//for (vector<Node>::iterator iter = Close.begin(); iter < Close.end(); iter++)
		//	if (iter->hash == temp.hash) flag = true;
		

		if (!flag) { Open.push_back(temp); }
	}

	sort(Open.begin(), Open.end(),MySort);

}

int main()
{
	Node node;
	InitNode(node);
	
	for (int i = 0; i < 9; i++) scanf("%d", &node.state[i]);
	//for (int i = 0; i < 9; i++) scanf("%d", &goal);
	CountDiff(node);

///*debug*/	cout << "First->depth=" << node.depth << " First->diff=" << node.diff << endl;

	if (0/*IsThereASolution(node)*/) { printf("无解！\n"); return 0; }
	else {
		printf("初始节点为：");
		for (int i = 0; i < 9; i++) {
			if (i % 3 == 0)cout << endl;
			if (node.state[i] != 0)
				cout << node.state[i] << " ";
			else cout << "  ";
		}
		cout << endl << endl;
	}

	Open.push_back(node);
	while (!Open.empty())
	{		
		// 从Open表中选择一个f值最小的节点,并将其从OPEN表中移出，放在CLOSED表中
		Close.push_back(Open.back());
		Open.pop_back();
		node = Close.back();
		
///*debug*/	for (int i = 0; i < 9; i++) { if (i % 3 == 0)cout << endl; cout << node.state[i] << " "; }
///*debug*/	cout << "Close->depth=" << node.depth << " Close->diff=" << node.diff << endl;

		// 如果是目标节点，则成功退出，求得一个解
		if (memcmp(node.state, goal, sizeof(goal)) == 0) {
			cout << "成功找到一条路径,路径如下：" << endl;

			vector<Node>Step;
			int fa = node.fa_hash;
			Step.push_back(node);

			while (fa){
				for (vector<Node>::iterator iter = Close.begin(); iter < Close.end(); iter++){
					if (iter->hash == fa) {
						Step.push_back(*iter);
						if (fa != 0) fa = iter->fa_hash; 
						break;
					}
				}
			}
		
			int step = 1;
			while(!Step.empty()){
				Node temp = Step.back();
				printf("STEP #%d", step++);
				for (int i = 0; i < 9; i++) {
					if (i % 3 == 0)cout << endl; 
					if(temp.state[i]!=0)
						cout << temp.state[i] << " "; 
					else cout << "  ";
				}
				cout << endl << endl;
				Step.pop_back();
			}
			system("pause");
			return 0;
		}

		// 扩展当前结点，生成全部后续节点
		CreateExpandingNode(node);
	}
	return 0;
}

// test 1:		1 3 0 8 2 4 7 6 5
// test 2:		2 8 0 1 6 3 7 5 4
// tset 3:		3 7 2 8 1 5 4 6 0

// test 4:		8 3 2 7 1 0 4 6 5