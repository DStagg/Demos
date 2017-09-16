#ifndef FLEETLEVEL_H
#define FLEETLEVEL_H

#include "Framework\Entities\Entity.h"
#include "Unit.h"
#include "Framework\IO\ImageManager.h"

#include "Level.h"

class FleetLevel : public Level
{
public:

	FleetLevel();

	void SetCommand(Unit* unit);
	Unit* GetCommand();
	Unit* GetUnit(int id);
	int CountUnits();
	void AddUnit(Unit* u);

	Unit* CreateUnit(std::string type, PairFloat pos);

	int CountSelected();
	void Select(Unit* u);
	void Deselect(Unit* u);
	Unit* GetSelected(int id);
	void ClearSelected();
	void ToggleSelect(Unit* u);

private:

	Unit* _CommandShip;
	std::vector<Unit*> _Units;
	std::vector<Unit*> _Selected;
	ImageManager _ImgMan;
};

#endif