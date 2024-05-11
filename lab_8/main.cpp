#include <iostream>
#include <omp.h>
#include <vector>
#include <unordered_map>
#include <string>

void parallel_sum_variable(int N, int num_threads) {
    int total_sum = 0;

    #pragma omp parallel if(N > num_threads) num_threads(num_threads) reduction(+:total_sum) // ��������� ��������� ����������, ���� ������������ �����. �����, ��� ���������� ������������ � �������� ����������
    {
        int id = omp_get_thread_num(); // �������� ����� ������� ����
        int partial_sum = 0;

        // ��������� ��������� ����� ��� ������ ����
        for (int i = 1 + id; i <= N; i += num_threads) {
            partial_sum += i;
            std::cout << "[" << id << "]: Sum = " << partial_sum << std::endl;
        }

        total_sum += partial_sum;
    }
    std::cout << "Sum = " << total_sum << std::endl;
}


std::vector<int> parallel_sum_for(int N, int k, const char* schedule_type) {
    int total_sum = 0;
    std::vector<int> iteration_numbers;
    std::cout << "Schedule type: " << schedule_type << std::endl;

    #pragma omp parallel num_threads(k)
    {
        int id = omp_get_thread_num(); // �������� ����� ������� ����
        int partial_sum = 0;

        // ������ ��� �������� ������� ��������
        std::vector<int> thread_iterations;

        int chunk_size = 1; // �� ��������� chunk_size = 1

        // ��������� schedule_type
        std::string schedule(schedule_type);
        size_t comma_pos = schedule.find(',');
        std::string schedule_type_ = schedule.substr(0, comma_pos);
        if (comma_pos != std::string::npos) {
            std::string chunk_size_str = schedule.substr(comma_pos + 1); // �������� +1, ����� ���������� ","
            chunk_size = std::stoi(chunk_size_str);
        }

        // ���������������� ���� � ������� ��������� for � ��������� �������� schedule
        if (schedule_type_ == "static" ) {
            #pragma omp for schedule(static, chunk_size)
            for (int i = 1; i <= N; ++i) {
                // ��������� ����� ��������
                thread_iterations.push_back(i);

                // ������� ��������� � ������� ��������
                #pragma omp critical
                std::cout << "[" << id << "]: calculation of the iteration number " << i << "." << std::endl;

                partial_sum += i;
            }
        }
        else if (schedule_type_ == "dynamic") {
            #pragma omp for schedule(dynamic, chunk_size)
            for (int i = 1; i <= N; ++i) {
                // ��������� ����� ��������
                thread_iterations.push_back(i);

                // ������� ��������� � ������� ��������
                #pragma omp critical
                std::cout << "[" << id << "]: calculation of the iteration number " << i << "." << std::endl;

                partial_sum += i;
            }
        }
        else if (schedule_type_ == "guided") {
            #pragma omp for schedule(guided, chunk_size)
            for (int i = 1; i <= N; ++i) {
                // ��������� ����� ��������
                thread_iterations.push_back(i);

                // ������� ��������� � ������� ��������
                #pragma omp critical
                std::cout << "[" << id << "]: calculation of the iteration number " << i << "." << std::endl;

                partial_sum += i;
            }
        }


        // ���������� ������ �������� �� ���� �����
        #pragma omp critical
        iteration_numbers.insert(iteration_numbers.end(), thread_iterations.begin(), thread_iterations.end());

        // ��������� ��������� �����
        #pragma omp critical
        total_sum += partial_sum;

        // ������� ��������� ����� ��� ������ ����
        std::cout << "[" << id << "]: Sum = " << partial_sum << std::endl;
    }

    // ������� ����� �����
    std::cout << "Sum = " << total_sum << std::endl;

    // ���������� ������ � �������� ��������
    return iteration_numbers;
}

int main() {
    int k = 4; 
    int N = 10; 

    const char* schedule_types[] = { "static", "static, 1", "static, 2", "dynamic", "dynamic, 2", "guided", "guided, 2" };
    int num_schedule_types = sizeof(schedule_types) / sizeof(schedule_types[0]);

    // ������� ��� �������� �����������
    std::unordered_map<std::string, std::vector<int>> results;

    // ��������� ���������� ��� ������� ���� ����������
    for (int i = 0; i < num_schedule_types; ++i) {
        std::vector<int> iterations = parallel_sum_for(N, k, schedule_types[i]);
        results[schedule_types[i]] = iterations;
    }

    // ������� ����������
    std::cout << "Results:" << std::endl;
    for (const auto& entry : results) {
        std::cout << "Schedule type: " << entry.first << ", Iteration numbers: ";
        for (int i : entry.second) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}

