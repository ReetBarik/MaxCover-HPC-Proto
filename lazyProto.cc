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
#include <chrono>

std::vector<int> max_cover(std::unordered_map<int, std::set<int>> data, int k, int theta) {

	Bitmask<int> covered(theta);
	std::vector<int> result(k, -1);


	for (size_t i = 0; i < k; i++) {

		int max = 0;
		int max_key = -1;

		for( const auto& [key, value] : data ) {

			if (value.size() > max) {
				max = value.size();
				max_key = key;
			}

		}
		result[i] = max_key;

		for (int e: data[max_key]) {
			covered.set(e);
		}

		for( const auto& [key, value] : data ) {

			std::set<int> temp;
			if (key != max_key) {
				for (int e: value) {
					if (covered.get(e)) {
						temp.insert(e);
					}
				}
				for (int e: temp) {
    				data[key].erase(e); 
      			}

			}

		}

		data.erase(max_key);
	}
	return result;
}


template <typename idTy>
struct CompareMaxHeap {

  bool operator()(const std::pair<idTy, std::set<idTy>> a,
                  const std::pair<idTy, std::set<idTy>> b) {
    return a.second.size() < b.second.size();
  }
};

std::vector<int> max_cover_lazy_greedy(std::unordered_map<int, std::set<int>> data, int k, int theta) {


	CompareMaxHeap<int> cmp;

	std::vector<std::pair<int, std::set<int>>> data_vec	= std::vector<std::pair<int, std::set<int>>>(data.begin(), data.end());
	std::priority_queue<std::pair<int, std::set<int>>,
	                	std::vector<std::pair<int, std::set<int>>>,
	                	decltype(cmp)> pq(data_vec.begin(), data_vec.end());

	Bitmask<int> covered(theta);
	
	std::vector<int> result(k, -1);
	int count = 0;


	while(count < k) {

		
		auto l = pq.top();
    	pq.pop();
    	
    	
    	// remove RR IDs from l that are already covered. 
		std::set<int> temp;
    	for (int e: l.second) {
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
    	if (marginal_gain >= r.second.size()) {
	      	result[count] = l.first;
	      	count += 1;
	      	
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
	int count = 0; int theta = -1, k = 300;

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
	
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	auto S1 = max_cover_lazy_greedy(data, k, theta);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	std::cout << "Time taken for lazy greedy = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	for (auto e: S1)
		std::cout << e << '\t';

	std::cout << '\n';


	begin = std::chrono::steady_clock::now();
	auto S2 = max_cover(data, k, theta);
	end = std::chrono::steady_clock::now();

	std::cout << "Time taken for naive greedy = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

	for (auto e: S2)
		std::cout << e << '\t';

	return 0;
}