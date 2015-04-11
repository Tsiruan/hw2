#include <iostream>
#include <cstdio>
#include <cassert>
#include "skiplist.h"

#define SKIP_LEVEL 50

using namespace std;

Data::Data() {
	user = new User;
	ad = new Ad;

	user -> data = this;
	ad -> data = this;
}

SkipNode_User::SkipNode_User() {
	next = NULL;
	decline = NULL;
	user = NULL;
}

SkipNode_Ad::SkipNode_Ad() {
	next = NULL;
	decline = NULL;
	ad = NULL;
}


bool User::operator > (User usr) {
	if (ID == usr.ID)
		return data -> ad -> ID > usr.data -> ad -> ID;
	else
		return ID > usr.ID;
}

bool Ad::operator > (Ad ad) {
	if (ID == ad.ID)
		return data -> user -> ID > ad.data -> user -> ID;
	else
		return ID > ad.ID;
}



/*
bool operator > (User *usr_1, User *usr_2) {
	if (usr_1 -> ID == usr_2 -> ID)
		return usr_1 -> data -> ad -> ID > usr_2 -> data -> ad -> ID;
	else
		return usr_1 -> ID > usr_2 -> ID;
}

bool operator > (Ad *ad_1, Ad *ad_2) {
	if (ad_1 -> ID == ad_2 -> ID)
		return ad_1 -> data -> user -> ID > ad_2 -> data -> user -> ID;
	else
		return ad_1 -> ID > ad_2 -> ID;
}
*/



