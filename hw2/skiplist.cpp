#include <iostream>
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include <time.h>
#include "skiplist.h"

#define SKIP_LEVEL 20
//#define DATA_MAX 10000000

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

Ad_stream::Ad_stream() {
	next = NULL;
	ad = NULL;
}


bool User::operator > (User *usr) {
	if (usr == NULL)
		return false;
	if (ID == usr -> ID) {
		if (data -> ad -> ID == usr -> data -> ad -> ID)
			return data -> query > usr -> data -> query;
		return data -> ad -> ID > usr -> data -> ad -> ID;
	} else
		return ID > usr -> ID;
}

bool Ad::operator > (Ad *ad) {
	if (ad == NULL)
		return false;
	if (ID == ad -> ID)
		return data -> user -> ID > ad -> data -> user -> ID;
	else
		return ID > ad -> ID;
}

bool check_property (Ad *ad_1, Ad *ad_2) {
	if (ad_1->URL == ad_2->URL && ad_1->Advertiser == ad_2->Advertiser && ad_1->keyword == ad_2->keyword
		 && ad_1->title == ad_2->title && ad_1->description == ad_2->description)
		return true;
	return false;
}

void print_ad_property(Ad *ad) {
	cout << "\t" << ad -> URL << " " << ad -> Advertiser << " " << ad -> keyword << " " << ad -> title << " " << ad -> description << endl;
	return;
}




void get(int usr, int ad, int q, int p, int d, SkipNode_User *usr_cursor) {
	int c = 0;
	int imp = 0;
	User *ptr_user;

	// search for the entrance into the linklist
	for (int now_level = SKIP_LEVEL; now_level > 0; now_level--) {
		while (usr_cursor -> next != NULL && usr > usr_cursor -> next -> user -> ID)
			usr_cursor = usr_cursor -> next;
		if (usr_cursor -> decline != NULL)
			usr_cursor = usr_cursor -> decline;
	}
	if (usr_cursor -> user == NULL) {
		ptr_user = usr_cursor -> decline -> user;
	} else {
		ptr_user = usr_cursor -> user;
	}
	// search for the value "userID == usr"
	while (usr > ptr_user -> ID) {
		ptr_user = ptr_user -> next;
	}
	// search for the value "adID == ad"
	while (ad > ptr_user -> data -> ad -> ID)
		ptr_user = ptr_user -> next;
	while (ad == ptr_user -> data -> ad -> ID && usr == ptr_user -> ID) {
		if (q == ptr_user -> data -> query && p == ptr_user -> data -> position && d == ptr_user -> data -> depth) {
			c += ptr_user -> data -> click;
			imp += ptr_user -> data -> impression;
		}
		ptr_user = ptr_user -> next;
	}

	cout << "********************" << endl;
	cout << c << " " << imp << endl;
	cout << "********************" << endl;
	return;
}




void clicked(int usr, SkipNode_User *usr_cursor) {
	User *ptr_user;
	int prev_ad = 0, prev_q = 0;

	// search for the entrance into the linklist
	for (int now_level = SKIP_LEVEL; now_level > 0; now_level--) {
		while (usr_cursor -> next != NULL && usr > usr_cursor -> next -> user -> ID)
			usr_cursor = usr_cursor -> next;
		if (usr_cursor -> decline != NULL)
			usr_cursor = usr_cursor -> decline;
	}
	if (usr_cursor -> user == NULL) {
		ptr_user = usr_cursor -> decline -> user;
	} else {
		ptr_user = usr_cursor -> user;
	}
	// search for the value "userID == usr"
	while (usr > ptr_user -> ID) {
		ptr_user = ptr_user -> next;
	}
	cout << "********************" << endl;
	while (usr == ptr_user -> ID) {
		if (ptr_user -> data -> click != 0) {
			if (prev_ad != ptr_user -> data -> ad -> ID || prev_q != ptr_user -> data -> query) {
				prev_ad = ptr_user -> data -> ad -> ID;
				prev_q = ptr_user -> data -> query;
				cout << prev_ad << " " << prev_q << endl;
			}
		}
		ptr_user = ptr_user -> next;
	}
	cout << "********************" << endl;
	return;
}




