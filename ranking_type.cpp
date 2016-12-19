#include<iostream>
#include<string>
#include<map>
#include<fstream>
using namespace std;
int main(int argc, char* argv[])
{
	ifstream fin1(argv[1]); // 所有的type文件
	ifstream fin2(argv[2]); // 需要排序的小type文件
	ofstream fout(argv[3]); // 输出排序完的小type文件

	map<string,int>type_to_f;
	string type;
	int num_type;
	int f;
	while(fin1 >> type >> f)
	{
		//cout << "hello" << endl;
		type_to_f[type] = f;
		//cout << type_to_f[type] << endl;
	}
	while(fin2 >> num_type)
	{
		cout << num_type << endl;
		fout << num_type << endl;
		string str[100];
		for (int i = 0; i < num_type; i++) // 对每一个column的type都排序
		{
			fin2 >> type;
			str[i] = type;
		}
		for (int i = 0; i < num_type - 1; i++)
			for (int j = 0; j < num_type - i - 1; j++)
			{
				//cout << str[j] << " " << str[j+1] << endl;
				//cout << type_to_f[str[j]] << " " << type_to_f[str[j+1]] << endl;
				if (type_to_f[str[j]] < type_to_f[str[j+1]])
				{
					
					cout << "str[j]="<<str[j] << " " << "str[j+1]=" << str[j+1] << endl;
					string temp = str[j];
					str[j] = str[j+1];
					str[j+1] = temp;
				}
			}	
		for (int i = 0; i < num_type; i++)
		{
			fout << str[i] << endl;
		}
	}

	return 0;
}