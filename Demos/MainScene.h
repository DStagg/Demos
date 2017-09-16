#ifndef MainScene_H
#define MainScene_H

#include <SFML\Graphics.hpp>
#include "Framework/Utility/Utility.h"
#include "Framework/Scene.h"
#include "Framework\IO\MenuList.h"

#include "SpaceScene.h"
#include "FleetScene.h"

class MainScene : public Scene
{
public:

	MainScene(sf::RenderWindow* rw);
	~MainScene();

	void Begin();
	void End();
	void Pause();
	void Resume();
	void Update(float dt);
	void DrawScreen();

private:

	sf::RenderWindow* _Window;

	SFMLMenuList _MenuList;

};

#endif