#include <iostream>
#include <vector>
using namespace std;

enum Move
{
	UP, DOWN, LEFT, RIGHT
};

struct Node
{
	int state[9];	// 0 1 2
					// 3 4 5
					// 6 7 8
	int cost;		// 估价函数fn的值
	Move move;
	// Node *parent;
};

vector<Node> Open;
vector<Node> Close;

const int target_state[9] = { 1,2,3,8,0,4,7,6,5 };

int IsSucceeded(Node &node)
{
	for (int i = 0; i < 9; i++)
		if (target_state[i] != node.state[i] && node.state[i] != 0)
			return 0;
	return 1;
}

int CountCost(Node &node, int depth)
{
	for (int i = 0; i < 9; i++)
		if (target_state[i] != node.state[i] && node.state[i] != 0)
			node.cost++;
	if (node.cost == 0) return 0;
	else return node.cost + depth;
}
// 1 2 3 8 0 4 7 6 5

void FindMinCost()
{
	vector<Node>::iterator iter = Open.begin() + 1, min = Open.begin();
	for (; iter < Open.end(); iter++)
		if (iter->cost < min->cost) min = iter;
	Open.erase(min);
	Close.push_back(*min);
}

void CreateExpandingNode(const Node *c_node, int depth)
{
	int index_0, temp;
	Node new_node;
	for (int i = 0; i < 9; i++)
		if (c_node->state[i] == 0) { index_0 = i; break; }

	depth += 1;

	// 空格上移 即空格0的下标-3的元素与空格交换
	if (c_node->move != DOWN) {
		new_node = *c_node;
		if (index_0 >= 3) {
			new_node.state[index_0] = new_node.state[index_0 - 3];
			new_node.state[index_0 - 3] = 0;
		}
		new_node.cost = CountCost(new_node, depth);
		// new_node.parent = (Node *)c_node;
		Open.push_back(new_node);
	}

	// 空格下移 即空格0的下标+3的元素与空格交换
	if (c_node->move != UP) {
		new_node = *c_node;
		if (index_0 <= 5) {
			new_node.state[index_0] = new_node.state[index_0 + 3];
			new_node.state[index_0 + 3] = 0;
		}
		new_node.cost = CountCost(new_node, depth);
		// new_node.parent = (Node *)c_node;
		Open.push_back(new_node);
	}


	// 空格左移 即空格0的下标-1的元素与空格交换
	if (c_node->move != RIGHT) {

		new_node = *c_node;
		if (index_0 != 0 && index_0 != 3 && index_0 != 6) {
			new_node.state[index_0] = new_node.state[index_0 - 1];
			new_node.state[index_0 - 1] = 0;
		}
		new_node.cost = CountCost(new_node, depth);
		// new_node.parent = (Node *)c_node;
		Open.push_back(new_node);

	}

	// 空格右移 即空格0的下标+1的元素与空格交换
	if (c_node->move != LEFT) {
		new_node = *c_node;
		if (index_0 != 2 && index_0 != 5 && index_0 != 8) {
			new_node.state[index_0] = new_node.state[index_0 + 1];
			new_node.state[index_0 + 1] = 0;
		}
		new_node.cost = CountCost(new_node, depth);
		// new_node.parent = (Node *)c_node;
		Open.push_back(new_node);
	}

}

int main()
{
	Node node;
	node.cost = 0;

	int depth = 0;

	// node.parent = NULL;
	cout << "请输入初始棋局：";
	for (int i = 0; i < 9; i++)
		scanf("%d", &node.state[i]);

	if (!CountCost(node, depth)) { 
		cout << "已经完成棋局" << endl; 
		return 0; 
	}
	else Open.push_back(node);

	while (!Open.empty())
	{
		// 从Open表中选择一个f值最小的节点
		FindMinCost();
		// 把节点从OPEN表中移出，并放入CLOSED的扩展节点表中
		node = Close.back();
		// 如果是目标节点，则成功退出，求得一个解
		if (IsSucceeded(node)) {
			cout << "成功找到一条路径" << endl;
			/* to do … */ 
			return 0; 
		}
		// 扩展当前结点，生成全部后续节点

	}

	cout << "无解" << endl;
	return 0;
}