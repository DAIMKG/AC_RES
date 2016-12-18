#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <set>
#include <string>
#include <queue>

using namespace std;
const string DataDir = "/Users/wzc/Downloads/yagoFacts.tsv";
const string InputDir = "/Users/wzc/Desktop/lab/test/inputEntity.txt";
const string OutputDir = "/Users/wzc/Desktop/lab/test/candidateEntity.txt";

const int N = 7800000;
const int MAXN = 10000;
const int MAXO = 10;

set<string> vis; 

string s[N];
int cnt;

struct A {
	double w;
	int id;
	inline bool operator < (const A &x) const {
		return w > x.w;
	}
}a[N];


void init() {
	FILE *fp = fopen(DataDir.c_str(), "r");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%*s%s%*s%s", tmp1, tmp2)) {
		if(vis.find(tmp1) == vis.end()) {
			s[cnt++] = tmp1;
			vis.insert(tmp1);
		}
		if(vis.find(tmp2) == vis.end()) {
			s[cnt++] = tmp2;
			vis.insert(tmp2);
		}
	}

	delete [] tmp1;
	delete [] tmp2;
	fclose(fp);
}

inline double jarowinkler_distance(const char *s1, const char *s2) {

	int l1 = strlen(s1), l2 = strlen(s2);
	int r = max(l1, l2) / 2 - 1, p = 0, m = 0, t = 0;
	bool match1[MAXN] = {0}, match2[MAXN] = {0};
	for(int i = 0; i < min(l1, l2) && p < 4; i++)
		if(s1[i] == s2[i]) p++;
		else break;
	m = p;
	for(int i = p; i < l1; i++)
		for(int j = max(p, i - r); j <= min(l2 - 1, i + r); j++)
			if(!match2[j] && s1[i] == s2[j]) {
				match1[i] = match2[j] = 1;
				m++;
				break;
			}

	if(m == 0) return 0;

	for(int i = p, j = p; i < l1; i++)
		if(match1[i]) {
			while(j < l2 && !match2[j])
				j++;
			if(j < l2 && s1[i] != s2[j])
				t++;
			j++;
		}

	t >>= 1;
	double ret = (1.0 * m / l1 + 1.0 * m / l2 + 1.0 * (m - t) / m) / 3;
	return ret + p * 0.1 * (1 - ret);
}

void go() {
	FILE *fp = fopen(InputDir.c_str(), "r");
	FILE *out = fopen(OutputDir.c_str(), "w");
	char *tmp = new char[MAXN];
	while(~fscanf(fp, "%s", tmp)) {
		fprintf(out, "%s:\n", tmp);
		priority_queue<A> candidate;

		for(int i = 0; i < cnt; i++) {
			a[i].w = jarowinkler_distance(tmp, s[i].substr(1, s[i].length() - 2).c_str());
			a[i].id = i;
			candidate.push(a[i]);
			if(candidate.size() > MAXO) candidate.pop();
		}

		int *printList = new int [MAXO];
		int sz = candidate.size();
		for(int i = 0; i < sz; i++) {
			printList[i] = candidate.top().id;
			candidate.pop();
		}

		for(int i = sz - 1; i >= 0; i--)
			fprintf(out, "%s\n", s[printList[i]].c_str());

		fprintf(out, "\n");
	}

	delete [] tmp;
}

int main() {
	init();
	fprintf(stdout, "init() finished!\n");
	go();
	return 0;
}