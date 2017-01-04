#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <map>

using namespace std;

const string tmpDir = "/Users/wzc/Desktop/lab/test/";

const string DataFile = "/Users/wzc/Downloads/yagoFacts.tsv";
const string EntityFile = tmpDir + "entityFile.txt";
const string PFile = tmpDir + "pFile.txt";
const string EdgeFile = tmpDir + "edge.dat";
const string GraphFile = tmpDir + "graph.dat";

const int MAXN = 10000;
const int N = 3000000, M = 12000000;
struct Fact {
	int s, o, pNum;
	Fact(){}
	Fact(int s_, int o_, int pNum_):s(s_), o(o_), pNum(pNum_){}
};
Fact facts[M >> 1];

int m, n;
map<string, int> entityID;
map<string, int> pNumber;

int pCnt;
string pName[MAXN];
int pVal[MAXN];

int head[N], to[M], w[M], nxt[M], graphmr;

void init() {
	FILE *fp = fopen(DataFile.c_str(), "r");
	FILE *entityFile = fopen(EntityFile.c_str(), "w");
	FILE *pFile = fopen(PFile.c_str(), "w");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN], *tmp3 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%*s%s%s%s", tmp1, tmp2, tmp3)) {
		if(pNumber.find(tmp2) == pNumber.end()) {
			fprintf(pFile, "%s %d\n", tmp2, pCnt);
			pNumber.insert(make_pair(tmp2, pCnt++));
		}

		pVal[pNumber[tmp2]]++;
		if(entityID.find(tmp1) == entityID.end()) {
			fprintf(entityFile, "%s %d\n", tmp1, n);
			entityID.insert(make_pair(tmp1, n++));
		}

		if(entityID.find(tmp3) == entityID.end()) {
			fprintf(entityFile, "%s %d\n", tmp3, n);
			entityID.insert(make_pair(tmp3, n++));
		}

		facts[m++] = Fact(entityID[tmp1], entityID[tmp3], pNumber[tmp2]);
	}

	delete [] tmp1;
	delete [] tmp2;
	delete [] tmp3;

	fclose(fp);
	fclose(entityFile);
	fclose(pFile);
}

inline void add(int x, int y, int z) {
	int p = graphmr++;
	to[p] = y; w[p] = z; nxt[p] = head[x]; head[x] = p;
}

void buildGraph() {
	memset(head, -1, sizeof head);
	for(int i = 0; i < m; i++) {

		int x = facts[i].s, y = facts[i].o, z = pVal[facts[i].pNum];

		add(x, y, z);
		add(y, x, z);
	}
}

void output() {
	FILE *fp = fopen(EdgeFile.c_str(), "wb");
	fwrite(facts, sizeof(Fact), m, fp);
	fclose(fp);


	fp = fopen(GraphFile.c_str(), "wb");
	fwrite(&n, sizeof(int), 1, fp);
	fwrite(&m, sizeof(int), 1, fp);
	fwrite(&graphmr, sizeof(int), 1, fp);
	fwrite(head, sizeof(int), n, fp);
	fwrite(to, sizeof(int), graphmr, fp);
	fwrite(w, sizeof(int), graphmr, fp);
	fwrite(nxt, sizeof(int), graphmr, fp);
	fclose(fp);

}

int main() {
	init();
	fprintf(stdout, "init() finished!\n");
	buildGraph();
	fprintf(stdout, "buildGraph() finished!\n");
	output();
	return 0;
}