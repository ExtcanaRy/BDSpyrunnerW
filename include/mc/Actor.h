#pragma once
#include <vector>
#include "Position.h"
#include "span.h"
#include "NetWork.h"

struct BlockSource;
struct ItemStack;
struct Level;
struct Tag;
struct Container;
struct ScorePacketInfo;
struct Abilities;
struct AttributeInstance;
enum class PlayerPermissionLevel : uint8_t {
	Visitor,
	Member,
	Operator,
	Custom
};
enum class CommandPermissionLevel : uint8_t {
	Any,
	GameMasters,
	Admin,
	Host,
	Owner,
	Internal 
};
struct UserEntityIdentifierComponent {
	NetworkIdentifier nid;
	char unk;
	void* uuid[2];
	struct Certificate* cert;
};
struct Actor {
	std::string getNameTag();
	void setNameTag(const std::string&);
	void setNameTagVisible(bool visible);
	int getDimensionId();
	Vec3* getPos();
	Vec3* getPosPrev();
	float getSpeedInMetersPerSecond();
	bool isStanding();
	bool isSneaking();
	BlockSource* getRegion();
	ItemStack* getArmor(int slot);
	unsigned getEntityTypeId();
	uintptr_t getOrCreateUniqueID();
	std::string getEntityTypeName();
	uintptr_t updateAttrs();
	AttributeInstance *getAttrs(void* a2);
	AttributeInstance *getHealthAttr();
	float getCurrentValue(AttributeInstance *attr);
	float getMaxValue(AttributeInstance *attr);
	void setCurrentValue(AttributeInstance *attr, float value);
	void setMaxValue(AttributeInstance *attr, float value);
	Level* getLevel();
	uintptr_t addEffect(uintptr_t ef);
	float getHealth();
	float getMaxHealth();
	void setHealth(float value);
	void setMaxHealth(float value);
	ItemStack* getOffhandSlot();
	Tag* save();
	// void setSize(float f1, float f2);
	auto getAllEffects();
	void teleport(Vec3* target, int did);
	bool addTag(const std::string& str);
	bool removeTag(const std::string& str);
	span<std::string> getTags();
	void kill();
};
struct Mob : Actor {};
struct Player : Mob {
	UserEntityIdentifierComponent* getUserEntityIdentifierComponent();
	std::string getUuid();
	std::string getXuid();
	NetworkIdentifier* getClientId();
	Container* getInventory();
	Container* getArmorContainer();
	Container* getEnderChestContainer();
	uintptr_t setArmor(int i, ItemStack* item);
	uintptr_t setOffhandSlot(ItemStack* item);
	void addItem(ItemStack* item);
	void addLevel(int level);
	int getSelectedItemSlot();
	ItemStack* getSelectedItem();
	ItemStack* getInventoryItem(int slot);
	PlayerPermissionLevel getPlayerPermissionLevel();
	void setPermissions(PlayerPermissionLevel m);
	Abilities* getAbilities();
	std::string getPlatformOnlineId();
	unsigned getPlatform();
	void sendInventroy();
	void openInventoryGUI();
	void doDeleteContainerManager();
	void resendAllChunks();
	void sendNetworkPacket(uintptr_t pkt);
	unsigned sendModalFormRequestPacket(const std::string& str);
	void sendTransferPacket(const std::string& address, unsigned short port);
	void sendDisconnectPacket(const std::string& msg);
	void sendTextPacket(int mode, const std::string& msg);
	void sendCommandRequestPacket(const std::string& cmd);
	void sendBossEventCodePacket(std::string name, float per, int eventtype);
	void sendsetDisplayObjectivePacket(const std::string& title, const std::string& name = "name");
	void sendSetScorePacket(char type, const std::vector<ScorePacketInfo>& slot);
	void sendPlaySoundPacket(const std::string& soundName, Vec3 position, float volume = 1.0, float pitch = 1.0);
};
inline bool IsPlayer(Actor* ptr) {
	if (ptr == nullptr)
		return false;
	if (ptr->getEntityTypeId() != 0x13F)
		return false;
	return true;
}
inline bool IsPlayerInit(Actor* ptr) {
	//ServerPlayer::isPlayerInitialized  !*((_BYTE *)this + 7680)
	return !*((char*)ptr + 7552);
}