void impressed(int usr_1, int usr_2, SkipNode_User *usr_skip_head) {
	SkipNode_User *usr1_cursor;
	SkipNode_User *usr2_cursor;
	User *ptr_user1, *ptr_user2;

	usr1_cursor = usr_skip_head;
	usr2_cursor = usr_skip_head;

	// search for the entrance of usr_1 into the linklist
	for (int now_level = SKIP_LEVEL; now_level > 0; now_level--) {
		while (usr1_cursor -> next != NULL && usr_1 > usr1_cursor -> next -> user -> ID)
			usr1_cursor = usr1_cursor -> next;
		if (usr1_cursor -> decline != NULL)
			usr1_cursor = usr1_cursor -> decline;
	}
	if (usr1_cursor -> user == NULL) {
		ptr_user1 = usr1_cursor -> decline -> user;
	} else {
		ptr_user1 = usr1_cursor -> user;
	}
	// search for the value "userID == usr_1"
	while (usr_1 > ptr_user1 -> ID) {
		ptr_user1 = ptr_user1 -> next;
	}

	// search for the entrance of usr_2 into the linklist
	for (int now_level = SKIP_LEVEL; now_level > 0; now_level--) {
		while (usr2_cursor -> next != NULL && usr_2 > usr2_cursor -> next -> user -> ID)
			usr2_cursor = usr2_cursor -> next;
		if (usr2_cursor -> decline != NULL)
			usr2_cursor = usr2_cursor -> decline;
	}
	if (usr2_cursor -> user == NULL) {
		ptr_user2 = usr2_cursor -> decline -> user;
	} else {
		ptr_user2 = usr2_cursor -> user;
	}
	// search for the value "userID == usr_2"
	while (usr_2 > ptr_user2 -> ID) {
		ptr_user2 = ptr_user2 -> next;
	}

	cout << "********************" << endl;

	// read through user_1 & user_2 and generate an ad_stream of an ad and print it
	while (ptr_user1 -> ID == usr_1 && ptr_user2 -> ID == usr_2) {
		int same_ad;
		Ad_stream ad_stream_head;
		Ad_stream *ad_stream_ptr;
		// find a same adID
		while (ptr_user1 -> data -> ad -> ID != ptr_user2 -> data -> ad -> ID) {
			if (ptr_user1 -> data -> ad -> ID > ptr_user2 -> data -> ad -> ID)
				ptr_user2 = ptr_user2 -> next;
			else
				ptr_user1 = ptr_user1 -> next;
			if (ptr_user1 -> ID != usr_1 || ptr_user2 -> ID != usr_2)
				break;
		}
		if (ptr_user1 -> ID != usr_1 || ptr_user2 -> ID != usr_2)
			break;
		same_ad = ptr_user1 -> data -> ad -> ID;
		// add all the ad of ID = same_ad into ad_stream
		while (ptr_user1 -> data -> ad -> ID == same_ad && ptr_user1 -> ID == usr_1) {
			int flag = 0;
			ad_stream_ptr = &ad_stream_head;
			while (ad_stream_ptr -> ad != NULL) {
				if (check_property(ptr_user1 -> data -> ad, ad_stream_ptr -> ad)) {
					ptr_user1 = ptr_user1 -> next;
					flag = 1;
					break;
				}
				ad_stream_ptr = ad_stream_ptr -> next;
			}
			if (flag == 1)
				continue;
			ad_stream_ptr -> ad = ptr_user1 -> data -> ad;
			ad_stream_ptr -> next = new Ad_stream;
			if (ptr_user1 -> next == NULL)
				break;
			ptr_user1 = ptr_user1 -> next;
		}
		while (ptr_user2 -> data -> ad -> ID == same_ad && ptr_user2 -> ID == usr_2) {
			int flag = 0;
			ad_stream_ptr = &ad_stream_head;
			while (ad_stream_ptr -> ad != NULL) {
				if (check_property(ptr_user2 -> data -> ad, ad_stream_ptr -> ad)) {
					ptr_user2 = ptr_user2 -> next;
					flag = 1;
					break;
				}
				ad_stream_ptr = ad_stream_ptr -> next;
			}
			if (flag == 1)
				continue;
			ad_stream_ptr -> ad = ptr_user2 -> data -> ad;
			ad_stream_ptr -> next = new Ad_stream;
			if (ptr_user2 -> next == NULL)
				break;
			ptr_user2 = ptr_user2 -> next;
		}
		ad_stream_ptr = &ad_stream_head;
		// print adID
		cout << same_ad << endl;
		// print property
		while (ad_stream_ptr -> ad != NULL) {
			print_ad_property(ad_stream_ptr -> ad);
			ad_stream_ptr = ad_stream_ptr -> next;
		}
	}
	cout << "********************" << endl;
	return;
}




