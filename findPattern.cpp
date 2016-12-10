#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include <stack>
#include <set>
using namespace std;

const int MAXL = 0x3f3f3f3f;
const int MAXN = 10000;
const int N = 7800000, M = 12000000;

struct Fact {
	string s, p, o;
	Fact(){}
	Fact(string s_, string p_, string o_):s(s_), p(p_), o(o_){}
};

int m, n;
map<string, int> pVal;
map<string, int> entityID;
//string IDentity[N];

int group;
set<int> node, edge;
int *pendingList;

Fact facts[M >> 1];

int head[N], to[M], w[M], nxt[M], graphmr;

int q[N], h, t, fromNode[N], fromEdge[N];
int dis[N], disW[N];

void init() {
	FILE *fp = fopen("/Users/wzc/Downloads/yagoFacts.tsv", "r");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN], *tmp3 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%*s%s%s%s", tmp1, tmp2, tmp3)) {
		facts[m++] = Fact(tmp1, tmp2, tmp3);
		if(pVal.find(tmp2) == pVal.end())
			pVal.insert(make_pair(tmp2, 0));
		else
			pVal[tmp2]++;

		if(entityID.find(tmp1) == entityID.end()) {
			//IDentity[n] = tmp1;
			entityID.insert(make_pair(tmp1, n++));
		}

		if(entityID.find(tmp3) == entityID.end()) {
			//IDentity[n] = tmp3;
			entityID.insert(make_pair(tmp3, n++));
		}
	}

	delete [] tmp1;
	delete [] tmp2;
	delete [] tmp3;

	fclose(fp);
}

inline void add(int x, int y, int z) {
	int p = graphmr++;
	to[p] = y; w[p] = z; nxt[p] = head[x]; head[x] = p;
}

void buildGraph() {
	memset(head, -1, sizeof head);
	for(int i = 0; i < m; i++) {

		int x = entityID[facts[i].s], y = entityID[facts[i].o], z = pVal[facts[i].p];

		add(x, y, z);
		add(y, x, z);
	}
}

void bfs(int S, int T) {
	q[h = t = 1] = S;
	dis[S] = 0;
	disW[S] = 0.0;

	while(h <= t) {
		int sta = q[h++];
		if(sta == T) break;
		for(int i = head[sta]; ~i; i = nxt[i])
			if(dis[to[i]] == MAXL) {
				dis[to[i]] = dis[sta] + 1;
				disW[to[i]] = disW[sta] + w[i];
				fromNode[to[i]] = sta;
				fromEdge[to[i]] = i >> 1;
				q[++t] = to[i];
			}
			else if(dis[to[i]] == dis[sta] + 1) {
				if(disW[to[i]] < disW[sta] + w[i]) {
					disW[to[i]] = disW[sta] + w[i];
					fromNode[to[i]] = sta;
					fromEdge[to[i]] = i >> 1;
					q[++t] = to[i];
				}
			}
	}
}

inline void collect(int S, int T) {
	for(int i = T; i != S; i = fromNode[i]) {
		node.insert(i);
	}
	node.insert(S);

	for(int i = T; i != S; i = fromNode[i]) {
		edge.insert(fromEdge[i]);
	}
}

inline void clear() {
	for(int i = 1; i <= t; i++)
		dis[q[i]] = MAXL;
}

void writeFile(int num) {
	FILE *fp;
	if(group == 1) fp = fopen("pattern.txt", "w");
	else fp = fopen("pattern.txt", "a");

	map<int, int> tmp;

	fprintf(fp, "%lu\n", node.size());
	int cnt = 0;
	for(set<int>::iterator it = node.begin(); it != node.end(); it++) {
		int curNode = *it;
		int flag = 1;
		for(int i = 0; i < num; i++)
			if(curNode == pendingList[i]) {
				flag = 0;
				break;
			}
		tmp.insert(make_pair(curNode, cnt));
		fprintf(fp, "%d %d\n", cnt++, flag);
	}

	fprintf(fp, "\n");

	fprintf(fp, "%lu\n", edge.size());
	for(set<int>::iterator it = edge.begin(); it != edge.end(); it++) {
		int curEdge = *it;
		fprintf(fp, "%d %d %s\n", tmp[entityID[facts[curEdge].s]], tmp[entityID[facts[curEdge].o]], facts[curEdge].p.c_str());
	}

	fprintf(fp, "\n");
	fprintf(fp, "\n");
	fclose(fp);
}

void go() {
	FILE *fp = fopen("input.txt", "r");

	int num;
	char *tmp = new char[MAXN];
	while(~fscanf(fp, "%d", &num)) {
		group++;

		pendingList = new int[num + 10];
		for(int i = 0; i < num; i++) {
			fscanf(fp, "%s", tmp);
			if(entityID.find(tmp) == entityID.end()) {
				fprintf(stderr, "Entity not found!\n");
				exit(1);
			}
			pendingList[i] = entityID[tmp];
		}

		memset(dis, 0x3f, sizeof dis);

		node.clear();
		edge.clear();

		for(int i = 0; i < num; i++)
			for(int j = i + 1; j < num; j++) {
				bfs(pendingList[i], pendingList[j]);
				collect(pendingList[i], pendingList[j]);
				clear();
			}

		writeFile(num);
		delete [] pendingList;
	}

	delete [] tmp;
	fclose(fp);
}



int main() {
	init();
	fprintf(stdout, "init() finished!\n");
	buildGraph();
	fprintf(stdout, "buildGraph() finished!\n");
	go();
	//maximalSpanningTree();
	return 0;
}