int main(void) {
	int /*c,*/ imp, /*url,*/ a, /*adver, d, p,*/ q, k, t, des, u;
	short int d, p;
	unsigned short int c, adver;
	unsigned long long url;

	FILE *fp;
	int skip_count[SKIP_LEVEL - 1];
	// for building the user skip list
	SkipNode_User user_skip_head;        // the head, const
	SkipNode_User *user_cursor;          // for searching on the existing skip node
	SkipNode_User *build_skip_user_ptr;  // for building new node while searching on the skip list
	// for building the ad skip list
	SkipNode_Ad ad_skip_head;            // the head, const
	SkipNode_Ad *ad_cursor;              // for searching on the existing skip node
	SkipNode_Ad *build_skip_ad_ptr;      // for building new node while searching on the skip list
	// for traversing in the user & ad linklist
	User *ptr_user;  // for searching on the user linklist
	Ad *ptr_ad;      // for searching on the ad linklist



	cout << "flag_1" << endl;   // debug flag_1



	fp = fopen("demotrack.txt", "r");
	assert(fp != NULL);


	cout << "flag_2" << endl;   // debug flag_2



	// build the head of the skip lists (user & ad)
	user_cursor = &user_skip_head;
	//cout << "flag_2.0" << endl;                     // debug
	ad_cursor = &ad_skip_head;
	//cout << "flag_2.0" << endl;                     // debug
	for (int i = 0; i <= SKIP_LEVEL; i++) {

		// decide the amount of the nodes we will be building
		if (i != SKIP_LEVEL)
			skip_count[i] = (SKIP_LEVEL - i) * (SKIP_LEVEL - i);

		//cout << "flag_2.1" << endl;                 // debug

		// building the nodes
		user_cursor -> decline = new SkipNode_User;
		ad_cursor -> decline = new SkipNode_Ad;

		//cout << "flag_2.2" << endl;                 // debug

		user_cursor = user_cursor -> decline;
		ad_cursor = ad_cursor -> decline;
	}

	ad_skip_head.next = NULL;     // i don't know why


	cout << "flag_3" << endl;   // debug flag_3


	// read in and insert a new data to the data structer
	while (fscanf(fp, "%hu%d%llu%d%hu%hd%hd%d%d%d%d%d", &c, &imp, &url, &a, &adver, &d, &p, &q, &k, &t, &des, &u) != EOF) {
		int now_level;    // current level we are on
		int build_usr_level, build_ad_level;
		build_skip_user_ptr = NULL;
		build_skip_ad_ptr = NULL;

		
		Data *new_data;
		new_data = new Data;
		// read in data
		new_data -> click = c;
		new_data -> impression = imp;
		new_data -> query = q;
		new_data -> position = p;
		new_data -> depth = d;
		new_data -> user -> ID = u;
		new_data -> ad -> ID = a;
		new_data -> ad -> URL = url;
		new_data -> ad -> Advertiser = adver;
		new_data -> ad -> keyword = k;
		new_data -> ad -> title = t;
		new_data -> ad -> description = des;


		cout << "flag_4" << endl;         // debug


		// decide the build levels
		// we want to build skip nodes along the way we search through the skip list
		int i = new_data -> ad -> ID % SKIP_LEVEL;
		if (skip_count[i] > 0) {
			build_usr_level = build_ad_level = i + 1;
			skip_count[i]--;
		}


		cout << "flag_5" << endl;         // debug



		// insert "new_data -> user" into linklist ====================
		user_cursor = &user_skip_head;
		// find the entrance into the linklist
		for (now_level = SKIP_LEVEL; now_level > 0; now_level--) {
			// move to next
			while (user_cursor -> next != NULL && *(new_data -> user) > *(user_cursor -> next -> user))
				user_cursor = user_cursor -> next;
			// build skip node
			if (now_level <= build_usr_level) {
				//cout << "levels: " << now_level << " " << build_usr_level << endl;           // debug
				if (build_skip_user_ptr == NULL) {+
					build_skip_user_ptr = new SkipNode_User;
					build_skip_user_ptr -> next = user_cursor -> next;
					user_cursor -> next = build_skip_user_ptr;
					build_skip_user_ptr -> user = new_data -> user;
					cout << "flag_build_usr_node_success" << endl;      // debug
				} else {
					build_skip_user_ptr -> decline = new SkipNode_User;
					build_skip_user_ptr = build_skip_user_ptr -> decline;
					build_skip_user_ptr -> next = user_cursor -> next;
					user_cursor -> next = build_skip_user_ptr;
					build_skip_user_ptr -> user = new_data -> user;
				}
			}
			// decline
			user_cursor = user_cursor -> decline;
		}

		cout << "flag_7" << endl;            // debug

		// connect builded skip nodes to user
		build_skip_user_ptr -> user = new_data -> user;
		build_skip_user_ptr = NULL;

		// for the insertion at the front
		if (user_cursor -> user == NULL) {
			ptr_user = user_cursor -> decline -> user;
			// for the first insertion
			if (ptr_user == NULL) {
				// insert
				user_cursor -> decline -> user = new_data -> user;
				new_data -> user -> next = NULL;
			} else {
				// search from the head of the link list
				if (ptr_user > new_data -> user) {
					// insert
					new_data -> user -> next = ptr_user;
					user_cursor -> decline -> user = new_data -> user;
				} else {
					while (new_data -> user > ptr_user -> next)
						ptr_user = ptr_user -> next;
					// insert
					new_data -> user -> next = ptr_user -> next;
					ptr_user -> next = new_data -> user;
				}
			}
		} else {
			ptr_user = user_cursor -> user;
			while (new_data -> user > ptr_user -> next)
				ptr_user = ptr_user -> next;
			// insert
			new_data -> user -> next = ptr_user -> next;
			ptr_user -> next = new_data -> user;
		}


		cout << "flag_8" << endl;           // debug


		// insert "new_data -> ad" into linklist ====================
		ad_cursor = &ad_skip_head;
		// find the entrance into the linklist
		for (now_level = SKIP_LEVEL; now_level > 0; now_level--) {
			//cout << "ad_now_level: " << now_level << endl;          // debug
			// move to next
			while (ad_cursor -> next != NULL && *(new_data -> ad) > *(ad_cursor -> next -> ad))
				ad_cursor = ad_cursor -> next;
			//cout << "flag_9" << endl;               // debug
			// build skip node
			if (now_level <= build_ad_level) {
				//cout << "levels: " << now_level << " " << build_ad_level << endl;     // debug
				if (build_skip_ad_ptr == NULL) {
					build_skip_ad_ptr = new SkipNode_Ad;
					build_skip_ad_ptr -> next = ad_cursor -> next;
					ad_cursor -> next = build_skip_ad_ptr;
					build_skip_ad_ptr -> ad = new_data -> ad;
				} else {
					build_skip_ad_ptr -> decline = new SkipNode_Ad;
					build_skip_ad_ptr = build_skip_ad_ptr -> decline;
					build_skip_ad_ptr -> next = ad_cursor -> next;
					ad_cursor -> next = build_skip_ad_ptr;
					build_skip_ad_ptr -> ad = new_data -> ad;
				}
			}
			// decline
			ad_cursor = ad_cursor -> decline;
		}
		// connect builded skip nodes to ad
		build_skip_ad_ptr -> ad = new_data -> ad;
		build_skip_ad_ptr = NULL;

		// for the insertion at the front
		if (ad_cursor -> ad == NULL) {
			ptr_ad = ad_cursor -> decline -> ad;
			// for the first insertion
			if (ptr_ad == NULL) {
				// insert
				ad_cursor -> decline -> ad = new_data -> ad;
				new_data -> ad -> next = NULL;
			} else {
				// search from the head of the link list
				if (ptr_ad > new_data -> ad) {
					// insert
					new_data -> ad -> next = ptr_ad;
					ad_cursor -> decline -> ad = new_data -> ad;
				} else {
					while (new_data -> ad > ptr_ad -> next)
						ptr_ad = ptr_ad -> next;
					// insert
					new_data -> ad -> next = ptr_ad -> next;
					ptr_ad -> next = new_data -> ad;
				}
			}
		} else {
			ptr_ad = ad_cursor -> ad;
			while (new_data -> ad > ptr_ad -> next)
				ptr_ad = ptr_ad -> next;
			// insert
			new_data -> ad -> next = ptr_ad -> next;
			ptr_ad -> next = new_data -> ad;
		}

		cout << "flag_end" << endl;           // debug

	}


	cout << "read in success" << endl;

	fclose(fp);
	return 0;
}