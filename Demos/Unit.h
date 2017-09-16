#ifndef UNIT_H
#define UNIT_H

#include "Framework\Entities\Entity.h"
#include <deque>

class Order;

class Unit : public Entity
{
public:

	Unit();
	Unit(Level* lvl, float x, float y);

	void Update(float dt);
	void Draw(sf::RenderWindow* rw);

	void AddOrder(Order* order);
	void PopOrder();
	void ClearOrders();
	int CountOrders();
	Order* GetOrder(int id);

protected:

	std::deque<Order*> _Orders;


private:

	

};

#endif