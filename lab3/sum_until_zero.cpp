#include <iostream>

int main()
{
    std::cout << "Enter the numbers: ";

    int n = -1;
    int count = 0;
    int sum = 0;

    while (n != 0)
    {
        std::cin >> n;

        if (n != 0)
        {
            count++;
            sum += n;
        }
    }

    if (count == 0)
    {
        std::cout << "Error: no numbers entered." << std::endl;
        return 1;
    }

    for (int i = 0; i < count; i++)
    {
        std::cout << "*";
    }

    std::cout << std::endl;

    return 0;
}