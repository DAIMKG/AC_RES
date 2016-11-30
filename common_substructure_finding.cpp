#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<set>
#include<sstream>
#include<vector>
#include<string.h>
#include<cstring>
#include<algorithm>
#include<utility>

using namespace std;

map <string,int>pred_to_int; // mapping each predicate to a integer
map <int,string>int_to_pred; // mapping a integer to each predicate
// 最后根据int值转成predicate的字符串，形成SPARQL语句的时候要用到

map <string,int>pred_num;
map <string,int>pred_max;  // for every predicate, it has an object with the max index, find the max index

map <int, int>obj_num;
map <int, double> selectivity_e1;
map <int,double> selectivity_e2;
map <string,double>selectivity_pred;
map<string, int*>pred_histo;
int adj_matrix[20][10][10];
vector<int> group_[20]; // the result of K-means, each group stores a vector of the gfd id who belongs to this group
vector<string>all_p_in_gfds;
int distinct_sub = 0;
int gfd_num;


class GFD
{
private:
	double cost;
	int id; // got in the p_index.txt file
	int group;
	vector<double>feature_vector;  // record the predict_index in a GFD; dimension is |p| in all GFDs
	// change from 'int' to 'double' for convenience

	vector<string>predicates; // indicate all the predicates in the GFD
	vector<vector<string> >pattern; // indicate a graph pattern
	//int adj_matrix;
public:
	double get_cost(){return this -> cost;}
	int get_id(){return this -> id;}
	int get_group() {return this -> group;}
	vector<string> get_pred(){return this -> predicates;}
	vector<double> get_feature_vector(){return this -> feature_vector;}
	vector<vector<string> >get_pattern(){return this -> pattern;}
	//int **get_matrix(){return this -> adj_matrix;}

	void set_cost(double co){cost = co;}
	void set_id(int i){id = i;}
	void set_group(int g){group = g;}
	//void set_matrix(int i, int j, int k){adj_matrix[i][j] = k;}
	//void load_matrix(int**a){adj_matrix = a;}

	void add_pred(string pred){this -> predicates.push_back(pred);}
	void add_pattern(vector<string> s) {this->pattern.push_back(s);}

	GFD(){cost = 0; id = 0; group = 0;}
	GFD(double co, int id_, int group_)
	{
		cost = co;
		id = id_;
		group = group_;
	}
}gfd[100];

double get_dis(vector<double>v1, vector<double>v2, int dimen)
{
	double result = 0;
	for (int i = 0; i < dimen; i++)
		result += (v1[i] - v2[i])*(v1[i] - v2[i]);
	return result;
}

void pred_map()  // fill in the pred_to_int vector
{
	ifstream fin("reduced_graph_pred_map.txt");
	string p;
	int id;
	while (fin >> p >> id)
	{
		pred_to_int[p] = id;
		int_to_pred[id] = p;
	}
}

void GFD_clustering(int k) // output: put every gfd in the right group
{

    /*input: a lot of graph patterns |sigma|
     * GFD的格式：
     * output: a
     * */
	// kmeans

	//step1: for every GFD pattern, generate a 0-1 feature vector
	int max_dimension = 0;

	/*for (int i = 1; i <= num; i++)
	{
		int dimension = gfd[i].get_pred().size();
		if (dimension > max_demension)
			max_demension = dimension;
	}*/
	max_dimension = all_p_in_gfds.size();
	map<string,int>mapper;
	int index = 0;
	for (int i = 0; i < max_dimension; i++)
	{
		mapper[all_p_in_gfds[i]] = index;
		index++;
	}
	for (int i = 1; i <= gfd_num; i++)  // initialize feature_vector for every gfd
	{
		for (int j = 0; j < max_dimension; j++)
			gfd[i].get_feature_vector()[j] = 0;
	}
	for (int i = 1; i <= gfd_num; i++)
	{
		vector<string>p = gfd[i].get_pred();
		int pred_n = p.size();
		for (int j = 0; j < pred_n; j++)
			gfd[i].get_feature_vector()[mapper[p[j]]] = 1;
	}

	//step2: according to the feature vecotr and K, do K-means
	// data structure
	//vector<int>group[20];   k must be less than 20, group[i]indicates the number of gfds belonging to the i-th group
	// vector<int>center[20] ; in each group, there is an average_vector to indicate the center of the group(cluster)

	vector<double> center[20];
	int change = 1;
	// Initialize, the initial centers is the first k gfds
	for (int i = 0; i < k; i++)
	{
		center[i] = gfd[i].get_feature_vector();
	}
	//int temp = group[3];
	while (change != 0)
	{
		change = 0;
		for(int i = 1; i <= gfd_num; i++)
		{
			// 对每一个gfd，都要找离哪一个center最近
			vector<double>feature = gfd[i].get_feature_vector();
			double min_dis = 100000;
			int mark = 0;
			for (int j = 0 ; j < k; j++)
			{
				double dis = get_dis(feature,center[j],max_dimension);
				if (dis < min_dis)
				{
					min_dis = dis;
					mark = j;
				}
			}
			if (gfd[i].get_group() != mark)
				change++;
			gfd[i].set_group(mark);
			group_[mark].push_back(i);
		}

		for (int i = 0; i < k; i++)
		{
			int x = group_[i].size();
			vector<double>center_;
			for (int w = 0; w < max_dimension; w++)
			{
				double tmp = 0;
				for (int j = 0; j < x; j++)
				{
					vector<double> a;
					vector<int>tmp_ = group_[i];
					int ind = tmp_[j];
					a = gfd[ind].get_feature_vector();
					tmp += a[w];
				}
				tmp = tmp / x;
				center_.push_back(tmp);
			}
			center[i] = center_;
		}
	}
}


