#include "Ship.h"

void ClampHeading(float& angle)
{
	while (angle >= 360.f) angle -= 360.f;
	while (angle < 0.f) angle += 360.f;
};

float SmallestAngleBetweenHeadings(float a1, float a2)
{
	return (float)(((((int)a1 - (int)a2) % 360) + 360 + 180) % 360 - 180);
};

////////////////////

Ship::Ship(Level* lvl) : Entity(lvl)
{
	_Heading = 0.f;
	_TurnRate = 0.f;
	_TurnThrust = 36.f;
	_Hull = 100.f;
	_MaxHull = 100.f;
	_Shield = 100.f;
	_MaxShield = 100.f;
	_RegenRate = 20.f;
	_RegenDelay = 5.f;
	_Fuel = 100.f;
	_MaxFuel = 100.f;
	_FuelRate = 1.f;
	_Mass = 1.f;
	_Thrust = 100.f;
};

void Ship::AddOrder(Order* order)
{
	_Orders.push(order);
};
void Ship::PopOrder()
{
	delete _Orders.front();
	_Orders.pop();
};

float& Ship::GetTurnRate()
{
	return _TurnRate;
};
float& Ship::GetTurnThrust()
{
	return _TurnThrust;
};
float& Ship::GetThrust()
{
	return _Thrust;
};
float& Ship::GetMass()
{
	return _Mass;
};
float& Ship::GetHeading()
{
	return _Heading;
};

void Ship::Update(float dt)
{
	//	Execute Orders
	if ((int)_Orders.size() != 0)
		_Orders.front()->Execute(dt, this);
	
	//	Turning Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		_TurnRate -= _TurnThrust * dt;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		_TurnRate += _TurnThrust * dt;
			
	//	Turning Update
	_Heading += _TurnRate * dt;
	ClampHeading(_Heading);
	
	//	Thrust Input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		Thrust(dt);
	
	//	Thrust Update
	GetPosition().Set(GetPosition()._X + (GetVelocity()._X * dt), GetPosition()._Y + (GetVelocity()._Y * dt));
		
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::F)) && ((int)_Orders.size() == 0))
	{
		float angle;
		if (GetVelocity().Size() == 0.f)
			angle = _Heading - 180.f;
		else
			angle = GetVelocity().Heading() - 180.f;
		ClampHeading(angle);
		AddOrder(new RotateToOrder(angle));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		AddOrder(new RotateToOrder(_Heading));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		AddOrder(new RotateToStopOrder());
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		if (GetVelocity().Size() != 0.f)
			AddOrder(new RotateToOrder(GetVelocity().Heading()));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		if (GetVelocity().Size() != 0.f)
			AddOrder(new RotateToOrder(GetVelocity().Heading()-180.f));
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::G)) && ((int)_Orders.size() == 0 ))
	{
		float angle;
		if (GetVelocity().Size() == 0.f)
			angle = _Heading - 180.f;
		else
			angle = GetVelocity().Heading() - 180.f;
		ClampHeading(angle);
		AddOrder(new RotateToOrder(angle));
		AddOrder(new BurnDeltaVOrder(GetVelocity().Size()));
	}
	else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::H)) && ((int)_Orders.size() == 0))
	{
		float angle;
		if (GetVelocity().Size() == 0.f)
			angle = _Heading - 180.f;
		else
			angle = GetVelocity().Heading() - 180.f;
		ClampHeading(angle);
		AddOrder(new RotateToOrder(angle));
		AddOrder(new BurnDeltaVOrder(GetVelocity().Size()*2.f));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
	{
		_Heading = 90.f;
		GetVelocity().Set(0.f, 0.f);
		GetPosition().Set(250.f, 250.f);
	}
	//	Debug text
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::cout << "Pos: (" << GetPosition()._X << "," << GetPosition()._Y << ") | Vel: (" << GetVelocity()._X << "," << GetVelocity()._Y << ")" << std::endl;
		std::cout << "Heading: " << _Heading << " | Turn Rate: " << _TurnRate << std::endl;
	}
};

void Ship::Draw(sf::RenderWindow* rw)
{
	sf::RectangleShape fuelBar;
	fuelBar.setPosition(0.f, rw->getSize().y - (rw->getSize().y * (_Fuel / _MaxFuel)));
	fuelBar.setSize(sf::Vector2f(25.f, rw->getSize().y * (_Fuel / _MaxFuel)));
	fuelBar.setFillColor(sf::Color::Green);
	rw->draw(fuelBar);

	sf::RectangleShape rect;
	rect.setPosition(GetPosition()._X, GetPosition()._Y);
	rect.setSize(sf::Vector2f(GetSize()._X, GetSize()._Y));
	rect.setOrigin(25.f, 50.f);
	rect.setRotation(_Heading);
	rect.setFillColor(sf::Color::Blue);
	rw->draw(rect);

	sf::Text debugText1, debugText2;
	sf::Font font;
	font.loadFromFile("Roboto-Regular.ttf");
	debugText1.setFont(font);
	debugText1.setPosition(0.f, rw->getSize().y - 100.f);
	debugText1.setString("Heading: " + FloatToString(_Heading) + " Heading from Vel: " + FloatToString(CalcHeading(0.f,0.f,GetVelocity()._X, GetVelocity()._Y)));
	rw->draw(debugText1);

	float targetAngle = atan2(GetVelocity()._X, GetVelocity()._Y);
	while (targetAngle >= 360.f) targetAngle -= 360.f;
	while (targetAngle < 0.f) targetAngle += 360.f;
	debugText2.setFont(font);
	debugText2.setPosition(0.f, rw->getSize().y - 50.f);
	debugText2.setString("Anti-Heading: " + FloatToString(_Heading - 180.f) +" Anti-Heading from Vel: " + FloatToString(CalcHeading(0.f,0.f,GetVelocity()._X, GetVelocity()._Y)-180.f));
	rw->draw(debugText2);

	sf::Text debugText3;
	debugText3.setFont(font);
	debugText3.setPosition(0.f, rw->getSize().y - 150.f);
	debugText3.setString("Debug Text 3");
	rw->draw(debugText3);
	
	sf::Text debugText4;
	debugText4.setFont(font);
	debugText4.setPosition(0.f, 0.f);
	debugText4.setString("[" + FloatToString(GetVelocity()._X) + "," + FloatToString(GetVelocity()._Y) + "]");
	rw->draw(debugText4);
	
	
	
};

void Ship::Thrust(float dt)
{
	float incAccelLimit = 0.001f;
	float incVelLimit = 0.01f;

	if (_Fuel <= 0)
		return;

	float dX = (_Thrust / _Mass) * dt * (cos(DegreeToRad(_Heading-90.f)));
	if (Abs(dX) < incAccelLimit) dX = 0.f;
	float dY = (_Thrust / _Mass) * dt * (sin(DegreeToRad(_Heading-90.f)));
	if (Abs(dY) < incAccelLimit) dY = 0.f;
	
	//	Accelerate
	GetVelocity()._X += dX;
	GetVelocity()._Y += dY;

	if (Abs(GetVelocity()._X) < incVelLimit) GetVelocity()._X = 0.f;
	if (Abs(GetVelocity()._Y) < incVelLimit) GetVelocity()._Y = 0.f;

	//	Burn Fuel
	_Fuel -= _FuelRate * dt;

	//////////////////////////////////////

};