void profit(int adID, double theta, SkipNode_Ad *ad_cursor) {
	Ad *ptr_ad;
	int flag = 0;

	// search for the entrance into the linklist
	for (int now_level = SKIP_LEVEL; now_level > 0; now_level--) {
		while (ad_cursor -> next != NULL && adID > ad_cursor -> next -> ad -> ID)
			ad_cursor = ad_cursor -> next;
		if (ad_cursor -> decline != NULL)
			ad_cursor = ad_cursor -> decline;
	}
	if (ad_cursor -> ad == NULL) {
		ptr_ad = ad_cursor -> decline -> ad;
	} else {
		ptr_ad = ad_cursor -> ad;
	}
	// search for the value "ptr_ad -> ID == adID"
	while (adID > ptr_ad -> ID) {
		ptr_ad = ptr_ad -> next;
	}
	cout << "********************" << endl;
	while(ptr_ad -> ID == adID) {
		int usr;
		double c = 0, imp = 0;
		usr = ptr_ad -> data -> user -> ID;

		while (ptr_ad -> data -> user -> ID == usr && ptr_ad -> ID == adID) {
			c += ptr_ad -> data -> click;
			imp += ptr_ad -> data -> impression;
			if (ptr_ad -> next == NULL) {
				flag = 1;
				break;
			}
			ptr_ad = ptr_ad -> next;
		}
		if (theta <= c/imp)
			cout << usr << endl;
		if (flag == 1)
			break;
	}
	cout << "********************" << endl;
	return;
}








