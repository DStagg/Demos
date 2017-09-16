#ifndef FleetScene_H
#define FleetScene_H

#include <SFML\Graphics.hpp>
#include "Framework/Utility/Utility.h"
#include "Framework/Scene.h"
#include <iostream>

#include "FleetLevel.h"
#include "Unit.h"
#include "Order.h"

class FleetScene : public Scene
{
public:

	FleetScene(sf::RenderWindow* rw);
	~FleetScene();

	void Begin();
	void End();
	void Pause();
	void Resume();
	void Update(float dt);
	void DrawScreen();

private:

	sf::RenderWindow* _Window;
	FleetLevel* _Level;
	sf::View _View;
	float _ZoomLevel;

	float CalcZoomLevel();

	bool _Dragging = false;
	PairFloat _DragA = PairFloat();
	PairFloat _DragB = PairFloat();
};


void DrawCross(sf::RenderWindow* win, Unit* u, sf::Color col);

#endif