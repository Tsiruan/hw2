#include <iostream>
#include <cassert>
#include <cstdio>
#include "skiplist.h"

using namespace std;

int main(void) {
	int /*c,*/ imp, /*url,*/ a, /*adver, d, p,*/ q, k, t, des, u;
	short int d, p;
	unsigned short int c, adver;
	unsigned long long url;
	FILE *fp;


	fp = fopen("demotrack.txt", "r");
	assert(fp != NULL);

	cout << "flag_1" << endl;

	fscanf(fp, "%hu%d%llu%d%hu%hd%hd%d%d%d%d%d", &c, &imp, &url, &a, &adver, &d, &p, &q, &k, &t, &des, &u);

	cout << url << endl;

	while (fscanf(fp, "%hu%d%llu%d%hu%hd%hd%d%d%d%d%d", &c, &imp, &url, &a, &adver, &d, &p, &q, &k, &t, &des, &u) != EOF) {
		cout << "flag" << endl;
		cout << url << endl;
	}

	cout << "read in success" << endl;

	return 0;
}