void statistical_calculator(char* a,char* b) // a is the reduced graph dataset, b is the pre-computed "pred + num .txt"
{
	ifstream fin(a), fin3(a);
	int e1, e2;
	string pred;
	//int mark[]
	long graph_size = 0;
	while (fin >> e1 >> pred >> e2)
	{
		graph_size++;
	}

	ifstream fin2(b);
	int num;
	while(fin2 >> pred >> num)
	{
		pred_num[pred] = num;
		pred_histo[pred] = new int[10];
		for (int i = 0; i < 10; i++)
			pred_histo[pred][i] = 0;
		selectivity_pred[pred] = num * 1.0 / graph_size;
	}

	int mark[graph_size + 5];
	memset(mark,0,sizeof(mark));

	while(fin3 >> e1 >> pred >> e2) // calculate the number of distinct subjects
	// at the same time, calculate the histogram for a given pred;
	{
		if (mark[e1] == 0) {distinct_sub++; mark[e1] = 1;}
		if (mark[e2] == 0) {distinct_sub++; mark[e2] = 1;}
		pred_histo[pred][e2*10 / pred_num[pred]]++;
	}

}


double selectivity(int e1, string pred, int e2)
{
	//return (1.0 / distinct_sub ) * selectivity_pred[pred] * (pred_histo[pred][e2 * ]
	return selectivity_pred[pred];
}

void substructure_filtering(int grou)  // 对第grou个group里所有的gfd用graph simulation的思想进行substructure_filtering
{
	// goal: filter out the sub-structure that does not contribute to the common structure
	// 第一步：只留下那些在所有的gfd里都出现的predicate
	//
	map<string, int>p_time;
	vector<int>gfd_id = group_[grou];
	int mark[100];
	int filter[100];
	memset(filter,0,sizeof(filter));
	memset(mark,0,sizeof(mark));

	int size_ = group_[grou].size();  // 这个里面有多少个gfd－－size
	for (int i = 0; i < size_; i++) // 对每一个gfd
	{
	    int tem = gfd_id[i];
	    vector<string>temp_pred = gfd[tem].get_pred();
	    for (int j = 0; j < temp_pred.size(); j++)
	    {
	    	string pp = temp_pred[j];  // cannot use: if ( p_time.count(temp_pred[j]) == 0)
	    	/*if (p_time.count(pp) == 0)
	    		p_time[temp_pred[j]] = 1;
	    	else
	    		p_time[temp_pred[j]]++;*/
	    	if (mark[pred_to_int[pp]] == 0)
	    	{
	    		mark[pred_to_int[pp]] = 1;  // 表示这个p已经被处理过了
	    		for (int k = 0; k < size_; k++)
	    		{
	    			if (k != i)
	    			{
	    				// 这个p要在其他所有的gfd里面都出现过
	    				int other_gfd=gfd_id[i]; // the i-th gfd in this group
	    				 vector<string>temp_pred2 = gfd[other_gfd].get_pred();
	    				 //if (temp_pred2.find(pp) == 0)
	    				 if ( find(temp_pred2.begin(), temp_pred2.end(), pp) != temp_pred2.end())
	    				 {
	    					 filter[pred_to_int[pp]] = 1;
	    					 break;
	    				 }
	    			}
	    		}
	    	}
	    }
	}

	for (int i = 0; i < size_; i++)
	{
		int tem = gfd_id[i];
		for (int j = 0; j < 10; j++)
			for (int k = 0; k < 10; k++)
			{

				if(filter[adj_matrix[i][j][k]] == 1)
				{
					adj_matrix[i][j][k] = 0;
				}
			}
	}
	/*for (int i = 0; i < size_; i++)  //对于同一个group里的所有gfd
	{
		int tem = gfd_id[i];
	    vector<string>temp_pred = gfd[tem].get_pred();
	    for (int j = 0; j < temp_pred.size(); j++)
	    {
	    	if (p_time[temp_pred[j]] == 1)
	    	{
	    		//int matrix[10][10];   // 这个gfd的matrix的对应的值为0
	    		matrix = gfd[tem].get_matrix();
	    		for (int s = 0; s < 10; s++)
	    			for (int t = 0; t < 10; t++)
	    			{
	    				if (matrix[s][t] == pred_to_int[temp_pred[j]])
	    					matrix[s][t] = 0;
	    			}
	    		gfd[tem].load_matrix(matrix);
	    	}
	    }
	}*/

}

