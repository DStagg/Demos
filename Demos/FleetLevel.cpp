#include "FleetLevel.h"

FleetLevel::FleetLevel()
{
	_ImgMan.LoadTextureFromFile("CommandShip", "CommandShip.png");
	_ImgMan.LoadTextureFromFile("FleetShip", "FleetShip.png");
};

void FleetLevel::SetCommand(Unit* unit)
{
	_CommandShip = unit;
};
Unit* FleetLevel::GetCommand()
{
	return _CommandShip;
};
Unit* FleetLevel::GetUnit(int id)
{
	if (id >= CountUnits()) id = CountUnits() - 1;
	if (id < 0) id = 0;

	return _Units[id];
};
int FleetLevel::CountUnits()
{
	return (int)_Units.size();
};
void FleetLevel::AddUnit(Unit* u)
{
	_Units.push_back(u);
};
Unit* FleetLevel::CreateUnit(std::string type, PairFloat pos)
{
	Unit* temp;
	temp = new Unit(this, pos._X, pos._Y);
	temp->GetSize().Set(11, 11);
	if (type == "Command")
		temp->GetGraphic().SetSprite(sf::Sprite(*_ImgMan.GetTexturePntr("CommandShip")));
	else
		temp->GetGraphic().SetSprite(sf::Sprite(*_ImgMan.GetTexturePntr("FleetShip")));
	return temp;
};

int FleetLevel::CountSelected()
{
	return (int)_Selected.size();
};
void FleetLevel::Select(Unit* u)
{
	for (int i = 0; i < CountSelected(); i++)
		if (GetSelected(i) == u)
			return;
	_Selected.push_back(u);
};
void FleetLevel::Deselect(Unit* u)
{
	for (int i = 0; i < CountSelected(); i++)
		if (GetSelected(i) == u)
			_Selected.erase(_Selected.begin() + i);
}
Unit* FleetLevel::GetSelected(int id)
{
	if (CountSelected() == 0) return 0;
	if (id >= CountSelected()) id = CountSelected() - 1;
	if (id < 0) id = 0;

	return _Selected[id];
};
void FleetLevel::ClearSelected()
{
	while (CountSelected() > 0)
		_Selected.pop_back();
};
void FleetLevel::ToggleSelect(Unit* u)
{
	for (int i = 0; i < CountSelected(); i++)
		if (GetSelected(i) == u)
		{
			_Selected.erase(_Selected.begin() + i);
			return;
		}
	Select(u);
};