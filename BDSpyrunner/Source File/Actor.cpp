#include <Actor.h>
#include <Tag.h>
#include <ItemStack.h>
#include <global.h>
#include <ScoreBoard.h>

using namespace std;
string Actor::getNameTag() {
	return SymCall<string&>("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this);
}

//��������������Ϣ

void Actor::setNameTag(const std::string&name) {
	VirtualCall(0x1F8, this, &name);
}

//�������������Ƿ�ɼ�

void Actor::setNameTagVisible(bool visible) {
	SymCall("?setNameTagVisible@Actor@@UEAAX_N@Z", this, visible);
}

//��ȡ���ﵱǰ����ά��ID

int Actor::getDimensionId() {
	int did;
	SymCall("?getDimensionId@Actor@@UEBA?AV?$AutomaticID@VDimension@@H@@XZ",
		this, &did);
	return did;
	//return FETCH(int, this + 236);//IDA Actor::getDimensionId
}

//��ȡ���ﵱǰ��������

Vec3* Actor::getPos() {
	return SymCall<Vec3*>("?getPos@Actor@@UEBAAEBVVec3@@XZ", this);
	//return (Vec3*)(this + 1268);//IDA Actor::getPos
}

//��ȡ����֮ǰ��������

Vec3* Actor::getPosOld() {
	return SymCall<Vec3*>("?getPosOld@Actor@@UEBAAEBVVec3@@XZ", this);
	//return (Vec3*)(this + 1280);//IDA Actor::getPosOld
}

//�Ƿ����Ƴ�

bool Actor::isRemoved() {
	return FETCH(bool, this + 7688);
}

//�Ƿ�����

bool Actor::isStand() {//IDA MovePlayerPacket::MovePlayerPacket 30
	return FETCH(bool, this + 480);
}


//ȡ����Դ

BlockSource* Actor::getRegion() {
	return SymCall<BlockSource*>("?getRegion@Actor@@QEBAAEAVBlockSource@@XZ", this);
	//return FETCH(BlockSource*, this + 872);//IDA Actor::getRegion
}

ItemStack* Actor::getArmor(int slot) {
	return SymCall<ItemStack*>("?getArmor@Actor@@UEBAAEBVItemStack@@W4ArmorSlot@@@Z",
		this, slot);
}

//��ȡʵ������

unsigned Actor::getEntityTypeId() {
	return VirtualCall<unsigned>(0x520, this);
	//return SymCall<unsigned>("?getEntityTypeId@Actor@@UEBA?AW4ActorType@@XZ", this);
}

//��ȡ��ѯ��ID

uintptr_t Actor::getUniqueID() {
	return SymCall<uintptr_t>("?getUniqueID@Actor@@QEBAAEBUActorUniqueID@@XZ", this);
}

//��ȡʵ������

string Actor::getEntityTypeName() {
	string type;
	SymCall<string&>("?EntityTypeToString@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4ActorType@@W4ActorTypeNamespaceRules@@@Z",
		&type, getEntityTypeId());
	return type;
}

//��������

uintptr_t Actor::updateAttrs() {
	return SymCall<uintptr_t>("?_sendDirtyActorData@Actor@@QEAAXXZ", this);
}

//��ȡ��ͼ��Ϣ

Level* Actor::getLevel() {
	return SymCall<Level*>("?getLevel@Actor@@QEBAAEBVLevel@@XZ", this);
}

//���һ��״̬

uintptr_t Actor::addEffect(uintptr_t ef) {
	return SymCall<uintptr_t>("?addEffect@Actor@@QEAAXAEBVMobEffectInstance@@@Z", this, ef);
}

//��ȡ����ֵ

int Actor::getHealth() {
	return SymCall<int>("?getHealth@Actor@@QEBAHXZ", this);
}

int Actor::getMaxHealth() {
	return SymCall<int>("?getMaxHealth@Actor@@QEBAHXZ", this);
}

void Actor::setHealth(int value) {
	uintptr_t hattr = (*reinterpret_cast<uintptr_t(**)(Actor*, void*)>(*(uintptr_t*)this + 1552))
		(this, SYM("?HEALTH@SharedAttributes@@2VAttribute@@B"));
	FETCH(int, hattr + 132) = value;
	//SymCall("?_setDirty@AttributeInstance@@AEAAXXZ", hattr);
}

void Actor::setMaxHealth(int value) {
	uintptr_t hattr = (*reinterpret_cast<uintptr_t(**)(Actor*, void*)>(*(uintptr_t*)this + 1552))
		(this, SYM("?HEALTH@SharedAttributes@@2VAttribute@@B"));
	FETCH(int, hattr + 128) = value;
	//SymCall("?_setDirty@AttributeInstance@@AEAAXXZ", hattr);
}

//��ȡ����

ItemStack* Actor::getOffHand() {
	return SymCall<ItemStack*>("?getOffhandSlot@Actor@@QEBAAEBVItemStack@@XZ", this);
}

Tag* Actor::save() {
	Tag* t = newTag(TagType::Compound);
	SymCall("?save@Actor@@UEAA_NAEAVCompoundTag@@@Z", this, t);
	return t;
}

//���ô�С

void Actor::setSize(float f1, float f2) {
	SymCall("?setSize@Actor@@UEAAXMM@Z", this, f1, f2);
}

//�Ƿ�Ǳ��

bool Actor::isSneaking() {
	return SymCall<bool>("?isSneaking@Actor@@QEBA_NXZ", this);
}

//��ȡ״̬�б�

auto Actor::getAllEffects() {
	//return SymCall<vector<MobEffectInstance>*>("?getAllEffects@Actor@@QEBAAEBV?$vector@VMobEffectInstance@@V?$allocator@VMobEffectInstance@@@std@@@std@@XZ", this);
}

//����

void Actor::teleport(Vec3* target, int did) {
	char mem[128];
	SymCall("?computeTarget@TeleportCommand@@SA?AVTeleportTarget@@AEAVActor@@VVec3@@PEAV4@V?$AutomaticID@VDimension@@H@@VRelativeFloat@@4H@Z",
		&mem, this, target, 0, did, 0, 0, 15);
	SymCall("?applyTarget@TeleportCommand@@SAXAEAVActor@@VTeleportTarget@@@Z",
		this, &mem);
}

//������ǩ

bool Actor::addTag(const string& str) {
	return SymCall<bool>("?addTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

//�Ƴ���ǩ

bool Actor::removeTag(const string& str) {
	return SymCall<bool>("?removeTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

//��ȡ��ǩ

span<string> Actor::getTags() {
	span<string> tags;
	SymCall<span<string>&>("?getTags@Actor@@QEBA?BV?$span@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0?0@gsl@@XZ",
		this, &tags);
	return tags;
}

string Player::getUuid() {//IDA ServerNetworkHandler::_createNewPlayer 222
	string p;
	SymCall<string&>("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this + 3000, &p);
	return p;
}

//�������ݰ�

//���ݵ�ͼ��Ϣ��ȡ���xuid

string& Player::getXuid() {
	return SymCall<string&>("?getPlayerXUID@Level@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z",
		getLevel(), this + 3000);
}

//��ȡ�����ʶ��

NetworkIdentifier* Player::getClientId() {
	return SymCall<NetworkIdentifier*>("?getClientId@Player@@QEBAAEBVNetworkIdentifier@@XZ",
		this);
	//IDA ServerPlayer::setPermissions 34
}

//��ȡ����

Container* Player::getInventory() {
	return SymCall<Container*>("?getInventory@Player@@QEAAAEAVContainer@@XZ", this);
	//return FETCH(Container*, FETCH(uintptr_t, this + 3208) + 176);//IDA Player::getInventory
}

//��ȡװ������

Container* Player::getArmorContainer() {
	return FETCH(Container*, this + 1648);//IDA Actor::_setArmorContainer 11
}

//��ȡĩӰ��

Container* Player::getEnderChestContainer() {
	return FETCH(Container*, this + 4360);//IDA ReplaceItemCommand::execute 1086 
}

//����һ��װ��

uintptr_t Player::setArmor(int i, ItemStack* item) {
	return SymCall<uintptr_t>("?setArmor@ServerPlayer@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z", this, i, item);
}

//���ø���

uintptr_t Player::setOffhandSlot(ItemStack* item) {
	return SymCall<uintptr_t>("?setOffhandSlot@Player@@UEAAXAEBVItemStack@@@Z", this, item);
}

//���һ����Ʒ

void Player::addItem(ItemStack* item) {
	SymCall<uintptr_t>("?addItem@@YAXAEAVPlayer@@AEAVItemStack@@@Z", this, item);
}

//���ӵȼ�

void Player::addLevel(int level) {
	SymCall("?addLevels@Player@@UEAAXH@Z", this, level);
}

//��ȡ��ǰѡ�еĿ�λ��

int Player::getSelectedItemSlot() {
	return SymCall<int>("?getSelectedItemSlot@Player@@QEBAHXZ", this);
	//return FETCH(unsigned, FETCH(uintptr_t, this + 3208) + 16);//IDA Player::getSelectedItemSlot
}

//��ȡ��ǰ��Ʒ

ItemStack* Player::getSelectedItem() {
	return SymCall<ItemStack*>("?getSelectedItem@Player@@QEBAAEBVItemStack@@XZ", this);
}

//��ȡ������Ʒ

ItemStack* Player::getInventoryItem(int slot) {
	return getInventory()->getSlots()[slot];
}

//��ȡ��Ϸʱ����Ȩ��

char Player::getPermissions() {
	return *FETCH(char*, this + 2376);//IDA ServerPlayer::setPermissions 22
}

//������Ϸʱ����Ȩ��

void Player::setPermissions(char m) {
	SymCall("?setPermissions@ServerPlayer@@UEAAXW4CommandPermissionLevel@@@Z",
		this, m);
}

//��ȡ��Ϸʱ����Ȩ��

char Player::getPermissionLevel() {//IDA Abilities::setPlayerPermissions ?
	return FETCH(char, FETCH(char*, this + 2376) + 1);
}

//������Ϸʱ����Ȩ��

void Player::setPermissionLevel(char m) {
	SymCall("?setPlayerPermissions@Abilities@@QEAAXW4PlayerPermissionLevel@@@Z",
		this + 2376, m);
}

//��ȡ�豸id

string Player::getDeviceId() {
	return FETCH(string, this + 8352); //IDA Player::Player  v13 + 8352
}

//��ȡ�豸ϵͳ����

int Player::getDeviceOS() {
	return FETCH(int, this + 2368);	//IDA ServerNetworkHandler::createNewPlayer  ConnectionRequest::getDeviceOS
}

//���ͱ���

void Player::sendInventroy() {
	SymCall("?sendInventory@ServerPlayer@@UEAAX_N@Z",
		this, true);
}

//ˢ������

void Player::resendAllChunks() {
	SymCall("?resendAllChunks@ServerPlayer@@UEAAXXZ", this);
}

//�������ݰ�

void Player::sendPacket(uintptr_t pkt) {
	SymCall("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z",
		this, pkt);
}

//ʹ��ҿͻ��˱���
bool Player::crash() {
	uintptr_t pkt = createPacket(58);
	FETCH(int, pkt + 14) = 0;
	FETCH(int, pkt + 15) = 0;
	FETCH(bool, pkt + 48) = 1;
	sendPacket(pkt);
	return true;
}

//ɱ��ʵ��
bool Actor::kill() {
	SymCall("?kill@Mob@@UEAAXXZ", this);
	return true;
}

unsigned Player::sendModalFormRequestPacket(const string& str) {
	static unsigned id = 0;
	uintptr_t pkt = createPacket(100);
	FETCH(unsigned, pkt + 48) = ++id;
	FETCH(string, pkt + 56) = str;
	sendPacket(pkt);
	return id;
}

void Player::sendTransferPacket(const string& address, unsigned short port) {
	uintptr_t pkt = createPacket(85);
	FETCH(string, pkt + 48) = address;
	FETCH(unsigned short, pkt + 80) = port;
	sendPacket(pkt);
}

void Player::sendDisconnectPacket(const string& msg) {
	uintptr_t pkt = createPacket(5);
	FETCH(string, pkt + 56) = msg;
	sendPacket(pkt);
}

void Player::sendTextPacket(int mode, const string& msg) {
	uintptr_t pkt = createPacket(9);
	FETCH(int, pkt + 48) = mode;
	FETCH(string, pkt + 56) = getNameTag();
	FETCH(string, pkt + 88) = msg;
	sendPacket(pkt);
}

void Player::sendCommandRequestPacket(const string& cmd) {
	uintptr_t pkt = createPacket(77);
	FETCH(string, pkt + 48) = cmd;
	SymCall<uintptr_t>("?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
		g_server_network_handler, getClientId(), pkt);
	//p->sendPacket(pkt);
}

void Player::sendBossEventCodePacket(string name, float per, int eventtype) {
	uintptr_t pkt = createPacket(74);
	FETCH(uintptr_t, pkt + 56) = FETCH(uintptr_t, getUniqueID());
	//FETCH(uintptr_t, pkt + 64) = FETCH(uintptr_t, getUniqueID());
	FETCH(int, pkt + 72) = eventtype;//0��ʾ,1����,2����,
	FETCH(string, pkt + 80) = name;
	FETCH(float, pkt + 112) = per;
	sendPacket(pkt);
}

void Player::sendsetDisplayObjectivePacket(const string& title, const string& name) {
	uintptr_t pkt = createPacket(107);
	FETCH(string, pkt + 48) = "sidebar";
	FETCH(string, pkt + 80) = name;
	FETCH(string, pkt + 112) = title;
	FETCH(string, pkt + 144) = "dummy";
	FETCH(char, pkt + 176) = 0;
	sendPacket(pkt);
}

void Player::sendSetScorePacket(char type, const vector<ScorePacketInfo>& slot) {
	uintptr_t pkt = createPacket(108);
	FETCH(char, pkt + 48) = type;//{set,remove}
	FETCH(vector<ScorePacketInfo>, pkt + 56) = slot;
	sendPacket(pkt);
}
