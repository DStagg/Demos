#include "FleetScene.h"

FleetScene::FleetScene(sf::RenderWindow* rw)
{
	_Window = rw;
};
FleetScene::~FleetScene()
{

};

void FleetScene::Begin()
{
	_Level = new FleetLevel();
	_Level->SetCommand(_Level->CreateUnit("Command", PairFloat(100.f, 100.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(200.f, 150.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(215.f, 150.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(230.f, 150.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(208.f, 165.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(222.f, 165.f)));
	_Level->AddUnit(_Level->CreateUnit("Fleet", PairFloat(215.f, 180.f)));
	
	_View = _Window->getDefaultView();	
};
void FleetScene::End()
{
	
};
void FleetScene::Pause()
{

};
void FleetScene::Resume()
{

};
void FleetScene::Update(float dt)
{
	sf::Event Event;
	while (_Window->pollEvent(Event))
	{
		if (Event.type == sf::Event::Closed)
			SetRunning(false);
		else if ((Event.type == sf::Event::MouseButtonPressed) && (Event.mouseButton.button == sf::Mouse::Left))
		{
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
				_Level->ClearSelected();
			
			if (GenBoundBox(_Level->GetCommand()).Contains(sf::Mouse::getPosition(*_Window).x, sf::Mouse::getPosition(*_Window).y))
				_Level->ToggleSelect(_Level->GetCommand());
			else
			{
				for (int i = 0; i < _Level->CountUnits(); i++)
					if (GenBoundBox(_Level->GetUnit(i)).Contains(sf::Mouse::getPosition(*_Window).x, sf::Mouse::getPosition(*_Window).y))
					{
						_Level->ToggleSelect(_Level->GetUnit(i));
						break;
					}
			}
			if ((_Level->CountSelected() == 0) && (!_Dragging))
			{
				_Dragging = true;
				_DragA.Set(sf::Mouse::getPosition(*_Window).x, sf::Mouse::getPosition(*_Window).y);
			}
		}
		else if ((Event.type == sf::Event::MouseButtonReleased) && (Event.mouseButton.button == sf::Mouse::Left))
			_Dragging = false;
		else if ((Event.type == sf::Event::MouseButtonPressed) && (Event.mouseButton.button == sf::Mouse::Right))
		{
			if (_Level->CountSelected() > 0)
			{
				for (int i = 0; i < _Level->CountSelected(); i++)
				{
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
						_Level->GetSelected(i)->ClearOrders();
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
					{
						float xTotal = 0.f;
						float yTotal = 0.f;
						for (int i = 0; i < _Level->CountSelected(); i++)
						{
							xTotal += _Level->GetSelected(i)->GetPosition()._X;
							yTotal += _Level->GetSelected(i)->GetPosition()._Y;
						}
						PairFloat groupPos(xTotal / _Level->CountSelected(), yTotal / _Level->CountSelected());
						PairFloat diff(_Level->GetSelected(i)->GetPosition()._X - groupPos._X, _Level->GetSelected(i)->GetPosition()._Y - groupPos._Y);
						_Level->GetSelected(i)->AddOrder(new MoveOrder(sf::Mouse::getPosition(*_Window).x + diff._X , sf::Mouse::getPosition(*_Window).y + diff._Y));
					}
					else
						_Level->GetSelected(i)->AddOrder(new MoveOrder(sf::Mouse::getPosition(*_Window).x, sf::Mouse::getPosition(*_Window).y));
				}
			}
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
			SetRunning(false);
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Up))
		{
			_View.zoom(0.5f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Down))
		{
			_View.zoom(2.f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::W))
		{
			_View.move(0.f, -50.f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::S))
		{
			_View.move(0.f, 50.f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A))
		{
			_View.move(-50.f, 0.f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::D))
		{
			_View.move(50.f, 0.f);
			_Window->setView(_View);
		}
		else if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Space))
			std::cout << "Zoom Level: " << CalcZoomLevel() << std::endl;
	}

	if (_Dragging)
	{
		_Level->ClearSelected();

		_DragB.Set(sf::Mouse::getPosition(*_Window).x, sf::Mouse::getPosition(*_Window).y);

		PairFloat origin(Min(_DragA._X, _DragB._X), Min(_DragA._Y, _DragB._Y));
		PairFloat size(Abs(_DragB._X - _DragA._X), Abs(_DragB._Y - _DragA._Y));
		AABB box(origin._X, origin._Y, size._X, size._Y);

		if (box.Contains(GenBoundBox(_Level->GetCommand())))
			_Level->Select(_Level->GetCommand());

		for (int i = 0; i < _Level->CountUnits(); i++)
			if (box.Contains(GenBoundBox(_Level->GetUnit(i))))
			{
				_Level->Select(_Level->GetUnit(i));
			}
	}

	_Level->GetCommand()->Update(dt);
	for (int i = 0; i < _Level->CountUnits(); i++)
		_Level->GetUnit(i)->Update(dt);
};
void FleetScene::DrawScreen()
{
	_Level->GetCommand()->Draw(_Window);
	for (int i = 0; i < _Level->CountUnits(); i++)
		_Level->GetUnit(i)->Draw(_Window);

	DrawCross(_Window, _Level->GetCommand(), sf::Color::Blue);
	for (int i = 0; i < _Level->CountUnits(); i++)
		DrawCross(_Window, _Level->GetUnit(i), sf::Color::Green);

	for (int i = 0; i < _Level->CountSelected(); i++)
	{
		sf::RectangleShape rect;
		rect.setPosition(_Level->GetSelected(i)->GetPosition()._X - 1.f, _Level->GetSelected(i)->GetPosition()._Y - 1.f);
		rect.setSize(sf::Vector2f(_Level->GetSelected(i)->GetSize()._X + 2.f, _Level->GetSelected(i)->GetSize()._Y + 2.f));
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineColor(sf::Color::Cyan);
		rect.setOutlineThickness(1.f);
		_Window->draw(rect);
	}

	if (_Dragging)
	{
		PairFloat origin(Min(_DragA._X, _DragB._X), Min(_DragA._Y, _DragB._Y));
		PairFloat size(Abs(_DragB._X - _DragA._X), Abs(_DragB._Y - _DragA._Y));

		sf::RectangleShape rect;
		rect.setPosition(origin._X , origin._Y);
		rect.setSize(sf::Vector2f(size._X, size._Y));
		rect.setFillColor(sf::Color(0, 0, 0, 0));
		rect.setOutlineColor(sf::Color::Cyan);
		rect.setOutlineThickness(1.f);
		_Window->draw(rect);
	}
};

void DrawCross(sf::RenderWindow* win, Unit* u, sf::Color color)
{
	sf::RectangleShape rect;
	rect.setPosition(u->GetPosition()._X + (u->GetSize()._X / 2.f) - 1.f, u->GetPosition()._Y);
	rect.setSize(sf::Vector2f(3.f, u->GetSize()._Y));
	rect.setFillColor(color);
	win->draw(rect);

	rect.setPosition(u->GetPosition()._X, u->GetPosition()._Y + (u->GetSize()._Y / 2.f) - 1.f);
	rect.setSize(sf::Vector2f(u->GetSize()._X, 3.f));
	win->draw(rect);
};

float FleetScene::CalcZoomLevel()
{
	return _Window->getDefaultView().getSize().x / _View.getSize().x;

};