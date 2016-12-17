#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<map>
#include<vector>

using namespace std;

// original user input: input_2.txt, from which I read the number of columns argv1

//input common substructure: outfile argv2

//input common attributes: SameFacts_2.txt argv3
//input common types: SameType_2.txt argv4
// in SameFacts_2.txt: predicates

// ./naive_sparql_forming input_2.txt outfile SameFacts_2.txt SameType_2.txt infile
string prefix1 = "PREFIX rdf:    <http://www.w3.org/1999/02/22-rdf-syntax-ns#>\n";
string prefix2 = "PREFIX base: <http://yago-knowledge.org/resource/>\n";

int main(int argc, char* argv[])
{
	int column_num = 0;
	int node_num = 0;
	int new_variable = 100; // 路径上非用户查询的变量使用？100...及以后的代替即可
	int s, column_s, o, column_o;
	string pred;
	map<int, int>node_to_column;
	map<int, int>column_to_node;
	//ifstream fin1("/Users/serena/Desktop/AC_RES-master/test/input_2.txt");
	ifstream fin1(argv[1]);
	ifstream fin2(argv[2]);
	ifstream fin3(argv[3]);
	ifstream fin4(argv[4]);
	ifstream fin5(argv[5]);
	ofstream fout("sparql_1.txt");
	// 先输出一个sparql试试看, 最终为了读取方便，要一个sparql一个文件，而不是把所有sparql都搞在一个文件里
	fout << prefix1;
	fout << prefix2;
	fout << "SELECT ";

	fin1 >> column_num;
	fin5 >> node_num;
	for (int i = 0; i < node_num; i++)
	{
		int a, b, c;
		fin5 >> a >> b;
		if (b == 0) 
		{
			fin5 >> c;
			column_to_node[c] = a;
		}
		
	}
	for (int i = 0; i < column_num; i++)
	   fout << "?" << column_to_node[i] << " ";
	fout <<"WHERE { "; 
	while(fin2 >> s) // 读入一行
	{
		if (s != -1) // s is a variable
		{
			int temp;
			fin2 >> temp;
			node_to_column[s] = temp;
			fout << "?" << s << " ";
		}
		else
		{
			fout <<"?" << new_variable << " ";
			new_variable++; 
		}
		fin2 >> pred >> o;
		fout << pred << " ";
		if (o != -1)
		{
			int temp2;
			fin2 >> temp2;
			node_to_column[o] = temp2;
			fout << "?" << o << " ";
		}
		else
		{
			fout << "?" << new_variable << " ";
			new_variable++;
		}
		fout << "\t" << "." << "\n";
	}

	//deal with common attributes
	int col = 0;
	int num_attributes;
	//"<"+str.substr(3) --> 复原成原来的

	while (fin3 >> num_attributes)
	{
		for (int i = 0; i < num_attributes; i++)
		{
			string str1, str2;
			fin3 >> str1 >> str2;
			if (str1[1] == '_')
			{
				fout << str2 << " " << "<"+str1.substr(3) <<" " << "?" << column_to_node[col] << "\t" << ".\n";
				// 最终的sparql统一按照node的标号来执行  
			}
			else
				fout << "?" << column_to_node[col] << " " << str1 << " " << str2 << "\t" << ".\n";
		} 
		col++;
	}

	//deal with common types
	int num_types;
	col = 0;
	while(fin4 >> num_types)
	{
		for (int i = 0; i < num_types; i++)
		{
			string t;
			fin4 >> t;
			fout << "?" << column_to_node[col] << " rdf:type " << t << "\t" << ".\n";
		}
		col++;
	}
	fout << "}";
	return 0;
}

