#include "SpaceScene.h"

SpaceScene::SpaceScene(sf::RenderWindow* rw)
{
	_Window = rw;
};
SpaceScene::~SpaceScene()
{

};

void SpaceScene::Begin()
{
	_Ship = Ship(&_Level);
	_Ship.GetPosition().Set(250.f, 250.f);
	_Ship.GetSize().Set(50.f, 100.f);

};
void SpaceScene::End()
{
	
};
void SpaceScene::Pause()
{

};
void SpaceScene::Resume()
{

};
void SpaceScene::Update(float dt)
{
	sf::Event Event;
	while (_Window->pollEvent(Event))
	{
		if (Event.type == sf::Event::Closed)
			SetRunning(false);
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
			SetRunning(false);
		
	}

	_Ship.Update(dt);
};
void SpaceScene::DrawScreen()
{
	_Ship.Draw(_Window);

	DebugDrawEntity(&_Ship, _Window);
	
};