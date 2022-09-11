#include "utility.h"


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

	int count = 0;
	for (size_t i = 1; i <= r2[0].first; i++) {
		count += r2[i].second.size();
	}

	int total_size = (r2[0].first + 1) * 2 + count;

	int *packed = (int *)malloc(total_size * sizeof(int));

	pack(r2, packed, total_size);

	std::vector<std::pair<int, std::set<int>>> R3 = unpack(packed);
	
	return 0;

}