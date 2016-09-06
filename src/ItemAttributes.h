#ifndef __OTSERV_ITEMATTRIBUTE_H__
#define __OTSERV_ITEMATTRIBUTE_H__

#include <cstdint>
#include <vector>
#include "log.h"


enum ItemDecayState_t { 
	DECAYING_FALSE = 0,
	DECAYING_TRUE,
	DECAYING_PENDING 
};

enum Attr_ReadValue {
	ATTR_READ_ERROR,
	ATTR_READ_CONTINUE,
	ATTR_READ_END 
};


enum ItemAttrType {
	ATTR_ITEM_ACTIONID = 0x01,
	ATTR_ITEM_UNIQUEID = 0x02,
	ATTR_ITEM_DESC = 0x04,
	ATTR_ITEM_TEXT = 0x08,
	ATTR_ITEM_WRITTENBY = 0x10,
	ATTR_ITEM_OWNER = 0x20,
	ATTR_ITEM_DURATION = 0x40,
	ATTR_ITEM_DECAYING = 0x80,
	ATTR_ITEM_CHARGES = 0x100,
	ATTR_ITEM_FLUIDTYPE = 0x200,
	ATTR_ITEM_DOORID = 0x300
};

enum ItemAttrValType {
	INTEGER,
	STRING
};


struct ItemAttribute
{
	ItemAttribute(ItemAttrType type);
	ItemAttribute(const ItemAttribute& other);
	ItemAttribute(const ItemAttribute&& other);
	~ItemAttribute();
	ItemAttribute& operator=(const ItemAttribute& other);
	ItemAttribute& operator=(const ItemAttribute&& other);
	ItemAttrValType GetValueType();

	union {
		int32_t integer;
		std::string str;
	};

	ItemAttrType type;
};




inline ItemAttribute::ItemAttribute(ItemAttrType type) : 
	type(type) 
{

}

inline ItemAttribute::ItemAttribute(const ItemAttribute& other)
{
	*this = other;
}

inline ItemAttribute::ItemAttribute(const ItemAttribute&& other)
{
	*this = std::move(other);
}

inline ItemAttribute::~ItemAttribute()
{

}

inline ItemAttribute& ItemAttribute::operator=(const ItemAttribute& other)
{
	if (this != &other) {
		type = other.type;
		switch(GetValueType()) {
		case INTEGER: integer = other.integer; break;
		case STRING: str = other.str; break;
		default: break;
		}
	}

	return *this;
}



inline ItemAttribute& ItemAttribute::operator=(const ItemAttribute&& other)
{
	if (this != &other) {
		type = other.type;
		switch(GetValueType()) {
		case INTEGER: integer = other.integer; break;
		case STRING: str = std::move(other.str); break;
		default: break;
		}
	}

	return *this;
}



inline ItemAttrValType ItemAttribute::GetValueType()
{
	ItemAttrValType valtype = INTEGER;
	switch (type) {
	case ATTR_ITEM_ACTIONID:  /* fall */
	case ATTR_ITEM_UNIQUEID:  /* fall */
	case ATTR_ITEM_OWNER:     /* fall */
	case ATTR_ITEM_DURATION:  /* fall */
	case ATTR_ITEM_DECAYING:  /* fall */
	case ATTR_ITEM_CHARGES:   /* fall */
	case ATTR_ITEM_FLUIDTYPE: /* fall */
	case ATTR_ITEM_DOORID: valtype = INTEGER; break;

	case ATTR_ITEM_DESC: /* fall */
	case ATTR_ITEM_TEXT: /* fall */
	case ATTR_ITEM_WRITTENBY: valtype = STRING; break;
	default:
		LOG_ERROR("UNDEFIED ITEM ATTRIBUTE VALUE TYPE");
		break;
	}

	return valtype;
}










class ItemAttributes
{
public:
	ItemAttributes() = default;
	ItemAttributes(const ItemAttributes& other);
	virtual ~ItemAttributes();

	const std::string& getSpecialDescription() const;
	const std::string& getText() const;
	const std::string& getWriter() const;
	uint16_t getActionId() const;
	uint16_t getUniqueId() const;
	uint16_t getCharges() const;
	uint16_t getFluidType() const;
	uint32_t getOwner() const;
	int32_t getDuration() const;
	uint32_t getDecaying() const;

	void setSpecialDescription(const std::string& desc);
	void setText(const std::string& text);
	void setWriter(std::string writer);
	void setActionId(uint16_t n);

	void setUniqueId(uint16_t n);
	void setCharges(uint16_t n);
	void setFluidType(uint16_t n);
	void setOwner(uint32_t owner);
	void setDuration(int32_t time);
	void decreaseDuration(int32_t time);
	void setDecaying(ItemDecayState_t decayState);

