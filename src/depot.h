//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////

#ifndef __OTSERV_DEPOT_H__
#define __OTSERV_DEPOT_H__

#include "container.h"

class Depot : public Container
{
public:
	Depot(uint16_t _type);
	~Depot() override;

	Depot *getDepot() override
	{
		return this;
	}
	const Depot *getDepot() const override
	{
		return this;
	}

	// serialization
	Attr_ReadValue readAttr(AttrTypes_t attr, PropStream &propStream) override;

	uint32_t getDepotId() const
	{
		return depotId;
	}
	void setMaxDepotLimit(uint32_t maxitems)
	{
		maxDepotLimit = maxitems;
	}
	void setDepotId(uint32_t id)
	{
		depotId = id;
	}

	// cylinder implementations
	ReturnValue __queryAdd(int32_t index, const Thing *thing, uint32_t count, uint32_t flags) const override;

	ReturnValue
	__queryMaxCount(int32_t index, const Thing *thing, uint32_t count, uint32_t &maxQueryCount, uint32_t flags) const override;

	void
	postAddNotification(Thing *thing, const Cylinder *oldParent, int32_t index, cylinderlink_t link = LINK_OWNER) override;
	void postRemoveNotification(Thing *thing,
	                                    const Cylinder *newParent,
	                                    int32_t index,
	                                    bool isCompleteRemoval,
	                                    cylinderlink_t link = LINK_OWNER) override;

	// overrides
	bool canRemove() const override
	{
		return false;
	}

private:
	uint32_t maxDepotLimit;
	uint32_t depotId;
};

#endif
