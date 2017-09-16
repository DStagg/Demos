#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <algorithm>

#include "../Data/AABB.h"

#include "EntityComponents.h"

class Level;

class Entity
{
public:

	Entity(Level* lvl);
	~Entity();

	PairFloat& GetPosition();
	PairFloat& GetVelocity();
	PairFloat& GetSize();

	Graphic& GetGraphic();
	Stats& GetStats();

	void SetAlive(bool b);
	bool GetAlive();

	virtual void Update(float dt) = 0;
	virtual void Draw(sf::RenderWindow* rw) = 0;

	void SetLevel(Level* lvl);
	Level* GetLevel();
	
private:

	bool _Alive;

	PairFloat	_Position;
	PairFloat	_Velocity;
	PairFloat	_Size;

	Stats _Stats;
	Graphic _Graphic;
	
	Level* _Level;
};

class BasicEnt : public Entity
{
public:

	BasicEnt(Level* lvl);

	void Update(float dt);
	void Draw(sf::RenderWindow* rw);

};

class SFXEnt : public BasicEnt
{
public:

	SFXEnt(Level* lvl);

	void Update(float dt);

private:

};

AABB GenBoundBox(Entity* ent);

class EntList
{
public:

	EntList();
	~EntList();

	void AddEnt(Entity* ent);
	void DelEnt(Entity* ent);
	void Cull(int limit);

	int CountEnts();
	Entity* GetEnt(int i);

private:

	std::vector<Entity*> _Entities;
};

void DebugDrawEntity(Entity* ent, sf::RenderWindow* target, sf::Color col = sf::Color::Red);

#endif