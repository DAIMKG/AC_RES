#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<vector>
#include<string>
#include<cstring>
#include<algorithms>
#include<utility>

using namespace std;

#define SO 0
#define OS 1
#define SS 2
#define OO 3

// 

class Pattern
{
	public:
		vector<int>variable;
		vector<int>null;
		vector<int>p;
		int graph[100][100]; // 原图，未变换
		int linegraph[100][100];
		int v_num; // 原图点数
		int e_num; // 原图边数
}pattern[10];


int pattern_num;
int vertice_num;
int edge_num;
int productgraph[100][100];
int temp_productgraph[100][100];

vector<int>productgraph_p;

Init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			pattern[i].graph[j][k] = -1;
			pattern[i].linegraph[j][k] = -1;
		}
	}
	pattern[i].v_num = 0;
	pattern[i].e_num = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			productgraph[i][j] = -1;
			temp_productgraph[i][j] = -1;
		}
	}
}

void swap()
{
	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			productgraph[i][j] = temp_productgraph[i][j];
}

void my_cin()
{
	ifstream fin(argv[1]);
	fin >> pattern_num;
	for (int i = 0; i < pattern_num; i++)
	{
		fin >> vertice_num;
		pattern[i].v_num = vertice_num;
		for (int j = 0; j < vertice_num; j++)
		{
			string state;
			int index_;
			fin >> index_ >> state;
			if (state[0] == 'v')
				(pattern[i].variable).push_back(index_);
			else if (state[0] == 'n')
				(pattern[i].null).push_back(index_);
		}
		fin >> edge_num;
		for (int j = 0; j < edge_num; j++)
		{
			int from;
			int to;
			int pred;
			fin >> from >> to >> pred;
			pattern[i].graph[from][to] = pred;
			(pattern[i].p).push_back(pred);
		}
	}
}

void form_linegraph()
{
	for (int i = 0; i < pattern_num; i++)
	{
		current_pattern = pattern[i];

		set<pair<int,int> >a;
		int t = current_pattern.v_num;
		for (int j = 0; j < t; j++)
		{
			for (int k = 0; k < t; K++)
			{
				if (current_pattern.graph[j][k] != -1)
				{
					a.insert(pair<int,int>(j,k));
				}
			}
		}

		set<pair<int,int> >::iterator it1;
		set<pair<int,int> >::iterator it2;
		for (it1=a.begin(); it1 != a.end(); it1++)
		{
			for (it2=a.begin(); it2 != a.end(); it2++)
			{
				if (*it1 != *it2)
				{
					pair<int,int>t1 = *it1;
					pair<int,int>t2 = *it2;

					int p1 = current_pattern[i].graph[t1.first][t1.second];
					int p2 = current_pattern[i].graph[t2.first][t2.second];

					if (t1.first == t2.first)
					{
						pattern[i].linegraph[p1][p2] = SS;
						pattern[i].linegraph[p2][p1] = SS;
					}
					else if (t1.second == t2.first)
					{
						pattern[i].linegraph[p1][p2] = OS;
						pattern[i].linegraph[p2][p1] = SO;
					}
					else if (t1.first == t2.second)
					{
						pattern[i].linegraph[p1][p2] = SO;
						pattern[i].linegraph[p2][p1] = OS;
					}
					else if (t1.second == t2.second)
					{
						pattern[i].linegraph[p1][p2] = OO;
						pattern[i].linegraph[p2][p1] = OO;
					}
				}
			}
		}
	}
}

void form_productgraph()  // 如果只有两个标签相同的点能形成product graph，那么就不会存在size指数增长的问题
// 序号直接表示p的标号
{
	int begin = 1;
	for (int i = 1; i < pattern_num - 1; i++)
	{
		vector<int>p1;
		vector<int>p2;
		if(begin)
		{
			p1 = pattern[0].p;
			p2 = pattern[1].p;
			begin = 0;
		}
		else
		{
			p1 = productgraph_p;
			p2 = pattern[i+1].p;
		}
		vector<int>common;
		common.clear();
		 /*vector<int>::iterator result = find( L.begin( ), L.end( ), 3 ); //查找3
    if ( result == L.end( ) ) //没找到
        cout << "No" << endl;
    else //找到
        cout << "Yes" << endl;*/
 
        for (int j = 0; j < p1.size(); j++)
        {
        	vector<int>::iterator result = find(p2.begin(), p2.end(), p1[j]);
        	if (result != p2.end())
        	{
        		common.push_back(p1[j]);
        	}
        }
        productgraph_p.clear();
        for (int j = 0; j < common.size(); j++)
        {
        	for (int k = 0; k < common.size(); k++)
        	{
        		if (j != k)
        		{
        			int pred1 = common[j];
        			int pred2 = common[k];
        			if(pattern[i].linegraph[pred1][pred2] == pattern[i+1].linegraph[pred1][pred2] && pattern[i].linegraph[pred1][pred2] != -1)
        			{
        				temp_productgraph[pred1][pred2] = pattern[i].linegraph[pred1][pred2];
        				productgraph_p.push_back(pred1);
        				productgraph_p.push_back(pred2);
        			}
          		}
        	}
        } // product graph还是line graph
        swap();
        memset(temp_productgraph,-1,sizeof(temp_productgraph));
	}
}	

int bestx[100]; // 当前最优解
int x[100]; //顶点与当前团的连接，x[i] = 1表示有连接
int cn = 0;
int bestn = 0;
memset(bestx,0,sizeof(bestx));
memset(x,0,sizeof(x));

void BackTrace(int i)
{
	if (i > 99)  // 100 应该是精确的连通图的点的个数
	{
		for (int j = 0; j < 100; j++)
			bestx[j] = x[j];
		nestn = cn;
		return;
	}
	int ok = 1;
	for (int j = 0; j < i; j++)
	{
		if(x[j] && productgraph[i][j] == 0)
		{
			ok = 0;
			break;
		}
	}
	if(ok)
	{
		x[i] = 1;
		cn++;
		BackTrace(i+1);
		x[i] = 0;
		cn--;
	}
	if(cn + 100 - i > bestn) // 100应该换成graph.v, 100是更弱的剪枝
	{
		x[i] = 0;
		BackTrace(i+1);
	}
}

// 最后看x[i] = 1, 就表示标号为i的pred在最大公共子结构中

void clique_detection() //productgraph[100][100]找团，团中点就是p
// 仅适用于团上点数较少的（小于100个）
{
	BackTrace(0);
}

int main(char* argv[])
{
	Init();
	my_cin(); // 要求：所有pattern里p都是经过局部映射（仅考虑pattern内部）的，从0开始
	form_linegraph();
	form_productgraph();
	clique_detection();

	return 0;
}