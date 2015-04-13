try: skiplist.cpp skiplist.h
	g++ skiplist.cpp -O2 -o dsa_hw2_test.exe
pg: skiplist.cpp skiplist.h
	g++ skiplist.cpp -O3 -o dsa_hw2_test.exe -pg
run:
	time ./dsa_hw2_test.exe /tmp2/KDDCup2012/track2/kddcup2012track2.txt