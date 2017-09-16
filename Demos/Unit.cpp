#include "Unit.h"
#include "Order.h"

Unit::Unit(Level* lvl, float x, float y) : Entity(lvl)
{
	GetPosition().Set(x, y);
};

Unit::Unit() : Entity(0)
{

};

void Unit::Update(float dt)
{
	if ((int)_Orders.size() > 0)
		_Orders.front()->Execute(dt, this);
};

void Unit::Draw(sf::RenderWindow* rw)
{
	PairFloat pos = GetPosition();
	for (int i = 0; i < CountOrders(); i++)
	{
		if (GetOrder(i)->_Type == "Move")
		{
			PairFloat target = ((MoveOrder*)GetOrder(i))->_Target;
			PairFloat delta(target._X - pos._X, target._Y - pos._Y);
			
			sf::RectangleShape path;
			path.setPosition(pos._X, pos._Y);
			path.setSize(sf::Vector2f(delta.Size(), 3.f));
			path.setFillColor(sf::Color::Magenta);
			path.setRotation(delta.Heading()-90.f);
			rw->draw(path);
	
			pos = target;
		}
	}
};

void Unit::AddOrder(Order* order)
{
	_Orders.push_back(order);
};
void Unit::PopOrder()
{
	delete _Orders.front();
	_Orders.pop_front();
};
void Unit::ClearOrders()
{
	while (CountOrders() > 0)
		PopOrder();
};
int Unit::CountOrders()
{
	return (int)_Orders.size();
};
Order* Unit::GetOrder(int id)
{
	if (CountOrders() == 0) return 0;
	if (id >= CountOrders()) id = CountOrders() - 1;
	if (id < 0) id = 0;

	return _Orders[id];
};