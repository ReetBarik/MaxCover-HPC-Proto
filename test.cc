#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <set>
#include <omp.h>
#include <cmath>

void transform(std::pair<int, std::set<int>>* R, int k, int m, int max) {

#pragma omp for
	for (size_t i = 0; i < k; i++) {

		std::set<int> temp = {};
		
		for (auto e: R[i].second)
			temp.insert(m * max + e);

		R[i].second = temp; 
	}
}


void merge (std::pair<int, std::set<int>>* R1, std::pair<int, std::set<int>>* R2, int k) {

	std::set<int> s = {};
	std::vector<std::pair<int, std::set<int>>> res;


	for (size_t i = 0; i < k; i++) {

		for (size_t j = 0; j < k; j++) {

			if (R1[i].first == R2[j].first) {

				for (auto e: R2[j].second) {
					R1[i].second.insert(e);
				}

				s.insert(R1[i].first);
				res.push_back(R1[i]);
				break;

			}
		} 
	}


	for (size_t j = 0; j < k; j++) {

		if (s.find(R1[j].first) == s.end()) {
			res.push_back(R1[j]);
		}
	}


	for (size_t j = 0; j < k; j++) {

		if (s.find(R2[j].first) == s.end()) {
			res.push_back(R2[j]);
		}

	}
	
	R1 = &res[0];

	for (auto e: R1[4].second)
		printf("%d\t", e);

}


void reduce (std::pair<int, std::set<int>>** R, int m, int k) {

	for (size_t i = 0; i < std::log2(m); ++i) {
#pragma omp for
		for (size_t j = 0; j < m; j = j + (2 * (i + 1)))		
			merge(R[j], R[j + (int)std::pow(2,i)], k);

	}
}

int main(int argc, char *argv[]) {

	size_t k = 4;
	size_t m = 2;

	using RRR = std::set<int>;
	using RRRIDS = std::pair<int, RRR>;

	std::vector<RRRIDS> R1(k);
	std::vector<RRRIDS> R2(k);

	R1[0].first = 1;
	R1[1].first = 10;
	R1[2].first = 5;
	R1[3].first = 7;


	R2[0].first = 1;
	R2[1].first = 10;
	R2[2].first = 7;
	R2[3].first = 3;


	std::vector<int> v11 = {1,4,3,7,10,11,15};
	std::vector<int> v12 = {1,7,10,2,5,9,12};
	std::vector<int> v13 = {1,2,9,8,13,14};
	std::vector<int> v14 = {1,7,13,6};


	std::vector<int> v21 = {10,15,6,8,13,14,7,2};
	std::vector<int> v22 = {11,4,5,3,6,13};
	std::vector<int> v23 = {1,9,15,5,3};
	std::vector<int> v24 = {12,13,8};

	for (auto e: v11)
		R1[0].second.insert(e);

	for (auto e: v12)
		R1[1].second.insert(e);
	
	for (auto e: v13)
		R1[2].second.insert(e);

	for (auto e: v14)
		R1[3].second.insert(e);
	
	for (auto e: v21)
		R2[0].second.insert(e);

	for (auto e: v22)
		R2[1].second.insert(e);
	
	for (auto e: v23)
		R2[2].second.insert(e);

	for (auto e: v24)
		R2[3].second.insert(e);

	RRRIDS* r1 = &R1[0];
	RRRIDS* r2 = &R2[0];

	RRRIDS* res = (RRRIDS*)malloc(sizeof(RRRIDS) * k);
	transform(r1, k, 0, 15);
	transform(r2, k, 1, 15);

	RRRIDS** R_total = (RRRIDS**)malloc(sizeof(RRRIDS*) * m);
	R_total[0] = r1;
	R_total[1] = r2;

	reduce (R_total, m, k);

	RRRIDS* final = (RRRIDS*)malloc(sizeof(RRRIDS));
	final = R_total[0];
	for (auto e: final[4].second)
		// printf("%d\t", e);	


	return 0;
	
	
}