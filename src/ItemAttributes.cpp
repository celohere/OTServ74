#include "ItemAttributes.h"
#include "Assert.h"
#include <algorithm>

static std::string s_emptyString;


bool ItemAttributes::hasAttribute(ItemAttrType type) const
{
	return (m_attrFlags & type) != 0;
}


const std::string& ItemAttributes::getStrAttr(ItemAttrType type) const
{
	if (!(m_attrFlags & type)) {
		return s_emptyString;
	}

	auto attr = getAttr(type);
	return attr ? attr->str : s_emptyString;
}


uint32_t ItemAttributes::getIntAttr(ItemAttrType type) const
{
	assert(validateIntAttrType(type));

	const ItemAttribute* const attr = getAttr(type);
	return attr != nullptr ? attr->integer : 0;
}


const ItemAttribute* ItemAttributes::getAttr(ItemAttrType type) const
{
	const auto itr = std::find_if(m_attributes.begin(), m_attributes.end(),
	                              [=](const ItemAttribute& attr) { return attr.type == type; });

	return itr != m_attributes.end() ? &(*itr) : nullptr;
}


void ItemAttributes::setStrAttr(ItemAttrType type, const std::string& value)
{
	assert(validateStrAttrType(type) && value.length() > 0);

	if (m_attrFlags & type) {
		auto itr = std::find_if(m_attributes.begin(), m_attributes.end(),
		                        [=](const ItemAttribute& attr) { return attr.type == type; });

		if (itr != m_attributes.end()) {
			itr->str = value;
		} else {
			ItemAttribute newAttr = type;
			newAttr.str = value;
			addAttr(newAttr);
		}
	}
}


void ItemAttributes::removeAttribute(ItemAttrType type)
{
	if (m_attrFlags & type) {
		auto itr =
		std::remove_if(m_attributes.begin(), m_attributes.end(),
		               [=](const ItemAttribute& attr) { return attr.type == type; });

		m_attributes.erase(itr, m_attributes.end());
		m_attrFlags &= ~type;
	}
}


void ItemAttributes::setIntAttr(ItemAttrType type, int32_t value)
{
	assert(validateIntAttrType(type));

	ItemAttribute* const attr = getAttr(type);
	if (attr) {
		attr->integer = value;
	}
}


void ItemAttributes::increaseIntAttr(ItemAttrType type, int32_t value)
{
	assert(validateIntAttrType(type));
	ItemAttribute* const attr = getAttr(type);
	if (attr) {
		attr->integer += value;
	}
}


bool ItemAttributes::validateIntAttrType(ItemAttrType type)
{
	// list of numeric type attributes
	switch (type) {
	case ATTR_ITEM_ACTIONID:
	case ATTR_ITEM_UNIQUEID:
	case ATTR_ITEM_OWNER:
	case ATTR_ITEM_DURATION:
	case ATTR_ITEM_DECAYING:
	case ATTR_ITEM_CHARGES:
	case ATTR_ITEM_FLUIDTYPE:
	case ATTR_ITEM_DOORID:
		return true;

	default:
		return false;
	}

	return false;
}


bool ItemAttributes::validateStrAttrType(ItemAttrType type)
{
	// list of text type attributes
	switch (type) {
	case ATTR_ITEM_DESC:
	case ATTR_ITEM_TEXT:
	case ATTR_ITEM_WRITTENBY:
		return true;

	default:
		break;
	}

	return false;
}


void ItemAttributes::addAttr(ItemAttribute attr)
{
	assert(!(m_attrFlags & attr.type));
	m_attributes.push_back(attr);
	m_attrFlags |= attr.type;
}


ItemAttribute* ItemAttributes::getAttr(ItemAttrType type)
{
	auto attr = static_cast<const ItemAttributes*>(this)->getAttr(type);
	return const_cast<ItemAttribute*>(attr);
}
