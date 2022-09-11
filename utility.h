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

void transform(std::pair<int, std::set<int>>* R, int k, int m, int max) {

#pragma omp for
	for (size_t i = 1; i <= k; i++) {

		std::set<int> temp = {};
		
		for (auto e: R[i].second)
			temp.insert(m * max + e);

		R[i].second = temp; 
	}
}


void merge (std::pair<int, std::set<int>>* R1, std::pair<int, std::set<int>>* R2) {

	 
	std::set<int> s = {};
	std::vector<std::pair<int, std::set<int>>> res;

	size_t k1 = R1[0].first;
	size_t k2 = R2[0].first;

	for (size_t i = 1; i <= k1; i++) {

		for (size_t j = 1; j <= k2; j++) {

			if (R1[i].first == R2[j].first) {
				// std::cout << R1[i].first << '\n';
				std::set<int> temp = {};
				for (auto e: R1[i].second) {
					
					temp.insert(e);
					// std::cout << e << '\t';
				}
				// std::cout << '\n';
				for (auto e: R2[j].second) {
					
					temp.insert(e);
					// std::cout << e << '\t';
				}
				// std::cout << '\n';
				s.insert(R1[i].first);
				res.push_back(std::make_pair(R1[i].first, temp));
				// std::cout << "Boo\n";
				break;

			}
		} 
	}

	for (size_t j = 1; j <= k1; j++) {

		if (s.find(R1[j].first) == s.end()) {
			std::set<int> temp = {};
			for (auto e: R1[j].second) {
					temp.insert(e);
				}
			res.push_back(std::make_pair(R1[j].first, temp));
		}
	}


	for (size_t j = 1; j <= k2; j++) {

		if (s.find(R2[j].first) == s.end()) {
			std::set<int> temp = {};
			for (auto e: R2[j].second) {
					temp.insert(e);
				}
			res.push_back(std::make_pair(R2[j].first, temp));
		}

	}


	std::pair<int, std::set<int>> temp;
	temp.first = res.size();
	temp.second = {};
	std::vector<std::pair<int, std::set<int>>>::iterator it;
	it = res.begin();
  	it = res.insert ( it , temp );


  	for (size_t i = 0; i < res.size(); i++) {

  		R1[i].first = res[i].first;
  		R1[i].second.clear();
  		for (auto e: res[i].second) {
  			R1[i].second.insert(e);
  		} 
  	}

}


void reduce (std::pair<int, std::set<int>>** R, int m) {

	for (size_t i = 0; i < std::log2(m); ++i) {
#pragma omp for
		for (size_t j = 0; j < m; j = j + (2 * (i + 1))) {
			
			merge(R[j], R[j + (int)std::pow(2,i)]);	
		}		
	}	
}

std::vector<int> max_cover(std::vector<std::pair<int, std::set<int>>> Final, int k, int theta) {

	Bitmask<int> covered(theta);
	std::vector<int> result;


	for (size_t i = 0; i < k; i++) {

		int max = 0;
		int max_pos = -1;
#pragma omp for				
		for (int j = 0; j < Final.size(); j++) {

			if (Final[j].second.size() >= max) {
				max_pos = j;
				max = Final[j].second.size();
			}

		}
		result.push_back(Final[max_pos].first);

		for (auto e: Final[max_pos].second) {
			covered.set(e);
		}

#pragma omp for
		for (int j = 0; j < Final.size(); j++) {

			if(j != max_pos) {
				for (auto e = Final[j].second.begin(); e != Final[j].second.end(); ) {
					if (covered.get(*e))
						e = Final[j].second.erase(e);
					else
						e++;
				}
			}
		}
		Final.erase(Final.begin() + max_pos);
	}
	return result;
}


void pack(std::pair<int, std::set<int>>* R, int *packed, int total_size) {

	int size = R[0].first; 

	packed[0] = size;
	packed[1] = total_size;

	size_t idx = 2;
#pragma omp for
	for (size_t i = 1; i <= size; i++) {
		packed[idx] = R[i].first;
		idx += 1;
		packed[idx] = R[i].second.size();
		idx += 1;
		for (auto e : R[i].second) {
			packed[idx] = e;
			idx += 1;
		}
	}
}

std::vector<std::pair<int, std::set<int>>> unpack(int *packed) {

	std::vector<std::pair<int, std::set<int>>> R;

	std::set<int> temp = {};
	R.push_back(std::make_pair(packed[0], temp));

	for (size_t i = 2; i < packed[1]; ) {
		temp = {};
		int key = packed[i];

		for (size_t j = i + 2; j < packed[i + 1] + i + 2; j++) {
			temp.insert(packed[j]);
		}

		R.push_back(std::make_pair(key, temp));

		i = packed[i + 1] + i + 2;
	}

	return R;

}

