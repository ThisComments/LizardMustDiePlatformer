#include <SFML/Graphics.hpp>

struct Point
{
    float x;
    float y;
};

struct Size
{
    float width;
    float height;
};

sf::RectangleShape CreateRectangle(
    const Point point, 
    const Size size, 
    const sf::Color color
)
{
    sf::RectangleShape rectangle(sf::Vector2f(size.width, size.height));
    rectangle.setFillColor(color);
    rectangle.setPosition({point.x, point.y});

    return rectangle;
}

sf::CircleShape CreateCircle(
    const Point point, 
    const float radius, 
    const sf::Color color
)
{
    sf::CircleShape circle(radius);
    circle.setFillColor(color);
    circle.setPosition({point.x, point.y});

    return circle;
}

sf::ConvexShape CreateConvex(
    const Point point, 
    const sf::Color color,
    const std::vector<Point>& points
)
{
    sf::ConvexShape shape;

    shape.setFillColor(color);
    shape.setPosition({point.x, point.y});
    shape.setPointCount(points.size());

    for (size_t i = 0; i < points.size(); i++)
    {
        shape.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
    }

    return shape;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Chizhov");
    sf::Color background = sf::Color::Green;
    sf::RectangleShape rectangle = CreateRectangle({300.f, 250.f}, {100.f, 50.f}, sf::Color::Red);
    sf::CircleShape circle1 = CreateCircle({300.f, 290.f}, 25.f, sf::Color::Black);
    sf::CircleShape circle2 = CreateCircle({400.f, 290.f}, 25.f, sf::Color::Black);

    std::vector<Point> points =
    {
        {0.f, 50.f},
        {70.f, 50.f},
        {0.f, 0.f}
    };

    sf::ConvexShape triangle = CreateConvex({400.f, 250.f}, sf::Color::Red, points);

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

        window.draw(circle1);
        window.draw(circle2);
        window.draw(rectangle);
        window.draw(triangle);

        window.display();
    }

    return 0;
}