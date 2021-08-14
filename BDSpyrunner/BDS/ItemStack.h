#pragma once
#include "../tool.h"
#include "Tag.h"

struct Item;
struct Block;
struct BlockLegacy;
struct Item;
struct ItemStack {
	VA vtable;
	Item* mItem;
	Tag* mUserData;
	Block* mBlock;
	short mAuxjson;
	char mCount;
	bool mValid;
	VA mPickupTime;
	bool mShowPickUp;
	vector<BlockLegacy*> mCanPlaceOn;
	VA mCanPlaceOnHash;
	vector<BlockLegacy*> mCanDestroy;
	VA mCanDestroyHash;
	VA mBlockingTick;
	ItemStack* mChargedItem;
	VA unk;

	//ȡ��ƷID,����ֵ,���
	short getId();
	short getDamageValue();
	//ȡ��Ʒ����
	string getName();
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
	void fromJson(const json& value);
};
struct Container {
	//��ȡ������������Ʒ
	vector<ItemStack*> getSlots();
	void clearItem(int slot, int num);
};
