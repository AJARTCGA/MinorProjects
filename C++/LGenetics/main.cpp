#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FULLSCREEN false



int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "LGenetics", (FULLSCREEN) ? sf::Style::Fullscreen : 0);
    bool paused = false;
    window.setKeyRepeatEnabled(false);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed){
            	if(event.key.code == sf::Keyboard::Space)
                    paused = !paused;
            }
        }
        window.clear();
        window.display();
    }
    std::cout << "Goodbye!" << std::endl;
    return 0;
}

