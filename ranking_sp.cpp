#include<iostream>
#include<fstream>
#include<map>
#include<string>
using namespace std;
int main(int argc, char* argv[])
{
	ifstream fin1(argv[1]); // 所有的(sp,num)文件
	ifstream fin2(argv[2]); // (po,num)
	ifstream fin3(argv[3]); // 需要排序的小attributes文件
	ofstream fout(argv[4]); // 输出排序完的小attributes文件
	
	map<string,int>sp_to_f;
	map<string,int>po_to_f;

	string s,p,o;
	int f;

	while(fin1 >> s >> p >> f)
	{
		string str = s+" "+p;
		sp_to_f[str] = f;
		cout << str << " " << f << endl;
	}
	while(fin2 >> p >> o >> f)
	{
		string str = p+" "+o;
		po_to_f[str] = f;
		cout << str << " " << f << endl;
	}
	cout << "tag1: ended reading big files" << endl;
	int num_attributes;
	string s1, s2;
	while(fin3 >> num_attributes)
	{
		string temp[300];
		map<string,int>mark; // mark = 0: (s,p);mark = 1(p,o)
		map<string,string>natural_to_mark;
		fout << num_attributes << endl;
		for (int i = 0; i < num_attributes; i++)
		{
			fin3 >> s1 >> s2;
			if (s1[1] == '_')
			{
				string tt = s2 + " <" + s1.substr(3);
				natural_to_mark[tt] = s1+ " " +s2;
				temp[i] = tt;
				mark[temp[i]] = 0;
			}
			else
			{
				temp[i] = s1 + " "+s2;
				mark[temp[i]] = 1;
			}
		}
		for (int i = 0; i < num_attributes; i++)
			cout << "temp[i]=" << temp[i] << endl;
		for (int i = 0; i < num_attributes - 1; i++)
		{
			for (int j = 0; j < num_attributes - i - 1; j++)
			{
				int f_a, f_b;
				if (mark[temp[j]] == 0) 
				{
					f_a = sp_to_f[temp[j]];
					cout << temp[j] << " " << f_a <<" " << "and the frequency is " << f_a << endl;
				}
				else
				{
					f_a = po_to_f[temp[j]];
					cout << temp[j] << " " << f_a << endl;
				}
				if (mark[temp[j+1]] == 0)
				{
					f_b = sp_to_f[temp[j+1]];
					cout << temp[j+1] <<  " " << f_b << endl;
				}
				else
				{
					f_b = po_to_f[temp[j+1]];
					cout << temp[j+1] << " " << f_b << endl;
				}

				if (f_a < f_b)
				{
					string t = temp[j];
					temp[j] = temp[j+1];
					temp[j+1] = t;
				}
			}
		}
		for (int i = 0; i < num_attributes; i++)
		{
			if (mark[temp[i]] == 0)
				fout << natural_to_mark[temp[i]] << endl;
			else
				fout << temp[i] << endl;
		}
	}
	return 0;
}