#include "Order.h"
#include "Ship.h"
#include "Unit.h"

MoveOrder::MoveOrder(float x, float y)
{
	_Target.Set(x, y);
	_Type = "Move";
};

void MoveOrder::Execute(float dt, Entity* ent)
{
	float Speed = 100.f;
	Unit* u = (Unit*)ent;

	PairFloat delta(_Target._X - u->GetPosition()._X, _Target._Y - u->GetPosition()._Y);
	float heading = delta.Heading();
	PairFloat move(dt * Speed * (cos(DegreeToRad(heading - 90.f))), dt * Speed * (sin(DegreeToRad(heading - 90.f))));

	if (move.Size() >= delta.Size())
	{
		u->GetPosition().Set(_Target._X, _Target._Y);
		u->PopOrder();
	}
	else
		u->GetPosition().Set(u->GetPosition()._X + move._X, u->GetPosition()._Y + move._Y);

};

RotateToOrder::RotateToOrder(float data)
{
	_TargetAngle = data;
	_Type = "RotateTo";
};

void RotateToOrder::Execute(float dt, Entity* ent)
{
	Ship* ship = (Ship*)ent;

	float targetAngle = _TargetAngle;
	float deltaAngle = SmallestAngleBetweenHeadings(targetAngle, ship->GetHeading());

	if (ship->GetTurnRate() == 0.f)							//	Standing still. Start turning.
		ship->GetTurnRate() += ship->GetTurnThrust() * dt;
	else if (Sign(deltaAngle) != Sign(ship->GetTurnRate()))	//	Turning in the wrong direction. Decelerate.
		ship->GetTurnRate() -= ship->GetTurnThrust() * dt * Sign(ship->GetTurnRate());
	else											//	Turning in the right direction. Time to slow?
	{
		float timeToTarget = Abs(deltaAngle) / Abs(ship->GetTurnRate());
		float timeToStop = Abs(ship->GetTurnRate()) / ship->GetTurnThrust();

		if (timeToTarget > timeToStop)
			ship->GetTurnRate() += ship->GetTurnThrust() * dt * Sign(deltaAngle);		//	Accelerate
		else
		{
			if (Abs(ship->GetTurnRate()) <= ship->GetTurnThrust() * dt)
				ship->GetTurnRate() = 0.f;									//	Snap to 0
			else
				ship->GetTurnRate() -= ship->GetTurnThrust() * dt * Sign(deltaAngle);	//	Decelerate
		}
	}
	if ((Abs(deltaAngle) < 1.f) && (Abs(ship->GetTurnRate()) < 1.f))			//	Snap finished and dispose of the order.
	{
		ship->GetTurnRate() = 0.f;
		ship->GetHeading() = targetAngle;
		ship->PopOrder();
	}

};

RotateToStopOrder::RotateToStopOrder()
{
	_Type = "RotateToStop";
};

void RotateToStopOrder::Execute(float dt, Entity* ent)
{
	Ship* ship = (Ship*)ent;

	if (ship->GetTurnRate() != 0.f)
	{
		if (Abs(ship->GetTurnRate()) <= ship->GetTurnThrust() * dt)
			ship->GetTurnRate() = 0.f;
		else
			ship->GetTurnRate() -= ship->GetTurnThrust() * dt * Sign(ship->GetTurnRate());
	}
	if (ship->GetTurnRate() == 0.f)
		ship->PopOrder();
};

BurnDeltaVOrder::BurnDeltaVOrder(float dv)
{
	_DeltaV = Abs(dv);
	_Type = "BurnDeltaV";
};

void BurnDeltaVOrder::Execute(float dt, Entity* ent)
{
	Ship* ship = (Ship*)ent;

	if (_DeltaV > 0.f)
	{
		//	How much should we thrust by this frame if thrusting at max thrust?
		PairFloat thrust((ship->GetThrust() / ship->GetMass()) * dt * cos(DegreeToRad(ship->GetHeading() - 90.f)), (ship->GetThrust() / ship->GetMass()) * dt * sin(DegreeToRad(ship->GetHeading() - 90.f)));
		float dV = thrust.Size();
		//	If the order has that much dV expenditure remaining use it all in a normal burn
		if (dV <= _DeltaV)
		{
			ship->Thrust(dt);
			_DeltaV -= dV;
		}
		else	// Otherwise do a proportional burn and end the order
		{
			float proportion = _DeltaV / dV;	//	The proportion of a normal burn to carry out
			ship->Thrust(dt * proportion);
			ship->PopOrder();
		}
	}
	else
		ship->PopOrder();
};