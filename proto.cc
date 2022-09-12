#include "utility.h"
#include <mpi.h>

int main(int argc, char *argv[]) {

	size_t k = 4;
	size_t m = 2;
	int rank, p;
	MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    std::pair<int, std::set<int>>* r = new std::pair<int, std::set<int>>[m * k + 1];

	if (rank == 0) {
		

		r[0].first = k;
		r[1].first = 1;
		r[2].first = 10;
		r[3].first = 5;
		r[4].first = 7;
		r[0].second = {};
		r[1].second = {};
		r[2].second = {};
		r[3].second = {};
		r[4].second = {};

		std::vector<int> v11 = {1,4,3,7,10,11,15};
		std::vector<int> v12 = {1,7,10,2,5,9,12};
		std::vector<int> v13 = {1,2,9,8,13,14};
		std::vector<int> v14 = {1,7,13,6};

		for (auto e: v11)
			r[1].second.insert(e);

		for (auto e: v12)
			r[2].second.insert(e);
		
		for (auto e: v13)
			r[3].second.insert(e);

		for (auto e: v14)
			r[4].second.insert(e);

	}

	else {

		r[0].first = k;
		r[1].first = 1;
		r[2].first = 10;
		r[3].first = 7;
		r[4].first = 3;
		r[0].second = {};
		r[1].second = {};
		r[2].second = {};
		r[3].second = {};
		r[4].second = {};

		std::vector<int> v21 = {10,15,6,8,13,14,7,2};
		std::vector<int> v22 = {11,4,5,3,6,13};
		std::vector<int> v23 = {1,9,15,5,3};
		std::vector<int> v24 = {12,13,8};
		
		for (auto e: v21)
			r[1].second.insert(e);

		for (auto e: v22)
			r[2].second.insert(e);
		
		for (auto e: v23)
			r[3].second.insert(e);

		for (auto e: v24)
			r[4].second.insert(e);

	}
	
	transform(r, k, 0, 15);


	int count = 0;
	for (size_t i = 1; i <= r[0].first; i++) {
		count += r[i].second.size();
	}

	int total_size = (r[0].first + 1) * 2 + count;

	int *packed = (int *)malloc(total_size * sizeof(int));

	pack(r, packed, total_size);

	std::vector<std::pair<int, std::set<int>>> R = unpack(packed);

	if (rank == 0) {

		int displacement[m] = {};
		int counts[m] = {};

#pragma omp for
		for (size_t i = 0; i < m; ++i) {
			displacement[i] = i;
			counts[i] = 1;
		}

		int* buffer = (int*)malloc(m * sizeof(int));

		MPI_Gatherv(&total_size, 1, MPI_INT, buffer, counts, displacement, MPI_INT, 0, MPI_COMM_WORLD);

		for (size_t i = 0; i < m; ++i) {
			std::cout << buffer[i] << " ";
		}
		free(buffer);

	}
	else {

		MPI_Gatherv(&total_size, 1, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);


	}
	
	MPI_Finalize();

}
