#include <mc/Actor.h>
#include <mc/tool.h>
#include <mc/Tag.h>
#include <mc/ItemStack.h>
#include <mc/ScoreBoard.h>

using namespace std;
inline uintptr_t createPacket(int type) {
	uintptr_t pkt[2];
	SymCall("?createPacket@MinecraftPackets@@SA?AV?$shared_ptr@VPacket@@@std@@W4MinecraftPacketIds@@@Z",
		pkt, type);
	return *pkt;
}

string Actor::getNameTag() {
	return SymCall<string&>("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this);
}

void Actor::setNameTag(const string& name) {
	VirtualCall(0x1F8, this, &name);
}

void Actor::setNameTagVisible(bool visible) {
	SymCall("?setNameTagVisible@Actor@@UEAAX_N@Z", this, visible);
}

int Actor::getDimensionId() {
	int did;
	SymCall("?getDimensionId@Actor@@QEBA?AV?$AutomaticID@VDimension@@H@@XZ",
		this, &did);
	return did;
}

Vec3* Actor::getPos() {
	return SymCall<Vec3*>("?getPosition@Actor@@UEBAAEBVVec3@@XZ", this);
}

Vec3* Actor::getPosPrev() {
	return SymCall<Vec3*>("?getPosPrev@Actor@@UEBAAEBVVec3@@XZ", this);
}

float Actor::getSpeedInMetersPerSecond() {
	return SymCall<float>("?getSpeedInMetersPerSecond@Actor@@QEBAMXZ", this);
}

bool Actor::isStanding() {//IDA MovePlayerPacket::MovePlayerPacket
	return Dereference<bool>(this, 472);
}

bool Actor::isSneaking() {
	return SymCall<bool>("?isSneaking@Actor@@QEBA_NXZ", this);
}

// Get block source

BlockSource* Actor::getRegion() {
	return SymCall<BlockSource*>("?getDimensionBlockSourceConst@Actor@@QEBAAEBVBlockSource@@XZ", this);
	//return Dereference<BlockSource*, this + 872);//IDA Actor::getRegion
}

ItemStack* Actor::getArmor(int slot) {
	return SymCall<ItemStack*>("?getArmor@Actor@@UEBAAEBVItemStack@@W4ArmorSlot@@@Z",
		this, slot);
}

unsigned Actor::getEntityTypeId() {
	//return VirtualCall<unsigned>(0x550, this);
	return SymCall<unsigned>("?getEntityTypeId@Actor@@UEBA?AW4ActorType@@XZ", this);
}

uintptr_t Actor::getUniqueID() {
	return SymCall<uintptr_t>("?getUniqueID@Actor@@QEBAAEBUActorUniqueID@@XZ", this);
}

string Actor::getEntityTypeName() {
	string type;
	SymCall<string&>("?EntityTypeToString@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4ActorType@@W4ActorTypeNamespaceRules@@@Z",
		&type, getEntityTypeId());
	return type;
}

// attributes
uintptr_t Actor::updateAttrs() {
	return SymCall<uintptr_t>("?_sendDirtyActorData@Actor@@QEAAXXZ", this);
}

AttributeInstance* Actor::getAttrs(void* a2) {
	return SymCall<AttributeInstance*>("?getMutableAttribute@Actor@@UEAAPEAVAttributeInstance@@AEBVAttribute@@@Z", this, a2);
}

float Actor::getCurrentValue(AttributeInstance *attr) {
	return SymCall<float>("?getCurrentValue@AttributeInstance@@QEBAMXZ", attr);
}

float Actor::getMaxValue(AttributeInstance *attr) {
	return SymCall<float>("?getMaxValue@AttributeInstance@@QEBAMXZ", attr);
}

void Actor::setCurrentValue(AttributeInstance *attr, float value) {
	SymCall("?setCurrentValue@AttributeInstance@@QEAAXM@Z", attr, value);
}

void Actor::setMaxValue(AttributeInstance *attr, float value) {
	SymCall("?setMaxValue@AttributeInstance@@QEAAXM@Z", attr, value);
}

// get level
Level* Actor::getLevel() {
	return SymCall<Level*>("?getCamera@Item@@UEBAPEAVICameraItemComponent@@XZ", this);
}

// add a effect
uintptr_t Actor::addEffect(uintptr_t ef) {
	return SymCall<uintptr_t>("?addEffect@Actor@@QEAAXAEBVMobEffectInstance@@@Z", this, ef);
}

AttributeInstance *Actor::getHealthAttr() {
	return getAttrs(SYM("?HEALTH@SharedAttributes@@2VAttribute@@B"));
}

float Actor::getHealth() {
	AttributeInstance *hattr = getHealthAttr();
	return getCurrentValue(hattr);
}

float Actor::getMaxHealth() {
	AttributeInstance *hattr = getHealthAttr();
	return getMaxValue(hattr);
}

void Actor::setHealth(float value) {
	AttributeInstance *hattr = getHealthAttr();
	setCurrentValue(hattr, value);
}

void Actor::setMaxHealth(float value) {
	AttributeInstance *hattr = getHealthAttr();
	setMaxValue(hattr, value);
}

// get off hand
ItemStack* Actor::getOffhandSlot() {
	return SymCall<ItemStack*>("?getOffhandSlot@Actor@@QEBAAEBVItemStack@@XZ", this);
}

Tag* Actor::save() {
	Tag* t = newTag(TagType::Compound);
	VirtualCall(0x530, this, t);
	//SymCall("?save@Actor@@UEBA_NAEAVCompoundTag@@@Z", this, t);
	return t;
}

void Actor::setSize(float f1, float f2) {
	SymCall("?setSize@Actor@@UEAAXMM@Z", this, f1, f2);
}

auto Actor::getAllEffects() {
	return SymCall<uintptr_t>("?getAllEffects@Actor@@QEBAAEBV?$vector@VMobEffectInstance@@V?$allocator@VMobEffectInstance@@@std@@@std@@XZ", this);
}
class TeleportRotationData {
public:
	char filler[32];
};

void Actor::teleport(Vec3* target, int did) {
	char mem[128];
	TeleportRotationData data;
	SymCall("?computeTarget@TeleportCommand@@SA?AVTeleportTarget@@AEAVActor@@VVec3@@PEAV4@V?$AutomaticID@VDimension@@H@@AEBV?$optional@VRotationData@RotationCommandUtils@@@std@@H@Z",
		&mem, this, target, 0, did, &data, 15);
	SymCall("?applyTarget@TeleportCommand@@SAXAEAVActor@@VTeleportTarget@@_N@Z",
		this, &mem, 0);
}

bool Actor::addTag(const string& str) {
	return SymCall<bool>("?addTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

bool Actor::removeTag(const string& str) {
	return SymCall<bool>("?removeTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

span<string> Actor::getTags() {
	span<string> tags;
	SymCall<span<string>&>("?getTags@Actor@@QEBA?BV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@XZ",
		this, &tags);
	return tags;
}

void Actor::kill() {
	SymCall("?kill@Mob@@UEAAXXZ", this);
}

UserEntityIdentifierComponent* Player::getUserEntityIdentifierComponent() {
	return SymCall<UserEntityIdentifierComponent*>("??$tryGetComponent@VUserEntityIdentifierComponent@@@Actor@@QEAAPEAVUserEntityIdentifierComponent@@XZ",
		this);
}

string Player::getUuid() {//IDA ServerNetworkHandler::_createNewPlayer 222
	//uintptr_t userid = SymCall<uintptr_t>("??$tryGetComponent@VUserEntityIdentifierComponent@@@Actor@@QEAAPEAVUserEntityIdentifierComponent@@XZ",
	//	this);
	string p;
	void* v33 = **(void***)(this + 8);
	int v107 = *(int*)(this + 16);
	void* v34 = SymCall<void*>("??$try_get@VUserEntityIdentifierComponent@@@?$basic_registry@VEntityId@@V?$allocator@VEntityId@@@std@@@entt@@QEBA?A_PVEntityId@@@Z", v33, &v107);
	SymCall<string&>("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		uintptr_t(v34) + 168, &p);
	return p;
}

string Player::getXuid() {
	string xuid;
	SymCall<string&>("?getXuid@Player@@UEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this,&xuid);
	return xuid;
}

// Get network identifier

NetworkIdentifier* Player::getClientId() {
	return &getUserEntityIdentifierComponent()->nid;
	//void* v6 = **(void***)(this + 8);
	//int v58 = *(int*)(this + 16);
	//char* v7 = SymCall<char*>("??$try_get@VUserEntityIdentifierComponent@@@?$basic_registry@VEntityId@@@entt@@QEBA?A_PVEntityId@@@Z", v6, &v58);
	//return (NetworkIdentifier*)(uintptr_t(v7) + 160);
	//IDA ServerPlayer::setPermissions 34
}

Container* Player::getInventory() {
	return SymCall<Container*>("?getInventory@Player@@QEAAAEAVContainer@@XZ",
		this);
	//return Dereference<Container*, Dereference<uintptr_t, this + 3208) + 176);//IDA Player::getInventory
}

Container* Player::getArmorContainer() {
	return SymCall<Container*>("?getArmorContainer@Actor@@QEBAAEBVSimpleContainer@@XZ",
		this);
}

Container* Player::getEnderChestContainer() {
	//IDA Player::Player Line776: v80 = (char *)this + 3800;
	return Dereference<Container*>(this, 3800);
}

// Set armor
uintptr_t Player::setArmor(int i, ItemStack* item) {
	return SymCall<uintptr_t>("?setArmor@ServerPlayer@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z",
		this, i, item);
}

uintptr_t Player::setOffhandSlot(ItemStack* item) {
	return SymCall<uintptr_t>("?setOffhandSlot@Player@@UEAAXAEBVItemStack@@@Z",
		this, item);
}

// Add a item
void Player::addItem(ItemStack* item) {
	// waiting for fix, test console commands:
	// pydebug
	// import json, mc
	// mc.getPlayerList()[0].addItem(json.dumps({"Count1":1,"Damage2":0,"Name8":"minecraft:netherite_sword","WasPickedUp1":0}))
	//SymCall("?addItem@PlayerUtils@@YAXAEAVPlayer@@AEAVItemStack@@@Z",
	//	this->getLevel(), this, item);

	SymCall("?add@Player@@UEAA_NAEAVItemStack@@@Z", this, item);
}

// Add exp level
void Player::addLevel(int level) {
	SymCall("?addLevels@Player@@UEAAXH@Z",
		this, level);
}

// Get the item currently in selected slot
int Player::getSelectedItemSlot() {
	return SymCall<int>("?getSelectedItemSlot@Player@@QEBAHXZ",
		this);
}

// Get the item currently in hand
ItemStack* Player::getSelectedItem() {
	return SymCall<ItemStack*>("?getSelectedItem@Player@@QEBAAEBVItemStack@@XZ",
		this);
}

ItemStack* Player::getInventoryItem(int slot) {
	return getInventory()->getSlots()[slot];
}

PlayerPermissionLevel Player::getPlayerPermissionLevel() {
	return SymCall<PlayerPermissionLevel>("?getPlayerPermissionLevel@Player@@QEBA?AW4PlayerPermissionLevel@@XZ",
		this);
}

void Player::setPermissions(PlayerPermissionLevel m) {
	_Big_uint128 v93[8];
	SymCall("?setPermissions@Player@@QEAAXW4CommandPermissionLevel@@@Z",
		this, m);
	memset(v93, 0, sizeof(v93));
	SymCall<uintptr_t>("??0UpdateAbilitiesPacket@@QEAA@UActorUniqueID@@AEBVLayeredAbilities@@@Z",
		v93, Actor::getUniqueID(), this + 2508);
}

//获取设备id
string Player::getPlatformOnlineId() {
	string id;
	SymCall<string&>("?getPlatformOnlineId@Player@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this, &id);
	return id;
	//return Dereference<string, this + 8352);
	//IDA Player::Player  v13 + 8352
}

//获取设备系统类型

unsigned Player::getPlatform() {
	return SymCall<unsigned>("?getPlatform@Player@@QEBA?AW4BuildPlatform@@XZ",
		this);
	//return Dereference<int, this + 2336);
	//IDA ServerNetworkHandler::createNewPlayer  ConnectionRequest::getDeviceOS
}

//发送背包

void Player::sendInventroy() {
	SymCall("?sendInventory@ServerPlayer@@UEAAX_N@Z",
		this, true);
}


void Player::openInventoryGUI(){
	SymCall("?openInventory@ServerPlayer@@UEAAXXZ",
		this);
}

//刷新区块

void Player::resendAllChunks() {
	SymCall("?resendAllChunks@Player@@UEAAXXZ", this);
}

//发送数据包

void Player::sendNetworkPacket(uintptr_t pkt) {
	SymCall("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z",
		this, pkt);
}

unsigned Player::sendModalFormRequestPacket(const string& str) {
	static unsigned id = 0;
	uintptr_t pkt = createPacket(100);
	Dereference<unsigned>(pkt, 48) = ++id;
	Dereference<string>(pkt, 56) = str;
	sendNetworkPacket(pkt);
	return id;
}

void Player::sendTransferPacket(const string& address, unsigned short port) {
	uintptr_t pkt = createPacket(85);
	Dereference<string>(pkt, 48) = address;
	Dereference<unsigned short>(pkt, 80) = port;
	sendNetworkPacket(pkt);
}

void Player::sendDisconnectPacket(const string& msg) {
	uintptr_t pkt = createPacket(5);
	Dereference<string>(pkt, 56) = msg;
	sendNetworkPacket(pkt);
}

void Player::sendTextPacket(int mode, const string& msg) {
	uintptr_t pkt = createPacket(9);
	Dereference<int>(pkt, 48) = mode;
	Dereference<string>(pkt, 56) = getNameTag();
	Dereference<string>(pkt, 88) = msg;
	sendNetworkPacket(pkt);
}

void Player::sendCommandRequestPacket(const string& cmd) {
	uintptr_t pkt = createPacket(77);
	Dereference<string>(pkt, 48) = cmd;
	//SymCall<uintptr_t>("?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
	//	global<ServerNetworkHandler>, getClientId(), pkt);
	sendNetworkPacket(pkt);
}

void Player::sendBossEventCodePacket(string name, float per, int eventtype) {
	uintptr_t pkt = createPacket(74);
	Dereference<uintptr_t>(pkt, 56) = Dereference<uintptr_t>(getUniqueID());
	//Dereference<uintptr_t, pkt + 64) = Dereference<uintptr_t, getUniqueID());
	Dereference<int>(pkt, 72) = eventtype;//0显示,1更新,2隐藏,
	Dereference<string>(pkt, 80) = name;
	Dereference<float>(pkt, 112) = per;
	sendNetworkPacket(pkt);
}

void Player::sendsetDisplayObjectivePacket(const string& title, const string& name) {
	uintptr_t pkt = createPacket(107);
	Dereference<string>(pkt, 48) = "sidebar";
	Dereference<string>(pkt, 80) = name;
	Dereference<string>(pkt, 112) = title;
	Dereference<string>(pkt, 144) = "dummy";
	Dereference<char>(pkt, 176) = 0;
	sendNetworkPacket(pkt);
}

void Player::sendSetScorePacket(char type, const vector<ScorePacketInfo>& slot) {
	uintptr_t pkt = createPacket(108);
	Dereference<char>(pkt, 48) = type;//{set,remove}
	Dereference<vector<ScorePacketInfo>>(pkt, 56) = slot;
	sendNetworkPacket(pkt);
}

void Player::sendPlaySoundPacket(const std::string& soundName, Vec3 pos, float volume, float pitch) {
	uintptr_t pkt = createPacket(86);
	Dereference<std::string>(pkt, 48) = soundName;
	Dereference<int>(pkt, 80) = int(pos.x * 8.0);
	Dereference<int>(pkt, 84) = int(pos.y * 8.0);
	Dereference<int>(pkt, 88) = int(pos.z * 8.0);
	Dereference<float>(pkt, 92) = volume;
	Dereference<float>(pkt, 96) = pitch;
	sendNetworkPacket(pkt);
}

// May not close the GUI, but will definitely close the container, after closing the player can not operate the container
void Player::doDeleteContainerManager() {
	SymCall("?doDeleteContainerManager@ServerPlayer@@QEAAX_N@Z",
		this, false);
}
