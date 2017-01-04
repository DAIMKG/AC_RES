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

const string tmpDir = "/Users/wzc/Desktop/lab/test/";

const string InputFile = tmpDir + "input.txt";
const string OutputFile = tmpDir + "pattern.txt";

const string EntityFile = tmpDir + "entityFile.txt";
const string PFile = tmpDir + "pFile.txt";
const string EdgeFile = tmpDir + "edge.dat";
const string GraphFile = tmpDir + "graph.dat";

const int MAXL = 0x3f3f3f3f;
const int MAXN = 10000;
const int N = 3000000, M = 12000000;

struct Fact {
	int s, o, pNum;
	Fact(){}
	Fact(int s_, int o_, int pNum_):s(s_), o(o_), pNum(pNum_){}
};

int m, n;

map<string, int> entityID;
map<string, int> pNumber;

string pName[MAXN];



int group;
set<int> node, edge;
int *pendingList;

Fact facts[M >> 1];

int head[N], to[M], w[M], nxt[M], graphmr;

int q[N], h, t, fromNode[N], fromEdge[N];
int dis[N], disW[N];

void init() {
	FILE *entity = fopen(EntityFile.c_str(), "r");
	FILE *p = fopen(PFile.c_str(), "r");
	FILE *edge = fopen(EdgeFile.c_str(), "rb");
	FILE *graph = fopen(GraphFile.c_str(), "rb");

	fread(&n, sizeof(int), 1, graph);
	fread(&m, sizeof(int), 1, graph);

	char *tmp = new char[MAXN];
	int id;
	for(int i = 1; i <= n; i++) {
		fscanf(entity, "%s%d", tmp, &id);
		entityID[tmp] = id;
	}

	while(~fscanf(p, "%s %d", tmp, &id)) {
		pName[id] = tmp;
	}

	delete [] tmp;

	fread(facts, sizeof(Fact), m, edge);

	fread(&graphmr, sizeof(int), 1, graph);
	fread(head, sizeof(int), n, graph);
	fread(to, sizeof(int), graphmr, graph);
	fread(w, sizeof(int), graphmr, graph);
	fread(nxt, sizeof(int), graphmr, graph);

	fclose(entity);
	fclose(p);
	fclose(edge);
	fclose(graph);
}


void bfs(int S, int T) {
	q[h = t = 1] = S;
	dis[S] = 0;
	disW[S] = 0;

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
	if(group == 1) fp = fopen(OutputFile.c_str(), "w");
	else fp = fopen(OutputFile.c_str(), "a");

	map<int, int> tmp;

	fprintf(fp, "%lu\n", node.size());
	int cnt = 0;
	for(set<int>::iterator it = node.begin(); it != node.end(); it++) {
		int curNode = *it;
		int flag = -1;
		for(int i = 0; i < num; i++)
			if(curNode == pendingList[i]) {
				flag = i;
				break;
			}
		tmp.insert(make_pair(curNode, cnt));
		if(flag != -1) {
			fprintf(fp, "%d 0 %d\n", cnt++, flag);
		}
		else {
			fprintf(fp, "%d 1\n", cnt++);
		}
	}

	fprintf(fp, "\n");

	fprintf(fp, "%lu\n", edge.size());
	for(set<int>::iterator it = edge.begin(); it != edge.end(); it++) {
		int curEdge = *it;
		fprintf(fp, "%d %d %s\n", tmp[facts[curEdge].s], tmp[facts[curEdge].o], pName[facts[curEdge].pNum].c_str());
	}

	fprintf(fp, "\n");
	fprintf(fp, "\n");
	fclose(fp);
}

void go() {
	FILE *fp = fopen(InputFile.c_str(), "r");

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
	go();
	return 0;
}









