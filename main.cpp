#include "Sprite.h"
#include "Planet.h"
#include "Util.h"

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include <iostream>
#include <set>

int main()
{
	srand(64);

	sf::RenderWindow window(sf::VideoMode(600, 600, 32), "Space Bitch!");
	window.setFramerateLimit(30);

	Planet planet;
	sf::Clock clock;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	int nFrames = 0;
	float tPeriod = 0;

	while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
				if (event.key.code == sf::Keyboard::Escape)
			       window.close();
				else 
					planet.OnKeyPressed(event.key.code);
        }

	    float tDelta = clock.restart().asSeconds();
		tPeriod += tDelta;
		++nFrames;
		if (tPeriod >= 1)
		{
			std::cout << int(nFrames / tPeriod) << " fps" << std::endl;
			nFrames = 0;
			tPeriod = 0;
		}

		//sprite.Update(tDelta.asSeconds());
		planet.Update(tDelta);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto sz = window.getSize();
		glViewport(0, 0, sz.x, sz.y);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30, sz.x/(float)sz.y, 1, 1000 );
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0, -420, -500);
		glRotatef(30, 1, 0, 0);

		planet.Draw(window);

		window.display();
    }

    return EXIT_SUCCESS;
}
