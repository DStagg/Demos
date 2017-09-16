#ifndef SHIP_H
#define SHIP_H

#include "Framework\Entities\Entity.h"
#include "Level.h"
#include <queue>
#include "Framework\Utility\Utility.h"
#include "Order.h"

void ClampHeading(float& angle);
float SmallestAngleBetweenHeadings(float targetangle, float startangle);

class Ship : public Entity
{
public:

	Ship(Level* lvl = 0);

	void Update(float dt);
	void Draw(sf::RenderWindow* rw);

	void AddOrder(Order* order);
	void PopOrder();

	void Thrust(float dt);
	
	float& GetTurnRate();
	float& GetTurnThrust();
	float& GetThrust();
	float& GetMass();
	float& GetHeading();
	
protected:

	std::queue<Order*> _Orders;

	float _Heading;						//	The heading that the ship is pointing in. North = 0, East = 90, S = 180, etc.
	float _TurnRate, _TurnThrust;		//	A turning ship will accelerate by _TurnThrust degrees per second per second. 
										//		_TurnRate is negative for counter-clockwise and positive for clockwise.
	float _Hull, _MaxHull;				//	Hull is the base unit of health and does not regenerate automatically
	float _Shield, _MaxShield;			//	Shield is the first unit of health to be depleted and regenerates by _RegenRate units
	float _RegenRate, _RegenDelay;		//		per second after _RegenDelay seconds of delay since damage was last taken	
	float _Fuel, _MaxFuel, _FuelRate;	//	Every ship burns _FuelRate units of fuel every 1 second 
	float _Mass, _Thrust;				//	Every unit of thrust accelerates 1 unit of mass by 1 unit of velocity per second ^ 2

};

#endif