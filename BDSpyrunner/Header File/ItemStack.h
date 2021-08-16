#pragma once
#include "json.hpp"
#include "Tag.h"

struct Item;
struct Block;
struct BlockLegacy;
struct Item;
struct ItemStack {
	//VA vtable;
	//Item* mItem;
	//Tag* mUserData;
	//Block* mBlock;
	//short mAuxValue;
	//char mCount;
	//bool mValid;
	//VA mPickupTime;
	//bool mShowPickUp;
	//vector<BlockLegacy*> mCanPlaceOn;
	//VA mCanPlaceOnHash;
	//vector<BlockLegacy*> mCanDestroy;
	//VA mCanDestroyHash;
	//VA mBlockingTick;
	//ItemStack* mChargedItem;
	//VA unk;
	char _this[144];

	ItemStack();
	ItemStack(const Json&);
	~ItemStack();
	//ȡ��ƷID,����ֵ,���
	short getAuxValue();
	unsigned char getCount();
	short getId();
	short getDamageValue();
	//ȡ��Ʒ����
	std::string getName();
	//ȡ����������
	int getStackCount();
	//�ж��Ƿ������
	bool isNull();
	bool isEmptyStack();
	Tag* getNetworkUserData();
	Tag* save();
	ItemStack* fromTag(Tag* t);
	bool getFromId(short id, short aux, char count);
	Item* getItem();
	void fromJson(const Json& value);
};
struct Container {
	//��ȡ������������Ʒ
	std::vector<ItemStack*> getSlots();
	void clearItem(int slot, int num);
};
