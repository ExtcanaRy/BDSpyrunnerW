#include "ScoreBoard.h"

//��ȡ�Ʒְ�����

inline string Objective::getScoreName() {
	return FETCH(string, this + 64);
}

//��ȡ�Ʒְ�չʾ����

inline string Objective::getScoreDisplayName() {
	return FETCH(string, this + 96);
}

inline auto Objective::createScoreboardId(Player* player) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBVPlayer@@@Z", this, player);
}

inline ScoreInfo* Objective::getPlayerScore(ScoreboardId* a2) {
	char a1[12];
	return SymCall<ScoreInfo*>("?getPlayerScore@Objective@@QEBA?AUScoreInfo@@AEBUScoreboardId@@@Z",
		this, a1, a2);
}

inline Objective* Scoreboard::getObjective(string str) {
	return SymCall<Objective*>("?getObjective@Scoreboard@@QEBAPEAVObjective@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", this, &str);
}

inline ScoreboardId* Scoreboard::getScoreboardId(const string& str) {
	return SymCall<ScoreboardId*>("?getScoreboardId@Scoreboard@@QEBAAEBUScoreboardId@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

inline vector<Objective*> Scoreboard::getObjectives() {
	vector<Objective*> s;
	SymCall("?getObjectives@Scoreboard@@QEBA?AV?$vector@PEBVObjective@@V?$allocator@PEBVObjective@@@std@@@std@@XZ",
		this, &s);
	return s;
}

inline auto Scoreboard::getDisplayInfoFiltered(string* str) {
	return SymCall<vector<PlayerScore>*>("?getDisplayInfoFiltered@Scoreboard@@QEBA?AV?$vector@UPlayerScore@@V?$allocator@UPlayerScore@@@std@@@std@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@@Z", this, str);
}

inline auto Scoreboard::getTrackedIds() {
	return SymCall<vector<ScoreboardId>*>("?getTrackedIds@Scoreboard@@QEBA?AV?$vector@UScoreboardId@@V?$allocator@UScoreboardId@@@std@@@std@@XZ", this);
}

inline ScoreboardId* Scoreboard::getScoreboardId(Player* a2) {
	return SymCall<ScoreboardId*>("?getScoreboardId@Scoreboard@@QEBAAEBUScoreboardId@@AEBVActor@@@Z", this, a2);
}

//������ҷ���

inline int Scoreboard::modifyPlayerScore(ScoreboardId* a3, Objective* a4, int count, int mode) {
	bool a2 = true;
	return SymCall<int>("?modifyPlayerScore@Scoreboard@@QEAAHAEA_NAEBUScoreboardId@@AEAVObjective@@HW4PlayerScoreSetFunction@@@Z",
		this, &a2, a3, a4, count, mode);
}

inline ScoreboardId* Scoreboard::createScoreBoardId(const string& s) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &s);
}

inline ScoreboardId* Scoreboard::createScoreBoardId(Player* player) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBVPlayer@@@Z", this, player);
}

inline ScorePacketInfo::ScorePacketInfo(ScoreboardId* s, unsigned num, const string& fake) :
	sid(*s), score(num), fake_name(fake) {
}

inline int ScoreInfo::getCount() {
	return FETCH(int, this + 12);
}

inline VA PlayerScore::getScore() {
	return FETCH(VA, this + 4);
}