int main(int argc, char *argv[]) {
	int /*c,*/ imp, /*url,*/ a, /*adver, d, p,*/ q, k, t, des, u;
	short int d, p;
	unsigned short int c, adver;
	unsigned long long url;

	char cmd[10];
	FILE *fp;
	int skip_count[SKIP_LEVEL];
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


	srand(time(NULL));

	fp = fopen(argv[1], "r");
	assert(fp != NULL);


	// build the head of the skip lists (user & ad)
	user_cursor = &user_skip_head;
	ad_cursor = &ad_skip_head;
	for (int i = 0; i <= SKIP_LEVEL; i++) {
		// building the nodes
		user_cursor -> decline = new SkipNode_User;
		ad_cursor -> decline = new SkipNode_Ad;

		user_cursor = user_cursor -> decline;
		ad_cursor = ad_cursor -> decline;
	}


	//long long data_control = 0;                               // for work station testing
	// read in and insert a new data to the data structer
	while (fscanf(fp, "%hu%d%llu%d%hu%hd%hd%d%d%d%d%d", &c, &imp, &url, &a, &adver, &d, &p, &q, &k, &t, &des, &u) != EOF) {
		//if (data_control > DATA_MAX)  // for work station testing
		//	break;
		//data_control++;

		int now_level;    // current level we are on
		int build_usr_level = 0;
		int build_ad_level = 0;
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


		// decide the build levels
		// we want to build skip nodes along the way we search through the skip list
		for (int i = 0; rand() % 100 < 37 && i < SKIP_LEVEL; i++) {
				build_usr_level++;
				build_ad_level++;
		}


		// insert "new_data -> user" into linklist ====================
		user_cursor = &user_skip_head;
		// find the entrance into the linklist
		for (now_level = SKIP_LEVEL; now_level > 0; now_level--) {
			// move to next
			while (user_cursor -> next != NULL && *(new_data -> user) > user_cursor -> next -> user)
				user_cursor = user_cursor -> next;
			// build skip node
			if (now_level <= build_usr_level) {
				//cout << "levels: " << now_level << " " << build_usr_level << endl;           // debug
				if (build_skip_user_ptr == NULL) {
					build_skip_user_ptr = new SkipNode_User;
					build_skip_user_ptr -> next = user_cursor -> next;
					user_cursor -> next = build_skip_user_ptr;
					build_skip_user_ptr -> user = new_data -> user;
					//cout << "flag_build_usr_node_success" << endl;      // debug
				} else {
					build_skip_user_ptr -> decline = new SkipNode_User;
					build_skip_user_ptr = build_skip_user_ptr -> decline;
					build_skip_user_ptr -> next = user_cursor -> next;
					user_cursor -> next = build_skip_user_ptr;
					build_skip_user_ptr -> user = new_data -> user;
				}
			}
			// decline
			if (user_cursor -> decline != NULL)
				user_cursor = user_cursor -> decline;
		}

		// connect builded skip nodes to user
		if (build_skip_user_ptr != NULL) {
			build_skip_user_ptr -> user = new_data -> user;
			build_skip_user_ptr = NULL;
		}

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
				if (*ptr_user > new_data -> user) {
					// insert
					new_data -> user -> next = ptr_user;
					user_cursor -> decline -> user = new_data -> user;
				} else {
					while (*(new_data -> user) > ptr_user -> next)
						ptr_user = ptr_user -> next;
					// insert
					new_data -> user -> next = ptr_user -> next;
					ptr_user -> next = new_data -> user;
				}
			}
		} else {
			ptr_user = user_cursor -> user;
			while (*(new_data -> user) > ptr_user -> next)
				ptr_user = ptr_user -> next;
			// insert
			new_data -> user -> next = ptr_user -> next;
			ptr_user -> next = new_data -> user;
		}


		// insert "new_data -> ad" into linklist ====================
		ad_cursor = &ad_skip_head;
		// find the entrance into the linklist
		for (now_level = SKIP_LEVEL; now_level > 0; now_level--) {
			//cout << "ad_now_level: " << now_level << endl;          // debug
			// move to next
			while (ad_cursor -> next != NULL && *(new_data -> ad) > ad_cursor -> next -> ad)
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
			if (ad_cursor -> decline != NULL)
				ad_cursor = ad_cursor -> decline;
		}
		// connect builded skip nodes to ad
		if (build_skip_ad_ptr != NULL) {
			build_skip_ad_ptr -> ad = new_data -> ad;
			build_skip_ad_ptr = NULL;
		}

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
				if (*ptr_ad > new_data -> ad) {
					// insert
					new_data -> ad -> next = ptr_ad;
					ad_cursor -> decline -> ad = new_data -> ad;
				} else {
					while (*(new_data -> ad) > ptr_ad -> next)
						ptr_ad = ptr_ad -> next;
					// insert
					new_data -> ad -> next = ptr_ad -> next;
					ptr_ad -> next = new_data -> ad;
				}
			}
		} else {
			ptr_ad = ad_cursor -> ad;
			while (*(new_data -> ad) > ptr_ad -> next)
				ptr_ad = ptr_ad -> next;
			// insert
			new_data -> ad -> next = ptr_ad -> next;
			ptr_ad -> next = new_data -> ad;
		}
	}
	//cout << "read in success" << endl;      // debug
	fclose(fp);


	//return 0;    // for work station testing



	char click[10] = "clicked";
	char gt[10] = "get";
	char prft[10] = "profit";
	char impre[10] = "impressed";
	char qt[10] = "quit";

	while (scanf("%s", cmd) != EOF) {
		int u, a, q, p, d;
		int u1, u2;
		double theta;
		if (strcmp(cmd, click) == 0) {
			scanf("%d", &u);
			clicked(u, &user_skip_head);
		} else if (strcmp(cmd, gt) == 0) {
			scanf("%d%d%d%d%d", &u, &a, &q, &p, &d);
			get(u, a, q, p, d, &user_skip_head);
		} else if (strcmp(cmd, prft) == 0) {
			scanf("%a%lf", &a, &theta);
			profit(a, theta, &ad_skip_head);
		} else if (strcmp(cmd, impre) == 0) {
			scanf("%d%d", &u1, &u2);
			impressed(u1, u2, &user_skip_head);
		} else if (strcmp(cmd, qt) == 0)
			return 0;
	}

	return 0;
}