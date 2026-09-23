#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Chizhov");
    sf::Color background = sf::Color::Green;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                {
                    window.close();
                }

                if (keyPressed->scancode == sf::Keyboard::Scancode::Space)
                {
                    background = sf::Color::Blue;
                }
            }
        }

        window.clear(background);
        window.display();
    }

    return 0;
}