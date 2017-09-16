#ifndef SPACESCENE_H
#define SPACESCENE_H

#include <SFML\Graphics.hpp>
#include "Framework/Utility/Utility.h"
#include "Framework/Scene.h"
#include <iostream>

#include "Ship.h"
#include "Level.h"

class SpaceScene : public Scene
{
public:

	SpaceScene(sf::RenderWindow* rw);
	~SpaceScene();

	void Begin();
	void End();
	void Pause();
	void Resume();
	void Update(float dt);
	void DrawScreen();

private:

	sf::RenderWindow* _Window;

	Level _Level;
	Ship _Ship;

};

#endif