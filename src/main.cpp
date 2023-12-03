#include <algorithm>
#include <iostream>
#include <vector>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <memory>

/*
Сделать систему, которая решает такую задачу: 
Реализовать квадратичную (с квадратичной сложностью, например, пузырьковая) 
и быструю (с н лог по н временем выполнения) сортировки, 
должна выводиться сравнительная таблица: два вектора на 1000000 template-овых (для инта и дабла) 
элементов и 10 циклов проверки времени сортировки 
В таблице три строки: сортировка первая, вторая, третья - из стандартной библиотеки algorithm(кажется?) 
найти худшее, среднее и лучшее время сортировки 
Воспользоваться знаниями по умным указателем, чтобы не плодить копии, генератор случайных массивов для инта и дабла.
*/

//cmake -G "MinGW Makefiles" ../
//cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ../

// Генератор массива
template <typename T, std::size_t N>
std::vector<T> generateTemplateArray() 
{
    srand(time(NULL));
    auto result = std::vector<T>(N);
    for (std::size_t i = 0; i < N; ++i) 
    {
        result[i] = static_cast<T>(rand());
        // result[i] = static_cast<T>(0 + rand() % (10 - 0 +1));
    }
    return result;
}

// Алгоритм пузырьковой сортировки (по возрастанию)
template <typename T>
void bubbleSort(std::vector<T>& arr) 
{
    const std::size_t N = arr.size();
    bool flag;
    for(std::size_t i = 0; i < N; i++) 
    {
        for(std::size_t j = 0; j < N - i - 1; j++) 
        {
            //flag = false;
            if(arr[j] > arr[j+1]) 
            {
                T tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
                /*
                std::swap(arr[j], arr[j + 1]);
                flag = true;
                */
            }
        }
        /*
        // Если на текущей итерации не было обменов, то массив уже отсортирован
        if (!flag) 
        {
            break;
        }
        */
    }
}

