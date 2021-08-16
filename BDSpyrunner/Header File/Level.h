#pragma once
#include <vector>
#include <string>
#include <tool.h>
#include "Position.h"

struct Actor;
struct Player;
struct BlockSource;
struct Scoreboard;
struct BlockPalette;
struct SPSCQueue;
struct ItemActor;
struct ItemStack;
struct Spawner {
	ItemActor* spawnItem(BlockSource* bs, ItemStack* item, Vec3* pos);
};

struct Level {
	//��ȡ����Դ û���ά�ȷ��ؿ�ָ��
	BlockSource* getBlockSource(int did);
	Scoreboard* getScoreBoard();
	unsigned getSeed();
	std::string getPlayerNames();
	Actor* fetchEntity(VA id);
	Player* getPlayerByXuid(const std::string& xuid);
	std::vector<Player*> getAllPlayers();
	BlockPalette* getBlockPalette();
	Spawner* getSpawner();
};
