#include <iostream>
#include <cstdio>
#include <cassert>

class Data;

class User {
public:
	int ID;
	
	User *next;
	Data *data;

	bool operator > (User*);
};

class Ad {
public:
	int ID;
	unsigned long long URL;
	unsigned short int Advertiser;
	int keyword;
	int title;
	int description;
	
	Ad *next;
	Data *data;

	bool operator > (Ad*);
};

class Data {
public:
	unsigned short int click;
	int impression;
	int query;
	short int position;
	short int depth;

	User *user;
	Ad *ad;

	Data();
};

class SkipNode_User {
public:
	SkipNode_User *next;
	SkipNode_User *decline;
	User *user;

	SkipNode_User();
};

class SkipNode_Ad {
public:
	SkipNode_Ad *next;
	SkipNode_Ad *decline;
	Ad *ad;

	SkipNode_Ad();
};