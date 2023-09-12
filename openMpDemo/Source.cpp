#include<iostream>
#include"omp.h"
#include<chrono>
using namespace std;

const int arr_size = 20000;

int arr[arr_size][arr_size];

void init_arr();
long long part_sum(int, int, int);
long long part_min(int, int, int);
int sizeThread = 4;
string out1;
string out2;

//============================================================================================================================================================

int main() {
	
		init_arr();
		omp_set_nested(1); //включення можливості створення вкладенних парал потоків (1/0)
		double t1 = omp_get_wtime(); //повернення поточного часу
#pragma omp parallel sections  //директива для створення пралел секцій 
 	{
#pragma omp section
		{
			long long x = part_min(0, arr_size, sizeThread);
			printf("minsum (CountThread = %d) = %d \n", sizeThread, x);
		}
		
#pragma omp section
		{
			long long x = part_sum(0, arr_size, sizeThread);
			printf("sum (CountThread = %d) = %d \n", sizeThread, x);

		}
	}
		double t2 = omp_get_wtime();
	
		printf("Total time - %F seconds", t2 - t1);
		return 0;
}

//============================================================================================================================================================

void init_arr() {

	for (int i = 0; i < arr_size; i++) {
		for (size_t j = 0; j < arr_size; j++)
		{
			arr[i][j] = arr_size*arr_size - i*arr_size-j;
		}
	}

	arr[arr_size / 2][arr_size / 2] = 100; 
}

//============================================================================================================================================================
// виконує обчислення суми всіх елементів в двовимірному масиві
long long part_sum(int start_index, int finish_index, int num_threads) {

		long long sum = 0;
		double t1 = omp_get_wtime();
#pragma omp parallel for reduction(+:sum) num_threads(num_threads)	//директива вказує, що наступний цикл for має виконуватися паралельно за допомогою багатьох потоків(num_threads), потрібно використовувати операцію + для обчислення суми всіх елементів у змінній sum в кожному потоці.
		for (int i = start_index; i < finish_index; i++) {			
			for (size_t j = 0;  j < arr_size;  j++)
			{
				sum += arr[i][j];
			}
		}
		double t2 = omp_get_wtime();
		printf("sum time - %F seconds \n", t2 - t1);

		return sum;
}

//============================================================================================================================================================

long long part_min(int start_index, int finish_index, int num_threads) {

		long long minsum = INT32_MAX;
		int min_row = -1; 
		double t1 = omp_get_wtime();
#pragma omp parallel for num_threads(num_threads)
		for (int i = start_index; i < finish_index; i++) {
			long long sum = 0;
			for (size_t j = 0; j < arr_size; j++)
			{
				sum += arr[i][j];
			}
		
			if (minsum > sum)
			{
#pragma omp critical // доступ ло блоку коду всереденні critical може бути здійснений лише одним поток(одночасно)
				if (minsum > sum)
				{
					min_row = i;
					minsum = sum;
				}
			}
		}
		double t2 = omp_get_wtime();
		printf("min sum time - %F seconds \n", t2 - t1);
		printf("Row with minimum sum: %d\n", min_row);


		return minsum;
}