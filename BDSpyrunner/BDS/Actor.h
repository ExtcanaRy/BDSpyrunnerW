#pragma once
#include "../tool.h"
#include "Math.h"
#include "span.h"

struct BlockSource;
struct ItemStack;
struct Level;
struct Tag;
struct NetworkIdentifier;
struct Container;
struct Actor {
	//��ȡ����������Ϣ
	string getNameTag();
	//��ȡ���ﵱǰ����ά��ID
	int getDimensionId();
	//��ȡ���ﵱǰ��������
	Vec3* getPos();
	//��ȡ����֮ǰ��������
	Vec3* getPosOld();
	//�Ƿ����Ƴ�
	bool isRemoved();
	//�Ƿ�����
	bool isStand();
	//ȡ����Դ
	BlockSource* getRegion();
	ItemStack* getArmor(int slot);
	//��ȡʵ������
	unsigned getEntityTypeId();
	//��ȡ��ѯ��ID
	VA getUniqueID();
	//��ȡʵ������
	string getEntityTypeName();
	//��������
	VA updateAttrs();
	//��ȡ��ͼ��Ϣ
	Level* getLevel();
	//���һ��״̬
	VA addEffect(VA ef);
	//��ȡ����ֵ
	int getHealth();
	int getMaxHealth();
	void setHealth(int value);
	void setMaxHealth(int value);
	//��ȡ����
	ItemStack* getOffHand();
	Tag* save();
	//���ô�С
	void setSize(float f1, float f2);
	//�Ƿ�Ǳ��
	bool isSneaking();
	//��ȡ״̬�б�
	auto getAllEffects();
	//����
	void teleport(Vec3* target, int did);
	//������ǩ
	bool addTag(const string& str);
	//�Ƴ���ǩ
	bool removeTag(const string& str);
	//��ȡ��ǩ
	span<string> getTags();
};
struct Mob : Actor {};
struct Player : Mob {
	string getUuid();
	//�������ݰ�
	void sendPacket(VA pkt);
	//���ݵ�ͼ��Ϣ��ȡ���xuid
	string& getXuid();
	//��������������
	void setName(const string& name);
	//��ȡ�����ʶ��
	NetworkIdentifier* getClientId();
	//��ȡ����
	Container* getInventory();
	//��ȡװ������
	Container* getArmorContainer();
	//��ȡĩӰ��
	Container* getEnderChestContainer();
	//����һ��װ��
	VA setArmor(int i, ItemStack* item);
	//���ø���
	VA setOffhandSlot(ItemStack* item);
	//���һ����Ʒ
	void addItem(ItemStack* item);
	//���ӵȼ�
	void addLevel(int level);
	//��ȡ��ǰѡ�еĿ�λ��
	int getSelectedItemSlot();
	//��ȡ��ǰ��Ʒ
	ItemStack* getSelectedItem();
	//��ȡ������Ʒ
	ItemStack* getInventoryItem(int slot);
	//��ȡ��Ϸʱ����Ȩ��
	char getPermissions();
	//������Ϸʱ����Ȩ��
	void setPermissions(char m);
	//��ȡ��Ϸʱ����Ȩ��
	char getPermissionLevel();
	//������Ϸʱ����Ȩ��
	void setPermissionLevel(char m);
	//��ȡ�豸id
	string getDeviceId();
	//��ȡ�豸ϵͳ����
	int getDeviceOS();
	//���ͱ���
	void sendInventroy();
	//ˢ������
	void resendAllChunks();
	//�Ͽ�����
	//void disconnect() {
	//	SymCall("?disconnect@ServerPlayer@@QEAAXXZ",this);
	//}
};
