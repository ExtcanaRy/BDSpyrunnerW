#pragma once
#include "json_tool.h"

struct Tag;
struct Block;
struct BlockLegacy;
struct Item;
struct ItemStack {
	//uintptr_t vtable;
	//Item* mItem;
	//Tag* mUserData;
	//Block* mBlock;
	//short mAuxValue;
	//char mCount;
	//bool mValid;
	//uintptr_t mPickupTime;
	//bool mShowPickUp;
	//vector<BlockLegacy*> mCanPlaceOn;
	//uintptr_t mCanPlaceOnHash;
	//vector<BlockLegacy*> mCanDestroy;
	//uintptr_t mCanDestroyHash;
	//uintptr_t mBlockingTick;
	//ItemStack* mChargedItem;
	//uintptr_t unk;
	char _this[144];

	ItemStack();
	ItemStack(const Json&);
	~ItemStack();
	short getAuxValue();
	unsigned char getCount();
	short getId();
	short getDamageValue();
	std::string getName();
	int getStackCount();
	bool isNull();
	bool isEmptyStack();
	Tag* getNetworkUserData();
	Tag* save();
	ItemStack* fromTag(Tag* t);
	//bool getFromId(short id, short aux, char count);
	Item* getItem();
	void fromJson(const Json& value);
};
struct Container {
	std::vector<ItemStack*> getSlots();
	void clearItem(int slot, int num);
};
