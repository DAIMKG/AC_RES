#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

const string DataDir = "/Users/wzc/Downloads/yagoSimpleTypes.tsv";
const string InputDir = "/Users/wzc/Desktop/lab/test/input.txt";
const string OutputDir = "/Users/wzc/Desktop/lab/test/SameType.txt";

const int MAXL = 17000000;
const int MAXN = 10000;

vector<pair<string, string> > AllType(MAXL);
vector<vector<set<string> > > myType;


void init() {
	FILE *fp = fopen(DataDir.c_str(), "r");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%s%*s%s", tmp1, tmp2)) {
		AllType.push_back(make_pair(tmp1, tmp2));
		if((++show) % 100000 == 0) {
			fprintf(stdout, "%d\n", show);
		}
	}

	delete [] tmp1;
	delete [] tmp2;
	fclose(fp);
}


void go() {
	FILE *fp = fopen(InputDir.c_str(), "r");

	int num;
	char *tmp = new char[MAXN];
	while(~fscanf(fp, "%d", &num)) {
		vector<set<string> > t;
		for(int i = 1; i <= num; i++) {
			fscanf(fp, "%s", tmp);

			set<string> tt;
			for(int j = 0; j < AllType.size(); j++)
				if(AllType[j].first == (string)tmp)
					tt.insert(AllType[j].second);
			if(tt.size() == 0) {
				fprintf(stderr, "Entity not found!\n");
				exit(1);
			}
			t.push_back(tt);
		}
		myType.push_back(t);
	}

	fclose(fp);
}

void findSame() {
	FILE *fp = fopen(OutputDir.c_str(), "w");

	int n = myType.size(), m = myType[0].size();


	for(int i = 0; i < m; i++) {
		set<string> sameType;
		sameType = myType[0][i];


		for(int k = 1; k < n; k++) {
			set<string> in;
			set_intersection(sameType.begin(), sameType.end(), myType[k][i].begin(), myType[k][i].end(), inserter(in, in.begin()));
			sameType = in;
		}

		fprintf(fp, "%lu\n", sameType.size());
		for(set<string>::iterator it = sameType.begin(); it != sameType.end(); it++)
			fprintf(fp, "%s\n", (*it).c_str());
		fprintf(fp, "\n");
	}

	fclose(fp);
}

int main() {

	init();
	fprintf(stdout, "init() finished!\n");
	go();
	fprintf(stdout, "go() finished!\n");
	findSame();

	return 0;
}
