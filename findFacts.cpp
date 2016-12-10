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

const int MAXL = 11300000;
const int MAXN = 10000;

struct Fact {
	string s, p, o;
	Fact(){}
	Fact(string s_, string p_, string o_):s(s_), p(p_), o(o_){}
};

vector<Fact> AllFacts(MAXL);
vector<vector<set<pair<string, string> > > > myType;

void init() {
	FILE *fp = fopen("/Users/wzc/Downloads/newFacts.tsv", "r");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN], *tmp3 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%s%s%s", tmp1, tmp2, tmp3)) {
		AllFacts.push_back(Fact(tmp1, tmp2, tmp3));
		if((++show) % 100000 == 0) {
			fprintf(stdout, "%d\n", show);
		}
	}

	delete [] tmp1;
	delete [] tmp2;
	delete [] tmp3;
	fclose(fp);
}

void go() {
	FILE *fp = fopen("input.txt", "r");

	int num;
	char *tmp = new char[MAXN];
	while(~fscanf(fp, "%d", &num)) {
		vector<set<pair<string, string> > > t;
		for(int i = 1; i <= num; i++) {
			fscanf(fp, "%s", tmp);

			set<pair<string, string> > tt;
			for(int j = 0; j < AllFacts.size(); j++)
				if(AllFacts[j].s == (string)tmp)
					tt.insert(make_pair(AllFacts[j].p, AllFacts[j].o));
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
	FILE *fp = fopen("SameFacts.txt", "w");

	int n = myType.size(), m = myType[0].size();

	for(int i = 0; i < m; i++) {

		set<pair<string, string> > sameType;
		sameType = myType[0][i];


		for(int k = 1; k < n; k++) {
			set<pair<string, string> > in;
			set_intersection(sameType.begin(), sameType.end(), myType[k][i].begin(), myType[k][i].end(), inserter(in, in.begin()));
			sameType = in;
		}

		fprintf(fp, "%lu\n", sameType.size());
		for(set<pair<string, string> >::iterator it = sameType.begin(); it != sameType.end(); it++)
			fprintf(fp, "%s\t%s\n", (it -> first).c_str(), (it -> second).c_str());
		
		fprintf(fp, "\n");
	}

	fclose(fp);
}

int main() {
	init();
	fprintf(stdout, "init() finished!\n");
	go();
	fprintf(stdout, "buildGraph() finished!\n");
	findSame();
	return 0;
}
