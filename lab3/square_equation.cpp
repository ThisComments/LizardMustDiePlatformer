#include <iostream>
#include <cmath>

int main()
{
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    std::cout << "Enter coefficients a, b and c for 'ax^2 + bx + c = 0': ";
    std::cin >> a >> b >> c;

    if (a != 0.0f)
    {
        float D = b * b - 4 * a * c;
        if (D > 0.0f)
        {
            float x_1 = (-b + std::sqrt(D)) / 2 * a;
            float x_2 = (-b - std::sqrt(D)) / 2 * a;
            std::cout << "Solution:" << std::endl << "x_1 = " << x_1 << std::endl << "x_2 = " << x_2 << std::endl;
        } 
        else if (D == 0.0f)
        {
            float x = -b / 2 * a;
            std::cout << "Solution x = " << x << std::endl;
        }
        else
        {
            std::cout << "There are no real solution." << std::endl;
        }
    }
    else 
    {
        std::cout << "Error: coefficient 'a' cannot be zero." << std::endl;
        return 1;
    }

    return 0;
}
