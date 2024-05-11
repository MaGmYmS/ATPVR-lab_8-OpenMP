#include <iostream>
#include <omp.h>

void parallel_sum_variable(int N, int num_threads) {
    int total_sum = 0;

    #pragma omp parallel if(N > num_threads) num_threads(num_threads) reduction(+:total_sum) // Создаются временные переменные, куда записывается сумма. После, эти переменные складываются с основной переменной
    {
        int id = omp_get_thread_num(); // Получаем номер текущей нити
        int partial_sum = 0;

        // Вычисляем частичную сумму для каждой нити
        for (int i = 1 + id; i <= N; i += num_threads) {
            partial_sum += i;
            std::cout << "[" << id << "]: Sum = " << partial_sum << std::endl;
        }

        total_sum += partial_sum;
    }
    std::cout << "Sum = " << total_sum << std::endl;
}

void parallel_sum_for(int N, int k) {
    int total_sum = 0;

    #pragma omp parallel num_threads(k)
    {
        int id = omp_get_thread_num(); // Получаем номер текущей нити
        int partial_sum = 0;

        // Распараллеливаем цикл с помощью директивы for
        #pragma omp for
        for (int i = 1; i <= N; ++i) {
            partial_sum += i;
        }
        std::cout << "[" << id << "]: Sum = " << partial_sum << std::endl;

        #pragma omp critical
        total_sum += partial_sum;
    }
    std::cout << "Sum = " << total_sum << std::endl;
}


int main() {
    int N = 10;
    int num_threads = 5;
    parallel_sum_variable(N, num_threads);

    parallel_sum_for(N, num_threads);

    return 0;
}