	void resetSpecialDescription();
	void resetText();
	void resetWriter();



protected:
	bool hasAttribute(ItemAttrType type) const;
	const std::string& getStrAttr(ItemAttrType type) const;
	uint32_t getIntAttr(ItemAttrType type) const;
	const ItemAttribute* getAttr(ItemAttrType type) const;
	void removeAttribute(ItemAttrType type);

	void setStrAttr(ItemAttrType type, const std::string& value);
	void setIntAttr(ItemAttrType type, int32_t value);
	void increaseIntAttr(ItemAttrType type, int32_t value);
	void addAttr(ItemAttribute attr);
	ItemAttribute* getAttr(ItemAttrType type);


	static bool validateIntAttrType(ItemAttrType type);
	static bool validateStrAttrType(ItemAttrType type);

	std::vector<ItemAttribute> m_attributes;
	uint16_t m_attrFlags = 0;
};





inline const std::string& ItemAttributes::getSpecialDescription() const
{
	return getStrAttr(ATTR_ITEM_DESC);
}

inline const std::string& ItemAttributes::getText() const
{
	return getStrAttr(ATTR_ITEM_TEXT);
}

inline const std::string& ItemAttributes::getWriter() const
{
	return getStrAttr(ATTR_ITEM_WRITTENBY);
}

inline uint16_t ItemAttributes::getActionId() const
{
	return static_cast<uint16_t>(getIntAttr(ATTR_ITEM_ACTIONID));
}

inline uint16_t ItemAttributes::getUniqueId() const
{
	return static_cast<uint16_t>(getIntAttr(ATTR_ITEM_UNIQUEID));
}

inline uint16_t ItemAttributes::getCharges() const
{
	return static_cast<uint16_t>(getIntAttr(ATTR_ITEM_CHARGES));
}

inline uint16_t ItemAttributes::getFluidType() const
{
	return static_cast<uint16_t>(getIntAttr(ATTR_ITEM_FLUIDTYPE));
}

inline uint32_t ItemAttributes::getOwner() const
{
	return static_cast<uint32_t>(getIntAttr(ATTR_ITEM_OWNER));
}

inline int32_t ItemAttributes::getDuration() const
{
	return static_cast<uint32_t>(getIntAttr(ATTR_ITEM_DURATION));
}

inline uint32_t ItemAttributes::getDecaying() const
{
	return static_cast<uint32_t>(getIntAttr(ATTR_ITEM_DECAYING));
}





inline ItemAttributes::ItemAttributes(const ItemAttributes& other) :
	m_attributes(other.m_attributes)
{
}

inline ItemAttributes::~ItemAttributes()
{
}


inline void ItemAttributes::setSpecialDescription(const std::string& desc)
{
	setStrAttr(ATTR_ITEM_DESC, desc);
}

inline void ItemAttributes::resetSpecialDescription()
{
	removeAttribute(ATTR_ITEM_DESC);
}


inline void ItemAttributes::setText(const std::string& text)
{
	setStrAttr(ATTR_ITEM_TEXT, text);
}

inline void ItemAttributes::resetText()
{
	removeAttribute(ATTR_ITEM_TEXT);
}


inline void ItemAttributes::setWriter(const std::string writer)
{
	setStrAttr(ATTR_ITEM_WRITTENBY, writer);
}

inline void ItemAttributes::resetWriter()
{
	removeAttribute(ATTR_ITEM_WRITTENBY);
}

inline void ItemAttributes::setActionId(uint16_t n)
{
	if (n < 100) 
		n = 100;

	setIntAttr(ATTR_ITEM_ACTIONID, n);
}

inline void ItemAttributes::setUniqueId(uint16_t n)
{
	if (n < 1000)
		n = 1000;

	setIntAttr(ATTR_ITEM_UNIQUEID, n);
}

inline void ItemAttributes::setCharges(uint16_t n)
{
	setIntAttr(ATTR_ITEM_CHARGES, n);
}


inline void ItemAttributes::setFluidType(uint16_t n)
{
	setIntAttr(ATTR_ITEM_FLUIDTYPE, n);
}


inline void ItemAttributes::setOwner(uint32_t owner)
{
	setIntAttr(ATTR_ITEM_OWNER, owner);
}


inline void ItemAttributes::setDuration(int32_t time)
{
	setIntAttr(ATTR_ITEM_DURATION, time);
}


inline void ItemAttributes::decreaseDuration(int32_t time)
{
	increaseIntAttr(ATTR_ITEM_DURATION, -time);
}


inline void ItemAttributes::setDecaying(ItemDecayState_t decayState)
{
	setIntAttr(ATTR_ITEM_DECAYING, decayState);
}















#endif