void common_structure_finding(int grou)
{
   vector<int>gfd_id = group_[grou];
   map<int,int>node_label[20]; // 这个group里，假设总共有20个gfd的pattern
   //node_label[i]表示该group里的第i个gfd（从0开始标号）

   int adj_matrix_p_label[20][20][20]; // 这个临接矩阵记录adj_matrix[i][j]里的（i，j）和pred不重复的重新标号的对应
   int linegraph_adj_matrix[20][20][20];

   memset(adj_matrix_p_label,0,sizeof(adj_matrix_p_label));
   memset(linegraph_adj_matrix,0,sizeof(linegraph_adj_matrix));

   int size_ = gfd_id.size();
   if (size_ > 1)  // only merge when multiple graphs
   {
	   // linegraph building
	   // build a linegraph for every GFD pattern
	   for (int i = 0; i < size_; i++)  // ｜gfd| in this group
	   {

		   // 对第i个gfd，用node_label[i]这个map来维护，重新标号；
		   int gfd_ = gfd_id[i];  // 该gfd的标号是gfd_
		   int node_num = gfd[gfd_].get_pred().size();
		   //for (int j = 0; j < node_num; j++)
			   //node_label[i][j] = gfd[gfd_].get_pred()[j];
		   // node_label<int,string>: int is different,
		   //but string can be the same(indicate the same predicates in the gfd pattern)

		   set<pair<int,int> >a;
		   int label_cnt = 0;
		   for (int j = 0; j < 10; j++)
		   {
			   for (int k = 0; k < 10; k++)
			   {
				   if (adj_matrix[gfd_][j][k]!= 0)
				   {
					   node_label[i][label_cnt] = adj_matrix[gfd_][j][k]; // 对每一个gfd的临接矩阵中的p，从0开始，从左往右
					   //至上而下标号，map到一个int值，这个int值是全局的predicate的一一对应的整数值
					   // 以后都只用这个整数值了，最后一步再转

					   a.insert(pair<int,int>(j,k));
					   label_cnt++;
				   }
			   }
		   }
		   if (node_num != a.size())
		   {
			   cout << "err: mistakes in pred_num counting in gfd" << endl;
		   }
		   //pair<int,int>aa;
		   //aa.first=1;
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
					   if (t1.first == t2.first)
					   {
						   	  //int p1_label = adj_matrix[gfd_][t1.first][t1.second];  // 以后一定要写注释，我已经看不懂了。。。
						   	  // 这里p1_label指的是重新映射过的不重复的，从0开始的值
						   	  //int p2_label = adj_matrix[gfd_][t2.first][t2.second];
						      int p1_label = it1-a.begin();
						      int p2_label = it2-a.begin();
						      linegraph_adj_matrix[i][p1_label][p2_label] = 1; // ss
						      linegraph_adj_matrix[i][p2_label][p1_label] = 1;
					   }
					   else if (t1.second == t2.first)
					   {
						   int p1_label = it1-a.begin();
						   int p2_label = it2-a.begin();
    					   linegraph_adj_matrix[i][p1_label][p2_label] = 2; // os
						   linegraph_adj_matrix[i][p2_label][p1_label] = 3; // so
					   }
					   else if (t1.first == t2.second)
					   {
						   int p1_label = it1-a.begin();
						   int p2_label = it2-a.begin();
						   linegraph_adj_matrix[i][p1_label][p2_label] = 3; // so
						   linegraph_adj_matrix[i][p2_label][p1_label] = 2; // os
					   }
					   else if (t1.second == t2.second)
					   {
						   int p1_label = it1-a.begin();
						   int p2_label = it2-a.begin();
						   linegraph_adj_matrix[i][p1_label][p2_label] = 4; // oo
						   linegraph_adj_matrix[i][p2_label][p1_label] = 4;  // oo
					   }
				   }
			   }
		   }
	   }

	   //product graph building


	   //maximal clique finding
   }
}

