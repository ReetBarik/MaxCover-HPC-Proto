#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <set>
#include <omp.h>
#include <cmath>
#include "bitmask.h"
#include <fstream>
#include <sstream>
#include <string.h>
#include <unordered_map>
#include <queue>


template <typename idTy>
struct CompareMaxHeap {

  bool operator()(const std::pair<idTy, std::set<idTy>> a,
                  const std::pair<idTy, std::set<idTy>> b) {
    return a.second.size() < b.second.size();
  }
};

std::vector<int> max_cover_lazy_greedy(std::unordered_map<int, std::set<int>> data, int k, int theta) {


	CompareMaxHeap<int> cmp;
	std::priority_queue<std::pair<int, std::set<int>>,
	                	std::vector<std::pair<int, std::set<int>>>,
	                	decltype(cmp)> pq;

	Bitmask<int> covered(theta);
	// std::vector<int> coveredBool(theta, 0);
	std::vector<int> result(k, -1);
	int count = 0;
	for( const auto& [key, value] : data ) {
		pq.push(std::make_pair(key, value));
	}

	while(count < k) {


		auto l = pq.top();
    	pq.pop();
    	
    	// remove RR IDs from l that are already covered. 
		std::set<int> temp;
    	for (auto e: l.second) {
      		if (covered.get(e)) {
        		temp.insert(e);
        	}
      	}
      	
      	for (int e: temp) {
    		l.second.erase(e); 
      	}

    	//calculate marginal gain
    	auto marginal_gain = l.second.size();

    	//calculate utiluty of next best
    	auto r = pq.top();
    	
    	// if marginal of l is better than r's utility, l is the current best 
    	if (marginal_gain > r.second.size()) {
	      	result[count] = l.first;
	      	count += 1;
	      	
	      	std::set<int>::iterator itr;
	      	for (auto e : l.second) {
	        	covered.set(e);
	      	}
    	}
    	// else push l's marginal into the heap 
	    else {
	      pq.push(l);
	    }

	}

	return result;

}


int main(int argc, char *argv[]) {

	std::unordered_map<int, std::set<int>> data;
	std::ifstream file;
	file.open("coverage.txt");
	int count = 0; int theta = -1;

	std::string line;
	while(getline(file, line))
	{
		std::set<int> temp = {};
		std::string word;
		for ( std::istringstream is( line ); is >> word; )
	    {
	        temp.insert(stoi(word));
	        if (stoi(word) > theta)
	        	theta = stoi(word);
	    }
	    data[++count] = temp;
	}
	
	auto S = max_cover_lazy_greedy(data, 10, theta);

	for (auto e: S)
		std::cout << e << '\t';

	return 0;
}