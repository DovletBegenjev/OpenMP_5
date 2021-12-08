// OpenMP_4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <omp.h>
#include <ctime>
#include <fstream>

using namespace std;

const int arr_size = 1000;

void num_file()
{
	ofstream out;

	out.open("numbers1.txt");
	for (int i = 0; i < arr_size; i++)
	{
		for (int j = 0; j < arr_size; j++)
		{
			out << 1 + rand() % 10 << " ";
		}
		out << endl;
	}
	out.close();

	out.open("numbers2.txt");
	for (int i = 0; i < arr_size; i++)
	{
		for (int j = 0; j < arr_size; j++)
		{
			out << 1 + rand() % 10 << " ";
		}
		out << endl;
	}
	out.close();
}

int main(int argc, char** argv)
{
	double* arr1 = new double[arr_size*arr_size];
	double* arr2 = new double[arr_size*arr_size];
	double* arr3 = new double[arr_size*arr_size];

	long double S = 0;

	num_file();

	int rank = omp_get_thread_num(), size = omp_get_num_procs();

	// Считаем матрицу из файла
	ifstream in("numbers1.txt");
	for (long long i = 0; i < arr_size; i++)
		for (long long j = 0; j < arr_size; j++)
			in >> arr1[i*arr_size + j];
	in.close();

	in.open("numbers2.txt");
	for (long long i = 0; i < arr_size; i++)
		for (long long j = 0; j < arr_size; j++)
			in >> arr2[i*arr_size + j];
	in.close();

	int threads = 2;

	double time = omp_get_wtime();

	int i, j;

#pragma omp parallel num_threads(threads)
	{
		// Евклидова норма матрицы
#pragma omp sections reduction(+: S) private(i, j)
#pragma omp section
		{
			for (i = 0; i < arr_size; ++i)
			{
				for (j = 0; j < arr_size; ++j)
				{
					S += arr1[i*arr_size + j] * arr1[i*arr_size + j];
				}
			}
		}

#pragma omp section
		{
			for (i = 0; i < arr_size; ++i)
			{
				for (j = 0; j < arr_size; ++j)
				{
					arr3[i*arr_size + j] = S * arr1[i*arr_size + j] + arr2[i*arr_size + j] - arr1[i*arr_size + j];
				}
			}
		}
	}

	S = sqrt(S);
	printf("E = %f\n", S);

	double time2 = omp_get_wtime() - time;
	printf("time = %f\n", time2);

	ofstream out("out.txt");
	for (int i = 0; i < arr_size; i++)
	{
		for (int j = 0; j < arr_size; j++)
		{
			out << arr3[i*arr_size + j] << " ";
		}
		out << endl;
	}
}
