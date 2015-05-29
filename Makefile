all:
	g++ -std=c++11 -g -pthread non_concurrent_list_exp_failure.cpp
	g++ -std=c++11 -g -o atom -pthread concurrent_list_exp.cpp

