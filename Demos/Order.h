#ifndef ORDER_H
#define ORDER_H

#include "Framework\Utility\Utility.h"
#include "Framework\Entities\Entity.h"

class Order
{
public:
	virtual void Execute(float dt, Entity* ent) = 0;

	std::string _Type;
};

class MoveOrder : public Order
{
public:

	MoveOrder(float x, float y);
	void Execute(float dt, Entity* ent);

	PairFloat _Target;
};

class RotateToOrder : public Order
{
public:

	RotateToOrder(float data);
	void Execute(float dt, Entity* ship);

	float _TargetAngle;
};

class RotateToStopOrder : public Order
{
public:

	RotateToStopOrder();
	void Execute(float dt, Entity* ship);
};

class BurnDeltaVOrder : public Order
{
public:

	BurnDeltaVOrder(float dv);
	void Execute(float dt, Entity* ship);

	float _DeltaV;
};

#endif