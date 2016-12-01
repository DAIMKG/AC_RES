#include<iostream>
#include<fstream>
#include<map>
#include<set>
#include<vector>
#include<string>
#include<string.h>
#include<cstring>
#include<algorithm>
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
		map<string,int>p_to_s; // for convenience to form queries after finding the maximal common substructure
		map<string,int>p_to_o;
}pattern[10];


int pattern_num;
int vertice_num;
int edge_num;
int productgraph[100][100];
int temp_productgraph[100][100];
map<string,int>pred_to_int;
map<int,string>int_to_pred;
Pattern current_pattern;

vector<int>productgraph_p;

void Init()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			for (int k = 0; k < 100; k++)
			{
				pattern[i].graph[j][k] = -1;
				pattern[i].linegraph[j][k] = -1;
			}
		}
	
		pattern[i].v_num = 0;
		pattern[i].e_num = 0;
	}
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

/*
input:
2

2 // 点数
1 variable
2 variable
1 // 边数
1 2 <has_currency>

2  // 点数
1 variable
2 variable
1 // 边数
1 2 <has_currency>
*/

void my_cin(char* infile)
{
	ifstream fin(infile);
	fin >> pattern_num;
	int pred = 0;
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
			string predicate;	

			fin >> from >> to >> predicate;
			(pattern[i].p_to_s)[predicate] = from;
			(pattern[i].p_to_o)[predicate] = to;

			if(pred_to_int.count(predicate) == 0)
			{
				pred_to_int[predicate] = pred;
				int_to_pred[pred] = predicate;
				pattern[i].graph[from][to] = pred; 
				pred++;
			}
			else
				pattern[i].graph[from][to] = pred_to_int[predicate];

			(pattern[i].p).push_back(pred_to_int[predicate]);
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
			for (int k = 0; k < t; k++)
			{
				if (current_pattern.graph[j][k] != -1)
				{
					a.insert(pair<int,int>(j,k));
				}
			}
		}

		cout << "preds for pattern " << i << " that we have:";
		for(set<pair<int,int> >::iterator si = a.begin(); si != a.end(); si++)
			cout <<"(" <<(*si).first << "," << (*si).second << ") ";
		cout << endl;
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

					int p1 = current_pattern.graph[t1.first][t1.second]; // 这是全局的p
					int p2 = current_pattern.graph[t2.first][t2.second];

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

		cout << "pattern" << i << "'s linegraph is:" << endl;
		for (int u = 0; u < (pattern[i].p).size(); u++)
		{
			for (int v = 0; v < (pattern[i].p).size(); v++)
			{
				cout << pattern[i].linegraph[u][v] << " ";
			}
			cout << endl;
		}
	}
}
// 至此，都是对的

void form_productgraph()  // 如果只有两个标签相同的点能形成product graph，那么就不会存在size指数增长的问题
// 序号直接表示p的标号
{
	int begin = 1;
	for (int i = 1; i < pattern_num ; i++)
	{
		vector<int>p1;
		vector<int>p2;
		if(begin)
		{
			p1 = pattern[0].p;
			p2 = pattern[1].p;
			begin = 0;
			for (int u = 0; u < 100; u++)
				for (int v = 0; v < 100; v++)
					productgraph[u][v] = pattern[0].linegraph[u][v];
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
        cout << "common pred number is " << common.size() << endl;
        for (int j = 0; j < common.size(); j++)
        {
        	for (int k = 0; k < common.size(); k++)
        	{
        		if (j != k)
        		{
        			int pred1 = common[j];
        			int pred2 = common[k];
        			if(productgraph[pred1][pred2] == pattern[i].linegraph[pred1][pred2] && pattern[i].linegraph[pred1][pred2] != -1)
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
	cout << "the product graph is: " << endl;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			cout << productgraph[i][j] << " ";
		}
		cout << endl;
	}
}	

int bestx[100]; // 当前最优解
int x[100]; //顶点与当前团的连接，x[i] = 1表示有连接
int cn = 0;
int bestn = 0;

//memset(bestx,0,sizeof(bestx)); memset cannot be called outside a function
//memset(x,0,sizeof(x));

void Init2()
{
for (int i = 0; i < 100; i++) 
	bestx[i] = 0;
for (int i = 0; i < 100; i++) 
	x[i] = 0;
}

void BackTrace(int i)
{
	if (i > 50)  // 100 应该是精确的连通图的点的个数
	{
		for (int j = 0; j < 100; j++)
			bestx[j] = x[j];
		bestn = cn;
		return;
	}
	int ok = 1;
	for (int j = 0; j < i; j++)
	{
		if(x[j] && productgraph[i][j] == -1)
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
	if(cn + 50 - i > bestn) // 100应该换成graph.v, 100是更弱的剪枝
	{
		x[i] = 0;
		BackTrace(i+1);
	}
}

// 最后看x[i] = 1, 就表示标号为i的pred在最大公共子结构中

void clique_detection() //productgraph[100][100]找团，团中点就是p
// 仅适用于团上点数较少的（小于100个）
{
	Init2();
	BackTrace(0);
}

void my_out(char* outfile)
{
	ofstream fout(outfile);
	for (int i = 0; i < 100; i++)
	{
		if (bestx[i] == 1)  //  最后留下来的pred还需要verify一下是否在所有pattern里的p，s部分都是variable或者都是blank nodes
		{
			int mark = 1; // 这个p是否要保留
			int type_s; // type = 0: variable
			//type = 1: blank nodes
			int type_o;
			int o = pattern[0].p_to_o[int_to_pred[i]];
			vector<int>::iterator iter = find((pattern[0].variable).begin(), (pattern[0].variable).end(), o);
			if (iter != (pattern[0].variable).end())
				type_o = 0;
			else
				type_o = 1;

			int s = pattern[0].p_to_s[int_to_pred[i]];
			vector<int>::iterator iter2 = find((pattern[0].variable).begin(), (pattern[0].variable).end(), s);
			if (iter2 != (pattern[0].variable).end())
				type_s = 0;
			else
				type_s= 1;

			for (int j = 1; j < pattern_num; j++)
			{
				o = pattern[i].p_to_o[int_to_pred[i]];
				s = pattern[i].p_to_s[int_to_pred[i]];
				if (type_o == 0 && find((pattern[i].variable).begin(), (pattern[i].variable).end(), o) == (pattern[i].variable).end())
				{
					mark = 0;
					break;
				}
				if (type_o == 1 && find((pattern[i].variable).begin(), (pattern[i].variable).end(), o) != (pattern[i].variable).end())
				{
					mark = 0;
					break;
				}
				if (type_s == 0 && find((pattern[i].variable).begin(), (pattern[i].variable).end(), o) == (pattern[i].variable).end())
				{
					mark = 0;
					break;
				}
				if (type_s == 1 && find((pattern[i].variable).begin(), (pattern[i].variable).end(), o) != (pattern[i].variable).end())
				{
					mark = 0;
					break;
				}
			}
			if (mark)
			{
				string final_p = int_to_pred[i];
				fout << (pattern[0].p_to_s)[final_p] << " " << final_p << " " << (pattern[0].p_to_o)[final_p] << endl;
			}

		}
	}

}
int main(int argc, char* argv[])
{
	Init();
	my_cin(argv[1]); // 要求：所有pattern里p都是经过局部映射（仅考虑pattern内部）的，从0开始
	form_linegraph();
	form_productgraph();
	clique_detection();
	for (int i = 0; i < 100; i++)
		if (bestx[i] == 1)
			cout << i << endl;
	my_out(argv[2]);

	return 0;
}