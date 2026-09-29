#include <SFML/Graphics.hpp>

const sf::Vector2u WINDOW_SIZE{800, 600};

struct Car
{
    sf::Vector2f position;
    const float SPEED;
    const sf::Color colorBody;
    const sf::Color colorWheels;
    const sf::Vector2f rectangleSize;
    const sf::Vector2f circle1Offset;
    const sf::Vector2f circle2Offset;
    const float circleRadius;
    const sf::Vector2f triangleOffset;
    const std::vector<sf::Vector2f> trianglePoints;
};

struct Bounds
{
    sf::Vector2f position;
    sf::Vector2f size;
};

sf::RectangleShape CreateRectangle(
    const sf::Vector2f point, 
    const sf::Vector2f size, 
    const sf::Color color
)
{
    sf::RectangleShape rectangle(size);
    rectangle.setFillColor(color);
    rectangle.setPosition(point);

    return rectangle;
}

sf::CircleShape CreateCircle(
    const sf::Vector2f point, 
    const float radius, 
    const sf::Color color
)
{
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition(point);

    return circle;
}

sf::ConvexShape CreateConvex(
    const sf::Vector2f point,
    const std::vector<sf::Vector2f>& points,
    const sf::Color color
)
{
    sf::ConvexShape shape;

    shape.setFillColor(color);
    shape.setPosition(point);
    shape.setPointCount(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        shape.setPoint(i, points[i]);
    }

    return shape;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE), "Chizhov");
    sf::Color background = sf::Color::Green;

    Car playerCar
    {
        {300.f, 250.f},
        300.f,
        sf::Color::Red,
        sf::Color::Black,
        {100.f, 50.f},
        {0.f, 40.f},
        {100.f, 40.f},
        25.f,
        {100.f, 0.f},
        {
            {0.f, 50.f},
            {70.f, 50.f},
            {0.f, 0.f}
        }
    };

    sf::RectangleShape rectangle = CreateRectangle
    (
        playerCar.position, 
        playerCar.rectangleSize, 
        playerCar.colorBody
    );
    sf::CircleShape circle1 = CreateCircle
    (
        playerCar.position + playerCar.circle1Offset, 
        playerCar.circleRadius, 
        playerCar.colorWheels
    );
    sf::CircleShape circle2 = CreateCircle
    (
        playerCar.position + playerCar.circle2Offset, 
        playerCar.circleRadius, 
        playerCar.colorWheels
    );
    sf::ConvexShape triangle = CreateConvex
    (
        playerCar.position + playerCar.triangleOffset, 
        playerCar.trianglePoints,
        playerCar.colorBody
    );

    sf::Clock clock;

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

        float dt = clock.restart().asSeconds();
        sf::Vector2f direction{0.f, 0.f};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            direction.x += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            direction.x -= 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            direction.y += 1.f;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            direction.y -= 1.f;
        }

        if (direction.length() > 0.f)
        {
            direction /= direction.length();
        }
        sf::Vector2f velocity = direction * playerCar.SPEED * dt;

        playerCar.position += velocity;
        rectangle.move(velocity);
        triangle.move(velocity);
        circle1.move(velocity);
        circle2.move(velocity);

        Bounds boundsPlayerCar
        {
            playerCar.position,
            {
                playerCar.triangleOffset.x + triangle.getGlobalBounds().size.x, 
                playerCar.circle1Offset.y + playerCar.circleRadius * 2
            }
        };

        if (boundsPlayerCar.position.x <= 0.f)
        {
            playerCar.position.x = 0.f;
        }
        if (boundsPlayerCar.position.y <= 0.f)
        {
            playerCar.position.y = 0.f;
        }
        if (boundsPlayerCar.position.x + boundsPlayerCar.size.x >= WINDOW_SIZE.x)
        {
            playerCar.position.x = WINDOW_SIZE.x - boundsPlayerCar.size.x;
        }
        if (boundsPlayerCar.position.y + boundsPlayerCar.size.y >= WINDOW_SIZE.y)
        {
            playerCar.position.y = WINDOW_SIZE.y - boundsPlayerCar.size.y;
        }

        rectangle.setPosition(playerCar.position);
        triangle.setPosition(playerCar.position + playerCar.triangleOffset);
        circle1.setPosition(playerCar.position + playerCar.circle1Offset);
        circle2.setPosition(playerCar.position + playerCar.circle2Offset);

        window.clear(background);

        window.draw(circle1);
        window.draw(circle2);
        window.draw(rectangle);
        window.draw(triangle);

        window.display();
    }

    return 0;
}