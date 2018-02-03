all:
	g++ -Wall -std=c++0x -g merge_list.cpp -o merge_list

clean:
	rm -f *.o
	rm -f *.out
	rm merge_list
