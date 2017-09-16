#include "MainScene.h"

MainScene::MainScene(sf::RenderWindow* rw)
{
	_Window = rw;
};
MainScene::~MainScene()
{

};

void MainScene::Begin()
{
	sf::Font font;
	font.loadFromFile("Roboto-Regular.ttf");
	
	_MenuList.Populate({ "Space" , "Fleet" , "Quit" });
	_MenuList.SetLoop(false);
	_MenuList.SetBuffers(25.f, 35.f);
	_MenuList.Format(font, 0.f, 0.f, SFMLMenuList::Orientation::Left);

};
void MainScene::End()
{
	
};
void MainScene::Pause()
{

};
void MainScene::Resume()
{

};
void MainScene::Update(float dt)
{
	sf::Event Event;
	while (_Window->pollEvent(Event))
	{
		if (Event.type == sf::Event::Closed)
			SetRunning(false);
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
			SetRunning(false);
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
			_MenuList.DecChoice();
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
			_MenuList.IncChoice();
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Return))
		{
			if (_MenuList.GetChoice() == 0)
				GetManager()->PushScene(new SpaceScene(_Window));
			else if (_MenuList.GetChoice() == 1)
				GetManager()->PushScene(new FleetScene(_Window));
			else
				GetManager()->Quit();
		}
		
	}
};
void MainScene::DrawScreen()
{
	_MenuList.Draw(_Window);
	
};