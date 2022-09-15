#include "utility.h"
#include <mpi.h>

int main(int argc, char *argv[]) {

	// mpicc -o pr proto.cc -fopenmp

	size_t k = 4;
	int rank, p;
	MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    std::pair<int, std::set<int>>* r = new std::pair<int, std::set<int>>[p * k + 1];

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
	
	transform(r, k, rank, 15);


	int count = 0;
	for (size_t i = 1; i <= r[0].first; i++) {
		count += r[i].second.size();
	}

	int total_size = (r[0].first + 1) * 2 + count;

	int *packed = (int *)malloc(total_size * sizeof(int));

	pack(r, packed, total_size);

	std::vector<std::pair<int, std::set<int>>> R = unpack(packed);

	if (rank == 0) {

		int displacement[p] = {};
		int counts[p] = {};

#pragma omp for
		for (size_t i = 0; i < p; ++i) {
			displacement[i] = i;
			counts[i] = 1;
		}

		int* buffer = (int*)malloc(p * sizeof(int));

		MPI_Gatherv(&total_size, 1, MPI_INT, buffer, counts, displacement, MPI_INT, 0, MPI_COMM_WORLD);

		int bufferSize = 0;
#pragma omp parallel for reduction(+ : bufferSize)
      	for (size_t i = 0; i < p; ++i)
      	{
        	bufferSize += buffer[i];
      	}

      	int* bigBuffer = (int*)malloc(bufferSize * sizeof(int));

#pragma omp for
		for (size_t i = 1; i < p; ++i) {
			displacement[i] = buffer[i - 1];
		}
		displacement[0] = 0;

      	MPI_Gatherv(packed, total_size, MPI_INT, bigBuffer, buffer, displacement, MPI_INT, 0, MPI_COMM_WORLD);

      	int** deLinearize = (int **)malloc(p * sizeof(int *));
      	std::pair<int, std::set<int>>** R_total = (std::pair<int, std::set<int>>**)malloc(sizeof(std::pair<int, std::set<int>>*) * p);
      	std::vector<std::vector<std::pair<int, std::set<int>>>> r_unpack(p);

#pragma omp for
      	for (size_t i = 0; i < p; i++) {

      		int start, end;
      		if (i == 0)
      			start = 0;
      		else 
      			start = buffer[i - 1];
      		end = start + buffer[i];

      		deLinearize[i] = (int *)malloc(buffer[i] * sizeof(int));
      		
      		for(size_t j = start; j < end; j++) {
      			deLinearize[i][j - start] = bigBuffer[j];
      		}

      		r_unpack[i] = unpack(deLinearize[i]);

      		R_total[i] = &r_unpack[i][0];      		
      	}

      	reduce (R_total, p);

		free(buffer);
		free(bigBuffer);

		std::vector<std::pair<int, std::set<int>>> final(R_total[0][0].first);

		for (size_t i = 1; i <= R_total[0][0].first; i++) {
			
			for (auto e: R_total[0][i].second) {
					final[i - 1].second.insert(e);	
			}
			final[i - 1].first = R_total[0][i].first;
		}

		std::vector<int> seeds = max_cover(final, k, 30);

		// for (auto e: seeds)
		// 	std::cout << e << " ";

	}
	else {

		MPI_Gatherv(&total_size, 1, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Gatherv(packed, total_size, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);

	}
	
	MPI_Finalize();

}
