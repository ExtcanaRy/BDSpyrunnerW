#include <mc/Level.h>
#include <mc/tool.h>

using namespace std;

BlockSource* Level::getBlockSource(int did) {
	void* result = malloc(0x18);
	void* d = SymCall<class WeakRefT<struct SharePtrRefTraits<class Dimension>> *>("?getDimension@Level@@UEBA?AV?$WeakRefT@U?$SharePtrRefTraits@VDimension@@@@@@V?$AutomaticID@VDimension@@H@@@Z",
		this, result, did)->mHandle.lock().get();
	if (!d) {
		free(result);
		return nullptr;
	}
	auto ret = SymCall<BlockSource*>("?getBlockSourceFromMainChunkSource@Dimension@@QEBAAEAVBlockSource@@XZ", d);
	free(result);
	return ret;
}

void Level::forEachPlayer(const std::function<bool(Player*)>& fn) {
	SymCall("?forEachPlayer@Level@@UEBAXV?$function@$$A6A_NAEBVPlayer@@@Z@std@@@Z",
		this, &fn);
}

Scoreboard* Level::getScoreBoard() {
	return Dereference<Scoreboard*>( this + 8600);//IDA Level::getScoreboard
}

unsigned Level::getSeed() {
	return SymCall<unsigned>("?getSeed@Level@@UEAAIXZ", this);
}

string Level::getPlayerNames() {
	string s;
	SymCall<string&>("?getPlayerNames@Level@@UEAA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this, &s);
	return s;
}

Actor* Level::fetchEntity(uintptr_t id) {
	return SymCall<Actor*>("?fetchEntity@Level@@UEBAPEAVActor@@UActorUniqueID@@_N@Z",
		this, id, false);
}

Player* Level::getPlayerByXuid(const string& xuid) {
	return SymCall<Player*>("?getPlayerByXuid@Level@@UEBAPEAVPlayer@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &xuid);
}

BlockPalette* Level::getBlockPalette() {
	return Dereference<BlockPalette*>( this + 2120);
}

Spawner* Level::getSpawner() {
	return SymCall<Spawner*>("?getSpawner@Level@@UEBAAEAVSpawner@@XZ",
		this);
}

ItemActor* Spawner::spawnItem(BlockSource* bs, ItemStack* item, Vec3* pos) {
	return SymCall<ItemActor*>("?spawnItem@Spawner@@QEAAPEAVItemActor@@AEAVBlockSource@@AEBVItemStack@@PEAVActor@@AEBVVec3@@H@Z",
		this, bs, item, nullptr, pos, 0);
}