// Вспомогательная функция для быстрой сортировки: разделение массива
template <typename T>
int partition(std::vector<T>& arr, int low, int high) 
{
    T pivot = arr[high];    // Выбираем последний элемент в качестве опорного
    int i = low - 1;        // Индекс меньшего элемента

    for (int j = low; j <= high - 1; j++) 
    {
        // Если текущий элемент меньше или равен опорному
        if (arr[j] <= pivot) 
        {
            i++;    // Увеличиваем индекс меньшего элемента
            std::swap(arr[i], arr[j]);
        }
    }

    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

// Алгоритм быстрой сортировки
template <typename T>
void quickSort(std::vector<T>& arr, int low, int high) 
{
    if (low < high) 
    {
        // Разделение и получение индекса опорного элемента
        int pivotIndex = partition(arr, low, high);

        // Рекурсивно сортируем две половины
        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

// Быстрая сортировка std
template<typename T>
void quickSortStd(std::vector<T>& arr)
{
    std::sort(arr.begin(), arr.end());
}

// Функция для измерения времени сортировки пузырьком
template<typename T>
unsigned long long durationBubbleSort(T& arr) // ms
{
    auto sortedArray = arr;
    auto start = std::chrono::high_resolution_clock::now();
    bubbleSort(sortedArray);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

// Функция для измерения времени быстрой сортировкой
template<typename T>
unsigned long long durationQuickSort(T& arr) // ms
{
    auto sortedArray = arr;
    auto start = std::chrono::high_resolution_clock::now();
    quickSort(sortedArray, 0, sortedArray.size() - 1);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

// Функция для измерения времени быстрой сортировкой std
template<typename T>
unsigned long long durationQuickSortStd(T& arr) // ms
{
    auto sortedArray = arr;
    auto start = std::chrono::high_resolution_clock::now();
    quickSortStd(sortedArray);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    return duration.count();
}

// Функция для измерения времени выполнения функции
template <typename Func, typename... Args>
double measureTime(Func&& func, Args&&... args)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::forward<Func>(func)(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main()
{
    std::cout << "Start program..." << std::endl << std::endl;

    constexpr std::size_t arraySize_1 = 10000; // количество элементов в массиве для пузырьковой сортировки 
    constexpr std::size_t arraySize_2 = 1000000; // количество элементов в массиве для быстрой сортировки
    constexpr int numTrials = 10; // Количество повторений для измерения времени

    // Генерация массивов
    auto intArrayBubbleSort = generateTemplateArray<int, arraySize_1>();
    auto doubleArrayBubbleSort = generateTemplateArray<double, arraySize_1>();
    auto intArrayQuickSort = generateTemplateArray<int, arraySize_2>();
    auto doubleArrayQuickSort = generateTemplateArray<double, arraySize_2>();

    // Измерение времени генерации массивов
    double generationTime_1 = measureTime(generateTemplateArray<int, arraySize_1>);
    double generationTime_2 = measureTime(generateTemplateArray<double, arraySize_1>);
    double generationTime_3 = measureTime(generateTemplateArray<int, arraySize_2>);
    double generationTime_4 = measureTime(generateTemplateArray<double, arraySize_2>);

    std::cout << "Generation time of intArray[10 000]: " << generationTime_1 << " seconds\n";
    std::cout << "Generation time of doubleArray[10 000]: " << generationTime_2 << " seconds\n";
    std::cout << "Generation time of intArray[1 000 000]: " << generationTime_3 << " seconds\n";
    std::cout << "Generation time of doubleArray[1 000 000]: " << generationTime_4 << " seconds\n\n";

    // Test № 1. Сортировка пузырьком вектора<int> на 10 000 элементов 
    std::cout <<"\nVector<int>[10 000] Bubble Sort test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    int bestSortTime = -1;
    int worstSortTime = -1;
    double totalSortTime = 0.;

    int duration;
    for(int i = 0; i < numTrials; i++)
    {
        duration = durationBubbleSort(intArrayBubbleSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;

    // Test № 2. Сортировка пузырьком вектора<double> на 10 000 элементов
    std::cout <<"\nVector<double>[10 000] Bubble Sort test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    bestSortTime = -1;
    worstSortTime = -1;
    totalSortTime = 0.;

    for(int i = 0; i < numTrials; i++)
    {
        duration = durationBubbleSort(doubleArrayBubbleSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;

    // Test № 3. Быстрая сортировка вектора<int> на 1 000 000 элементов 
    std::cout <<"\nVector<int>[1 000 000] Quick Sort test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    bestSortTime = -1;
    worstSortTime = -1;
    totalSortTime = 0.;

    for(int i = 0; i < numTrials; i++)
    {
        duration = durationQuickSort(intArrayQuickSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;

    // Test № 4. Быстрая сортировка вектора<double> на 1 000 000 элементов 
    std::cout <<"\nVector<double>[1 000 000] Quick Sort test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    bestSortTime = -1;
    worstSortTime = -1;
    totalSortTime = 0.;

    for(int i = 0; i < numTrials; i++)
    {
        duration = durationQuickSort(doubleArrayQuickSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;

    // Test № 5. Быстрая сортировка вектора<int> на 1 000 000 элементов 
    std::cout <<"\nVector<int>[1 000 000] Quick Sort std test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    bestSortTime = -1;
    worstSortTime = -1;
    totalSortTime = 0.;

    for(int i = 0; i < numTrials; i++)
    {
        duration = durationQuickSortStd(intArrayQuickSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;

    // Test № 6. Быстрая сортировка вектора<double> на 1 000 000 элементов 
    std::cout <<"\nVector<double>[1 000 000] Quick Sort std test :\n\n";
    std::cout <<"Iter\t\tDuration\n";
    bestSortTime = -1;
    worstSortTime = -1;
    totalSortTime = 0.;

    for(int i = 0; i < numTrials; i++)
    {
        duration = durationQuickSortStd(doubleArrayQuickSort);
        std::cout <<i+1<<"\t\t" << duration <<" ms\n";
        if(bestSortTime < 0) bestSortTime = duration;
        if(worstSortTime < 0) worstSortTime = duration;
        if(worstSortTime < duration) worstSortTime = duration;
        if(bestSortTime > duration) bestSortTime = duration;
        totalSortTime += duration;
    }
    totalSortTime /= numTrials;

    std::cout <<"\n";
    std::cout << "Best Sort Time: " << bestSortTime << " ms\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " ms\n";
    std::cout << "Average Sort Time: " << totalSortTime << " ms\n";
    std::cout <<"--------------------------\n"<< std::endl;
/*
    // Измерение времени сортировки "Пузырек" 
    double sortTime_1 = measureTime(bubbleSort<int>, intArray);
    std::cout << "Sorting time of intArray: " << sortTime_1 << " seconds\n";
    double sortTime_2 = measureTime(bubbleSort<double>, doubleArray);
    std::cout << "Sorting time of doubleArray: " << sortTime_2 << " seconds\n";
    
    // Измерение времени сортировки "Быстрая сортировка" 
    double sortTime_1 = measureTime(quickSort<int>, *intArray, 0, intArray->size() - 1);
    std::cout << "Sorting time of intArray: " << sortTime_1 << " seconds\n";
    double sortTime_2 = measureTime(quickSort<double>, *doubleArray, 0, doubleArray->size() - 1); 
    std::cout << "Sorting time of doubleArray: " << sortTime_2 << " seconds\n";
*/
/*
    double totalSortTime = 0.0;
    double bestSortTime = std::numeric_limits<double>::max();
    double worstSortTime = 0.0;

    for (int i = 0; i < numTrials; ++i)
    {
        auto sortedArray = intArray; // Копия массива для каждого теста

        // Измерение времени быстрой сортировки
        double sortTime = measureTime(quickSort<int>, intArray, 0, intArray.size() - 1);
        std::cout << "Sort time " << i << " iteration: " << sortTime << std::endl;
        totalSortTime += sortTime;

        // Обновление лучшего и худшего времени
        bestSortTime = std::min(bestSortTime, sortTime);
        worstSortTime = std::max(worstSortTime, sortTime);
    }

    double averageSortTime = totalSortTime / numTrials;

    std::cout << "Best Sort Time: " << bestSortTime << " seconds\n";
    std::cout << "Worst Sort Time: " << worstSortTime << " seconds\n";
    std::cout << "Average Sort Time: " << averageSortTime << " seconds\n";
*/
/*    
    // Генерация массивов типа int и double
    auto begin1 = std::chrono::steady_clock::now();
    // std::vector<int> intArray = generateTemplateArray<int, arraySize>();
    auto intArray = generateTemplateArray<int, arraySize>();
    auto end1 = std::chrono::steady_clock::now();
    auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
    std::cout << "The time of generate intArray: " << elapsed_ms1.count() << " ms\n";

    clock_t start2 = clock();
    // std::vector<double> doubleArray = generateTemplateArray<double, arraySize>();
    auto doubleArray = generateTemplateArray<double, arraySize>();
    clock_t end2 = clock();
    double seconds2 = (double)(end2 - start2) / CLOCKS_PER_SEC;
    printf("The time of generate doubleArray: %f seconds\n", seconds2);

    clock_t start3 = clock();
    bubleSort<int>(intArray);
    clock_t end3 = clock();
    double seconds3 = (double)(end3 - start3) / CLOCKS_PER_SEC;
    printf("The time of bubleSort intArray: %f seconds\n", seconds3);
*/  
/*
    // Вывод значений массива
    std::cout << "\nGenerated int Array: ";
    for (const auto& value : doubleArrayQuickSort) 
    {
        std::cout << value << " ";
    }
    std::cout << std::endl << std::endl;

    std::cout << "Generated double Array: ";
    for (const auto& value : doubleArrayQuickSort) 
    {
        std::cout << value << " ";
    }
    std::cout << std::endl << std::endl;
*/
    return 0;
}
