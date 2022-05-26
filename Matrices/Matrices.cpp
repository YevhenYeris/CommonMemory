#include <iostream>
#include <cstdio>
#include <thread>
#include <future>
#include <cassert>

std::mutex cout_lock;

void printMatrix(const std::vector<std::vector<int>>& m)
{
	for (int i = 0; i < m.size(); ++i) {
		std::cout << "                 ";
		for (int j = 0; j < m[i].size(); ++j) {
			std::cout << m[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void printMatrices(const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b, const std::vector<std::vector<int>>& c)
{
	std::cout << "\nMatrix A[n x m]: \n";
	printMatrix(a);
	std::cout << "\nMatrix B[m x k]: \n";
	printMatrix(b);
	std::cout << "\nMatrix C[n x k]: \n";
	printMatrix(c);
}

void multMatrices(int i, int j, const std::vector<std::vector<int>>& a, const std::vector<std::vector<int>>& b, std::vector<std::vector<int>>& c)
{
	int n = b.size();

	int result = 0;
	for (int k = 0; k < n; ++k) {
		result += a[i][k] * b[k][j];
	}

	std::lock_guard<std::mutex> lock(cout_lock);
	std::cout << "C[" << i + 1 << "]" << "[" << j + 1 << "] = ";
	std::cout << "A[" << i + 1 << "][_]" << " x " << "B[_][" << j + 1 << "] = " << result << std::endl;
	c[i][j] = result;
}

std::vector<std::vector<int>> genMatrix(int n, int m)
{
	srand(time(0));
	std::vector<std::vector<int>> res(n, std::vector<int>(m, 0));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			res[i][j] = rand() % 10;
		}
	}
	return res;
}

std::vector<std::vector<int>> inputMatrix(int n, int m)
{
	srand(time(0));
	std::vector<std::vector<int>> res(n, std::vector<int>(m, 0));

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			std::cout << "[" << i << "][" << j << "] = ";
			std::cin >> res[i][j];
		}
	}
	return res;
}

int main()
{
	int n, m, k;
	std::cout << "Please, enter n, m, k:\n	";
	std::cin >> n >> m >> k;
	std::cout << std::endl;

	std::vector<std::vector<int>> A(n, std::vector<int>(m, 0));
	std::vector<std::vector<int>> B(m, std::vector<int>(k, 0));
	std::vector<std::vector<int>> C(n, std::vector<int>(k, 0));

	std::cout << "Generate random? [Y\\N]\n";
	if (std::cin.get() == 'Y' || std::cin.get() == 'y') {
		A = genMatrix(n, m);
		B = genMatrix(m, k);
	}
	else {
		A = inputMatrix(n, m);
		B = inputMatrix(m, k);
	}

	std::cout << "\nMultiplying matrices...\n";
	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

	std::vector<std::thread> threads;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < k; ++j) {
			threads.push_back(std::thread(multMatrices, i, j, cref(A), cref(B), ref(C)));
		}
	}

	for (int i = 0; i < n * k; ++i) {
		threads[i].join();
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	std::cout << "\nMultiplication finished\nTime: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
	printMatrices(A, B, C);

	return 0;
}
