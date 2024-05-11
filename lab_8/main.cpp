#include <iostream>
#include <omp.h>

void parallel_sum(int N, int num_threads) {
    int total_sum = 0;

    #pragma omp parallel if(N > num_threads) num_threads(num_threads) reduction(+:total_sum)
    {
        int id = omp_get_thread_num(); // Получаем номер текущей нити
        int partial_sum = 0;

        // Вычисляем частичную сумму для каждой нити
        for (int i = 1 + id; i <= N; i += num_threads) {
            partial_sum += i;
            std::cout << "[" << id << "]: Sum = " << partial_sum << std::endl;
        }
    }

    std::cout << "Sum = " << total_sum << std::endl;
}

int main() {
    int N = 10;
    int num_threads = 5;

    parallel_sum(N, num_threads);

    return 0;
}
