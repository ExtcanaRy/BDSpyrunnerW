#include "Actor.h"
#include "Tag.h"
#include "ItemStack.h"
//��ȡ����������Ϣ

inline string Actor::getNameTag() {
	return SymCall<string&>("?getNameTag@Actor@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ", this);
}

//��ȡ���ﵱǰ����ά��ID

inline int Actor::getDimensionId() {
	int did;
	SymCall("?getDimensionId@Actor@@UEBA?AV?$AutomaticID@VDimension@@H@@XZ",
		this, &did);
	return did;
	//return FETCH(int, this + 236);//IDA Actor::getDimensionId
}

//��ȡ���ﵱǰ��������

inline Vec3* Actor::getPos() {
	return SymCall<Vec3*>("?getPos@Actor@@UEBAAEBVVec3@@XZ", this);
	//return (Vec3*)(this + 1268);//IDA Actor::getPos
}

//��ȡ����֮ǰ��������

inline Vec3* Actor::getPosOld() {
	return SymCall<Vec3*>("?getPosOld@Actor@@UEBAAEBVVec3@@XZ", this);
	//return (Vec3*)(this + 1280);//IDA Actor::getPosOld
}

//�Ƿ����Ƴ�

inline bool Actor::isRemoved() {
	return FETCH(bool, this + 7688);
}

//�Ƿ�����

inline bool Actor::isStand() {//IDA MovePlayerPacket::MovePlayerPacket 30
	return FETCH(bool, this + 480);
}


//ȡ����Դ

inline BlockSource* Actor::getRegion() {
	return SymCall<BlockSource*>("?getRegion@Actor@@QEBAAEAVBlockSource@@XZ", this);
	//return FETCH(BlockSource*, this + 872);//IDA Actor::getRegion
}

inline ItemStack* Actor::getArmor(int slot) {
	return SymCall<ItemStack*>("?getArmor@Actor@@UEBAAEBVItemStack@@W4ArmorSlot@@@Z",
		this, slot);
}

//��ȡʵ������

inline unsigned Actor::getEntityTypeId() {
	return VirtualCall<unsigned>(0x520, this);
	//return SymCall<unsigned>("?getEntityTypeId@Actor@@UEBA?AW4ActorType@@XZ", this);
}

//��ȡ��ѯ��ID

inline VA Actor::getUniqueID() {
	return SymCall<VA>("?getUniqueID@Actor@@QEBAAEBUActorUniqueID@@XZ", this);
}

//��ȡʵ������

inline string Actor::getEntityTypeName() {
	string type;
	SymCall<string&>("?EntityTypeToString@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@W4ActorType@@W4ActorTypeNamespaceRules@@@Z",
		&type, getEntityTypeId());
	return type;
}

//��������

inline VA Actor::updateAttrs() {
	return SymCall<VA>("?_sendDirtyActorData@Actor@@QEAAXXZ", this);
}

//��ȡ��ͼ��Ϣ

inline Level* Actor::getLevel() {
	return SymCall<Level*>("?getLevel@Actor@@QEBAAEBVLevel@@XZ", this);
}

//���һ��״̬

inline VA Actor::addEffect(VA ef) {
	return SymCall<VA>("?addEffect@Actor@@QEAAXAEBVMobEffectInstance@@@Z", this, ef);
}

//��ȡ����ֵ

inline int Actor::getHealth() {
	return SymCall<int>("?getHealth@Actor@@QEBAHXZ", this);
}

inline int Actor::getMaxHealth() {
	return SymCall<int>("?getMaxHealth@Actor@@QEBAHXZ", this);
}

inline void Actor::setHealth(int value) {
	VA hattr = (*reinterpret_cast<VA(**)(Actor*, void*)>(*(VA*)this + 1552))
		(this, SYM("?HEALTH@SharedAttributes@@2VAttribute@@B"));
	FETCH(int, hattr + 132) = value;
	//SymCall("?_setDirty@AttributeInstance@@AEAAXXZ", hattr);
}

inline void Actor::setMaxHealth(int value) {
	VA hattr = (*reinterpret_cast<VA(**)(Actor*, void*)>(*(VA*)this + 1552))
		(this, SYM("?HEALTH@SharedAttributes@@2VAttribute@@B"));
	FETCH(int, hattr + 128) = value;
	//SymCall("?_setDirty@AttributeInstance@@AEAAXXZ", hattr);
}

//��ȡ����

inline ItemStack* Actor::getOffHand() {
	return SymCall<ItemStack*>("?getOffhandSlot@Actor@@QEBAAEBVItemStack@@XZ", this);
}

inline Tag* Actor::save() {
	Tag* t = newTag(TagType::Compound);
	SymCall("?save@Actor@@UEAA_NAEAVCompoundTag@@@Z", this, t);
	return t;
}

//���ô�С

inline void Actor::setSize(float f1, float f2) {
	SymCall("?setSize@Actor@@UEAAXMM@Z", this, f1, f2);
}

//�Ƿ�Ǳ��

inline bool Actor::isSneaking() {
	return SymCall<bool>("?isSneaking@Actor@@QEBA_NXZ", this);
}

//��ȡ״̬�б�

inline auto Actor::getAllEffects() {
	//return SymCall<vector<MobEffectInstance>*>("?getAllEffects@Actor@@QEBAAEBV?$vector@VMobEffectInstance@@V?$allocator@VMobEffectInstance@@@std@@@std@@XZ", this);
}

//����

inline void Actor::teleport(Vec3* target, int did) {
	char mem[128];
	SymCall("?computeTarget@TeleportCommand@@SA?AVTeleportTarget@@AEAVActor@@VVec3@@PEAV4@V?$AutomaticID@VDimension@@H@@VRelativeFloat@@4H@Z",
		&mem, this, target, 0, did, 0, 0, 15);
	SymCall("?applyTarget@TeleportCommand@@SAXAEAVActor@@VTeleportTarget@@@Z",
		this, &mem);
}

//������ǩ

inline bool Actor::addTag(const string& str) {
	return SymCall<bool>("?addTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

//�Ƴ���ǩ

inline bool Actor::removeTag(const string& str) {
	return SymCall<bool>("?removeTag@Actor@@QEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

//��ȡ��ǩ

inline span<string> Actor::getTags() {
	span<string> tags;
	SymCall<span<string>&>("?getTags@Actor@@QEBA?BV?$span@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0?0@gsl@@XZ",
		this, &tags);
	return tags;
}

inline string Player::getUuid() {//IDA ServerNetworkHandler::_createNewPlayer 222
	string p;
	SymCall<string&>("?asString@UUID@mce@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this + 3000, &p);
	return p;
}

//�������ݰ�

inline void Player::sendPacket(VA pkt) {
	SymCall("?sendNetworkPacket@ServerPlayer@@UEBAXAEAVPacket@@@Z",
		this, pkt);
}

//���ݵ�ͼ��Ϣ��ȡ���xuid

inline string& Player::getXuid() {
	return SymCall<string&>("?getPlayerXUID@Level@@UEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVUUID@mce@@@Z",
		getLevel(), this + 3000);
}

//��������������

inline void Player::setName(const string& name) {
	SymCall("?setName@Player@@UEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &name);
}

//��ȡ�����ʶ��

inline NetworkIdentifier* Player::getClientId() {
	return SymCall<NetworkIdentifier*>("?getClientId@Player@@QEBAAEBVNetworkIdentifier@@XZ",
		this);
	//IDA ServerPlayer::setPermissions 34
}

//��ȡ����

inline Container* Player::getInventory() {
	return SymCall<Container*>("?getInventory@Player@@QEAAAEAVContainer@@XZ", this);
	//return FETCH(Container*, FETCH(VA, this + 3208) + 176);//IDA Player::getInventory
}

//��ȡװ������

inline Container* Player::getArmorContainer() {
	return FETCH(Container*, this + 1648);//IDA Actor::_setArmorContainer 11
}

//��ȡĩӰ��

inline Container* Player::getEnderChestContainer() {
	return FETCH(Container*, this + 4360);//IDA ReplaceItemCommand::execute 1086 
}

//����һ��װ��

inline VA Player::setArmor(int i, ItemStack* item) {
	return SymCall<VA>("?setArmor@ServerPlayer@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z", this, i, item);
}

//���ø���

inline VA Player::setOffhandSlot(ItemStack* item) {
	return SymCall<VA>("?setOffhandSlot@Player@@UEAAXAEBVItemStack@@@Z", this, item);
}

//���һ����Ʒ

inline void Player::addItem(ItemStack* item) {
	SymCall<VA>("?addItem@@YAXAEAVPlayer@@AEAVItemStack@@@Z", this, item);
}

//���ӵȼ�

inline void Player::addLevel(int level) {
	SymCall("?addLevels@Player@@UEAAXH@Z", this, level);
}

//��ȡ��ǰѡ�еĿ�λ��

inline int Player::getSelectedItemSlot() {
	return SymCall<int>("?getSelectedItemSlot@Player@@QEBAHXZ", this);
	//return FETCH(unsigned, FETCH(VA, this + 3208) + 16);//IDA Player::getSelectedItemSlot
}

//��ȡ��ǰ��Ʒ

inline ItemStack* Player::getSelectedItem() {
	return SymCall<ItemStack*>("?getSelectedItem@Player@@QEBAAEBVItemStack@@XZ", this);
}

//��ȡ������Ʒ

inline ItemStack* Player::getInventoryItem(int slot) {
	return getInventory()->getSlots()[slot];
}

//��ȡ��Ϸʱ����Ȩ��

inline char Player::getPermissions() {
	return *FETCH(char*, this + 2376);//IDA ServerPlayer::setPermissions 22
}

//������Ϸʱ����Ȩ��

inline void Player::setPermissions(char m) {
	SymCall("?setPermissions@ServerPlayer@@UEAAXW4CommandPermissionLevel@@@Z",
		this, m);
}

//��ȡ��Ϸʱ����Ȩ��

inline char Player::getPermissionLevel() {//IDA Abilities::setPlayerPermissions ?
	return FETCH(char, FETCH(char*, this + 2376) + 1);
}

//������Ϸʱ����Ȩ��

inline void Player::setPermissionLevel(char m) {
	SymCall("?setPlayerPermissions@Abilities@@QEAAXW4PlayerPermissionLevel@@@Z",
		this + 2376, m);
}

//��ȡ�豸id

inline string Player::getDeviceId() {
	return FETCH(string, this + 8352); //IDA Player::Player  v13 + 8352
}

//��ȡ�豸ϵͳ����

inline int Player::getDeviceOS() {
	return FETCH(int, this + 2368);	//IDA ServerNetworkHandler::createNewPlayer  ConnectionRequest::getDeviceOS
}

//���ͱ���

inline void Player::sendInventroy() {
	SymCall("?sendInventory@ServerPlayer@@UEAAX_N@Z",
		this, true);
}

//ˢ������

inline void Player::resendAllChunks() {
	SymCall("?resendAllChunks@ServerPlayer@@UEAAXXZ", this);
}
