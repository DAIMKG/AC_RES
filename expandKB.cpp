#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

using namespace std;

const string OldDataDir = "/Users/wzc/Downloads/yagoFacts.tsv";
const string NewDataDir = "/Users/wzc/Downloads/newFacts.tsv";
const int MAXN = 10000;

int main() {
	FILE *fp = fopen(OldDataDir.c_str(), "r");
	FILE *fout = fopen(NewDataDir.c_str(), "w");

	char *tmp1 = new char[MAXN], *tmp2 = new char[MAXN], *tmp3 = new char[MAXN];
	int show = 0;

	fgets(tmp1, MAXN, fp);

	while(~fscanf(fp, "%*s%s%s%s", tmp1, tmp2, tmp3)) {

		if((++show) % 100000 == 0) {
			fprintf(stdout, "%d\n", show);
		}
		fprintf(fout, "%s\t%s\t%s\n", tmp1, tmp2, tmp3);
		fprintf(fout, "%s\t<__%s\t%s\n", tmp3, tmp2 + 1, tmp1);
	}
	delete [] tmp1;
	delete [] tmp2;
	delete [] tmp3;
	fclose(fp);
	fclose(fout);
}