void query_rewrite()
{
     // output a formal SPARQL query
}

void form_query(int i)  // 第i个group不用找union，直接分下来
{
	vector<int>gfds = group_[i];
	int s = gfds.size();
	for (int i = 0; i < s; i++)
	{

	}
}

void cost_calculator()
{
	for (int i = 1; i <= gfd_num; i++)
	{
		vector<string>p = gfd[i].get_pred();
		double min = 1;
		int x = p.size();
		for (int j = 0; j < x; j++)
			if(selectivity_pred[p[j]] < min)
				min = selectivity_pred[p[j]];
		gfd[i].set_cost(min);
	}
}

int main(int argc, char *argv[])
// argv[3]: int, to indicate the number of GFDs (we think that the file name is p_x.txt, x >= 1)
// argv[4]: K-value is K-means
// for a small dataset of 47k, I want to generate 10 gfds and classify them into 3 clusters
{
	 //ifstream fin1("pred_map.txt");
	 pred_map();
	 string pred;
	 int index;
	 gfd_num=argv[3]-"0";
	 //step1: read in the GFDs(only patterns) and construct a gfd class for every gfd pattern
	 memset(adj_matrix,0,sizeof(adj_matrix));
	 for (int i = 1; i <= gfd_num; i++)
	 {
	 		stringstream ss;
	 		ss << i;
	 		string str;
	 		ss >> str;  // this is used to convert int i to string i for use in the file name

	 		ifstream fin("p_" + str + ".txt");
	 		string x, y;
	 		string pred;
	 		gfd[i].set_cost(0);
	 		gfd[i].set_id(i);
	 		gfd[i].set_group(i);
	 		while(fin >> x >> pred >> y)
	 		{
	 			int temp_x = x.c_str() - "a";  // first convert to char*, then minus
	 			int temp_y = y.c_str() - "a";
	 			gfd[i].add_pred(pred);
	 			all_p_in_gfds.push_back(pred);
	 			vector<string>triple_temp;
	 			triple_temp.push_back(x);
	 			triple_temp.push_back(pred);
	 			triple_temp.push_back(y);
	 			gfd[i].add_pattern(triple_temp);
	 			//gfd[i].set_matrix(temp_x,temp_y,pred_to_int[pred]);
	 			adj_matrix[i][temp_x][temp_y] = pred_to_int[pred];
	 		}
	 		// to do: if (has_attribute) change pattern and dependency
	 		fin.close();
	 }

	 GFD_clustering(argv[4]-"0"); // step2: cluster the gfd patterns by k-means

	 //Step2: refine query clusters
	// for (every cluster)
	// {
	//	  Initialize |this cluster| groups, each query is a group; the set is denoted as C = {Ci,Cj,...}
	// 	  while (exist untestes pair with max_Jaccard(Ci, Cj)) do  // we need to calculate the Jaccard online
	//    {
	//          calculate the union of Ci \/ Cj;
	//			if (cost(Ci) + cost(Cj) > cost(Ci \/ Cj)
	//			{remove Ci,Cj from C and add Ci \/ Cj}
	//			else (tested(Ci, Cj))
	//    }
	//	  for the final set C, find the common structures for every Ci group
	//	  for every Ci, rewrite the query using OPTIONAL
	// }
	// For simplicity, just for each cluster, find the common structures and rewrite the query
	for (int i = 0; i < argv[4] - "0"; i++)
	{
		//first implement a simple version that: if the cost as a whole is smaller, that's all right!
		// if the sum of seperate cost is larger, the use the queries one by one, mark it as un-united.
		int cluster_size=group_[i].size();
		substructure_filtering(i);
		common_structure_finding(i);
		//double cost_whole : common structure 里predicate的sel的和
		double cost_whole = 0;
		double cost_seperate = 0;
		for (int j = 0; j < cluster_size; j++)
		{
			vector<int>tmp_ = group_[i];
			int ind_ = tmp_[j];
			cost_seperate += gfd[ind_].get_cost();
		}
		if (cost_seperate < cost_whole)
			query_rewrite();
		else
			form_query(i);
	}

	return 0;
}


