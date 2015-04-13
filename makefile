try: skiplist.cpp skiplist.h
	g++ skiplist.cpp -O2 -o demo
g: skiplist.cpp skiplist.h
	g++ skiplist.cpp -g -o demo
run:
	time ./demo /tmp2/KDDCup2012/track2/kddcup2012track2.txt