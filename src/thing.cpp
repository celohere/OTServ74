#include "otpch.h"

#include "creature.h"
#include "cylinder.h"
#include "item.h"
#include "player.h"
#include "thing.h"
#include "tile.h"
#include "log.h"




const Position& Thing::getPosition() const
{
	const Tile* tile = getTile();
	if (tile) {
		return tile->getTilePosition();
	} else {
		LOG_DEBUG_MOVESYS("NULL TILE");
		return Tile::null_tile.getTilePosition();
	}
}



std::string Thing::getXRayDescription() const
{
	if (isRemoved())
		return "Thing you looked at seems to be removed.";

	const auto& pos = getPosition();
	std::stringstream ret;
	ret << "Position: [" << pos.x << ", " << pos.y << ", " << pos.z << "]";
	return ret.str();
}




bool Thing::isRemoved() const
{
	if (parent == nullptr)
		return true;
	else if(getParent()->isRemoved())
		return true;
	
	return false;
}



const Tile* Thing::getTile() const
{
	auto cylinder = getTopParent();
	if (!cylinder) {
		LOG_DEBUG_MOVESYS("NULL TILE");
		return &Tile::null_tile;
	}

	const auto root_cylinder = cylinder->getParent();
	if (root_cylinder)
		cylinder = root_cylinder;

	return dynamic_cast<const Tile*>(cylinder);
}



const Cylinder* Thing::getTopParent() const
{
	// tile
	if (getParent() == nullptr)
		return dynamic_cast<const Cylinder*>(this);

	const Cylinder* aux = getParent();
	const Cylinder* prevaux = dynamic_cast<const Cylinder*>(this);

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<const Cylinder*>(prevaux))
		return prevaux;

	return aux;
}




Tile* Thing::getTile()
{
	auto cylinder = getTopParent();

	if (!cylinder) {
		LOG_DEBUG_MOVESYS("NULL TILE");
		return &Tile::null_tile;
	}

	const auto root_cylinder = cylinder->getParent();
	if (root_cylinder)
		cylinder = root_cylinder;

	return dynamic_cast<Tile*>(cylinder);
}




Cylinder* Thing::getTopParent()
{
	// tile
	if (getParent() == nullptr) 
		return dynamic_cast<Cylinder*>(this);

	Cylinder* aux = getParent();
	Cylinder* prevaux = dynamic_cast<Cylinder*>(this);

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<Cylinder*>(prevaux))
		return prevaux;

	return aux;
}






