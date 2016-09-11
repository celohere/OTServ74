#include "otpch.h"

#include <string>

#include "creature.h"
#include "cylinder.h"
#include "item.h"
#include "player.h"
#include "tile.h"
#include "Log.h"
#include "thing.h"


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
	if (isRemoved()) {
		return "Thing you looked at seems to be removed.";
	}
	
	const auto to_str = [](int n) { return std::to_string(n); };
	const auto& pos = getPosition();
	return "Position: [" + to_str(pos.x) + ", " + to_str(pos.y) + ", " + to_str(pos.z) + "]";
}


bool Thing::isRemoved() const
{
	if (parent == nullptr) {
		return true;
	} else if (getParent()->isRemoved()) {
		return true;
	}

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
	if (root_cylinder) {
		cylinder = root_cylinder;
	}

	return dynamic_cast<const Tile*>(cylinder);
}


const Cylinder* Thing::getTopParent() const
{
	// tile
	if (getParent() == nullptr) {
		return dynamic_cast<const Cylinder*>(this);
	}

	const Cylinder* aux = getParent();
	const Cylinder* prevaux = dynamic_cast<const Cylinder*>(this);

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<const Cylinder*>(prevaux)) {
		return prevaux;
	}

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
	if (root_cylinder) {
		cylinder = root_cylinder;
	}

	return dynamic_cast<Tile*>(cylinder);
}


Cylinder* Thing::getTopParent()
{
	// tile
	if (getParent() == nullptr) {
		return dynamic_cast<Cylinder*>(this);
	}

	Cylinder* aux = getParent();
	Cylinder* prevaux = dynamic_cast<Cylinder*>(this);

	while (aux->getParent() != nullptr) {
		prevaux = aux;
		aux = aux->getParent();
	}

	if (dynamic_cast<Cylinder*>(prevaux)) {
		return prevaux;
	}

	return aux;
}
