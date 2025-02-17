#pragma once
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "Position.h"

template <typename T>
struct SharePtrRefTraits {};
template <typename T>
class WeakStorageSharePtr {
public:
	std::weak_ptr<T> mHandle;
};
template <typename T>
class WeakRefT : public WeakStorageSharePtr<T> {};
struct Actor;
struct Player;
struct BlockSource;
struct Scoreboard;
struct BlockPalette;
struct ItemActor;
struct ItemStack;
struct SPSCQueue;
struct Spawner {
	ItemActor* spawnItem(BlockSource* bs, ItemStack* item, Vec3* pos);
};

struct Level {
	BlockSource* getBlockSource(int did);
	void forEachPlayer(const std::function<bool(Player*)>&);
	Scoreboard* getScoreBoard();
	unsigned getSeed();
	Actor* fetchEntity(uintptr_t id);
	Player* getPlayerByXuid(const std::string& xuid);
	BlockPalette* getBlockPalette();
	Spawner* getSpawner();
};
