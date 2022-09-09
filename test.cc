#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <set>
#include <omp.h>
#include <cmath>

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

	int uncommon = 0;

	for (size_t j = 1; j <= k1; j++) {

		if (s.find(R1[j].first) == s.end()) {
			std::set<int> temp = {};
			for (auto e: R1[j].second) {
					
					temp.insert(e);
					// std::cout << e << '\t';
				}
			res.push_back(std::make_pair(R1[j].first, temp));
			// std::cout << "Boo\n";
			uncommon += 1;
		}
	}


	for (size_t j = 1; j <= k2; j++) {

		if (s.find(R2[j].first) == s.end()) {
			std::set<int> temp = {};
			for (auto e: R2[j].second) {
					
					temp.insert(e);
					// std::cout << e << '\t';
				}
			res.push_back(std::make_pair(R2[j].first, temp));
			// std::cout << "Boo\n";
			uncommon += 1;
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

std::vector<int> max_cover(std::vector<std::pair<int, std::set<int>>> Final, int k) {

	std::vector<int> result;


	for (size_t i = 0; i < k; i++) {

		int max = 0;
		int max_pos = -1;
#pragma omp for
		for (int j = 0; j < Final.size(); j++) {
			// std::cout << Final[j].second.size() << std::endl;
			if (Final[j].second.size() >= max) {
				max_pos = j;
				max = Final[j].second.size();
			}

		}

		result.push_back(Final[max_pos].first);
		
#pragma omp for
		for (size_t j = 0; j < Final.size(); j++) {

			if(j != max_pos) {

				for (auto e: Final[max_pos].second) {
					Final[j].second.erase(e);
				}

			}
		}

		Final.erase(Final.begin() + max_pos);
	}

	return result;

}

int main(int argc, char *argv[]) {

	size_t k = 4;
	size_t m = 2;

	std::pair<int, std::set<int>>* r1 = new std::pair<int, std::set<int>>[m * k + 1];
	std::pair<int, std::set<int>>* r2 = new std::pair<int, std::set<int>>[m * k + 1];

	r1[0].first = k;
	r1[1].first = 1;
	r1[2].first = 10;
	r1[3].first = 5;
	r1[4].first = 7;
	r1[0].second = {};
	r1[1].second = {};
	r1[2].second = {};
	r1[3].second = {};
	r1[4].second = {};

	r2[0].first = k;
	r2[1].first = 1;
	r2[2].first = 10;
	r2[3].first = 7;
	r2[4].first = 3;
	r2[0].second = {};
	r2[1].second = {};
	r2[2].second = {};
	r2[3].second = {};
	r2[4].second = {};


	std::vector<int> v11 = {1,4,3,7,10,11,15};
	std::vector<int> v12 = {1,7,10,2,5,9,12};
	std::vector<int> v13 = {1,2,9,8,13,14};
	std::vector<int> v14 = {1,7,13,6};


	std::vector<int> v21 = {10,15,6,8,13,14,7,2};
	std::vector<int> v22 = {11,4,5,3,6,13};
	std::vector<int> v23 = {1,9,15,5,3};
	std::vector<int> v24 = {12,13,8};

	for (auto e: v11)
		r1[1].second.insert(e);

	for (auto e: v12)
		r1[2].second.insert(e);
	
	for (auto e: v13)
		r1[3].second.insert(e);

	for (auto e: v14)
		r1[4].second.insert(e);
	
	for (auto e: v21)
		r2[1].second.insert(e);

	for (auto e: v22)
		r2[2].second.insert(e);
	
	for (auto e: v23)
		r2[3].second.insert(e);

	for (auto e: v24)
		r2[4].second.insert(e);


	transform(r1, k, 0, 15);
	transform(r2, k, 1, 15);

	std::pair<int, std::set<int>>** R_total = (std::pair<int, std::set<int>>**)malloc(sizeof(std::pair<int, std::set<int>>*) * m);
	R_total[0] = r1;
	R_total[1] = r2;
	
	reduce (R_total, m);

	std::vector<std::pair<int, std::set<int>>> final(R_total[0][0].first);
	
	for (size_t i = 1; i <= R_total[0][0].first; i++) {
		
		for (auto e: R_total[0][i].second) {
				final[i - 1].second.insert(e);	
		}
		final[i - 1].first = R_total[0][i].first;
	}

	std::vector<int> seeds = max_cover(final, k);

	return 0;
	
	
}