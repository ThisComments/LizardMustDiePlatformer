#include <SFML/Graphics.hpp>

const sf::Vector2u WINDOW_SIZE{800, 600};

struct Player
{
    sf::Vector2f position;
    float speed;
    sf::RectangleShape rectangle;
    sf::CircleShape circle1;
    sf::CircleShape circle2;
    sf::ConvexShape triangle;
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

void playerInit(Player& player)
{
    player.position = {300.f, 250.f};
    player.speed = 300.f;

    player.triangle = CreateConvex
    (
        player.position + sf::Vector2f{100.f, 0.f}, 
        {
            {0.f, 50.f},
            {70.f, 50.f},
            {0.f, 0.f}
        },
        sf::Color::Red
    );

    player.rectangle = CreateRectangle
    (
        player.position, 
        {100.f, 50.f}, 
        sf::Color::Red
    );

    player.circle1 = CreateCircle
    (
        player.position + sf::Vector2f{0.f, 50.f}, 
        25.f, 
        sf::Color::Black
    );

    player.circle2 = CreateCircle
    (
        player.position + sf::Vector2f{100.f, 50.f}, 
        25.f, 
        sf::Color::Black
    );
}

void playerUpdate(Player& player, float dt)
{
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

    float directionLength = direction.length();

    if (directionLength > 0.f)
    {
        direction /= directionLength;
    }
    sf::Vector2f velocity = direction * player.speed * dt;

    player.position += velocity;

    sf::FloatRect boundsPlayer
    {
        player.position,
        {
            player.rectangle.getGlobalBounds().size.x + player.triangle.getGlobalBounds().size.x, 
            player.rectangle.getGlobalBounds().size.y + player.circle1.getGlobalBounds().size.y
        }
    };

    if (boundsPlayer.position.x <= 0.f)
    {
        player.position.x = 0.f;
    }
    if (boundsPlayer.position.y <= 0.f)
    {
        player.position.y = 0.f;
    }
    if (boundsPlayer.position.x + boundsPlayer.size.x >= WINDOW_SIZE.x)
    {
        player.position.x = WINDOW_SIZE.x - boundsPlayer.size.x;
    }
    if (boundsPlayer.position.y + boundsPlayer.size.y >= WINDOW_SIZE.y)
    {
        player.position.y = WINDOW_SIZE.y - boundsPlayer.size.y;
    }

    player.rectangle.setPosition(player.position);
    player.triangle.setPosition(player.position + sf::Vector2f{player.rectangle.getGlobalBounds().size.x, 0.f});
    player.circle1.setPosition(player.position + sf::Vector2f{0.f, player.rectangle.getGlobalBounds().size.y});
    player.circle2.setPosition(player.position + player.rectangle.getGlobalBounds().size);
}

void playerDraw(sf::RenderWindow& window, const Player& player)
{
    window.draw(player.circle1);
    window.draw(player.circle2);
    window.draw(player.rectangle);
    window.draw(player.triangle);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE), "Chizhov");
    sf::Color background = sf::Color::Green;

    Player player;
    playerInit(player);

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
        
        playerUpdate(player, dt);

        window.clear(background);

        playerDraw(window, player);

        window.display();
    }

    return 0;
}