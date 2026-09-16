#include <iostream>
#include <vector>
#include <climits>

int CountSum(const std::vector<int>& numbers)
{
    int sum = 0;

    for (int i = 0; i < numbers.size(); i++)
    {
        sum += numbers[i];
    }

    return sum;
}

float CountMiddle(const std::vector<int>& numbers)
{
    return static_cast<float>(CountSum(numbers)) / numbers.size();
}

int FindMax(const std::vector<int>& numbers)
{
    int max = INT_MIN;

    for (int i = 0; i < numbers.size(); i++)
    {
        max = (max < numbers[i]) ? numbers[i] : max;
    }

    return max;
}

int FindMin(const std::vector<int>& numbers)
{
    int min = INT_MAX;

    for (int i = 0; i < numbers.size(); i++)
    {
        min = (min > numbers[i]) ? numbers[i] : min;
    }

    return min;
}

int main()
{
    std::vector<int> numbers;
    int N = 0;

    std::cout << "Enter the number of elements: ";
    std::cin >> N;

    for (int i = 0; i < N; i++)
    {
        int n = 0;
        std::cin >> n;
        numbers.push_back(n);
    }

    std::cout << "Sum: " << CountSum(numbers) << std::endl;
    std::cout << "Middle: " << CountMiddle(numbers) << std::endl;
    std::cout << "Minimum: " << FindMin(numbers) << std::endl;
    std::cout << "Maximum: " << FindMax(numbers) << std::endl;

    return 0;
}