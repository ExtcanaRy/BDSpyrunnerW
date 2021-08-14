#pragma once
#include "../tool.h"

struct Actor;
struct Player;
struct BlockSource;
struct Scoreboard;
struct BlockPalette;
struct SPSCQueue;
struct Level {
	//��ȡ����Դ û���ά�ȷ��ؿ�ָ��
	BlockSource* getBlockSource(int did);
	Scoreboard* getScoreBoard();
	unsigned getSeed();
	string getPlayerNames();
	Actor* fetchEntity(VA id);
	Player* getPlayerByXuid(const string& xuid);
	vector<Player*> getAllPlayers();
	BlockPalette* getBlockPalette();
};
