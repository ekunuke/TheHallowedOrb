#include "Game.h"

int main()
{
    srand(time(0));
    int seed = rand();
    srand(seed);
    sf::RenderWindow window(sf::VideoMode(800, 600), "The Hallowed Orb");
    window.setFramerateLimit(60);

    window.setKeyRepeatEnabled(false);
    ResourceManager::initialize();
    sf::Image image = ResourceManager::getTexture("icon.png").copyToImage();
    window.setIcon(32, 32, image.getPixelsPtr());
    bool focus = true;
    int frame = 0;

    Game game;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::LostFocus)
            {
                focus = false;
            }
            else if(event.type == sf::Event::GainedFocus)
            {
                focus = true;
            }
            else if(event.type == sf::Event::Resized)
            {
                float newRatio = event.size.width / event.size.height;
                float newWidth = event.size.width;
                float newHeight = event.size.height;
                if(newRatio < (4.f/3.f))
                {
                    newWidth = event.size.height * (4.f/3.f);
                }
                else if(newRatio > (4.f/3.f))
                {
                    newHeight = event.size.width * (3.f/4.f);
                }
                window.setSize({newWidth, newHeight});
            }
        }
        if(focus)
        {
            game.step();
        }
        window.clear(sf::Color(222, 238, 214));
        game.draw(window);
        window.display();
        frame++;
    }
    ResourceManager::cleanUp();
    return 0;
}
