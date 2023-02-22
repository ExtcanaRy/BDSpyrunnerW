#include <mc/ScoreBoard.h>
#include <mc/tool.h>

using namespace std;

string Objective::getScoreName() {
	return Dereference<string>( this + 64);
}

string Objective::getScoreDisplayName() {
	return Dereference<string>( this + 96);
}

auto Objective::createScoreboardId(Player* player) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBVPlayer@@@Z", this, player);
}

ScoreInfo* Objective::getPlayerScore(ScoreboardId* a2) {
	char a1[40];
	return SymCall<ScoreInfo*>("?getPlayerScore@Objective@@QEBA?AUScoreInfo@@AEBUScoreboardId@@@Z",
		this, a1, a2);
}

Objective* Scoreboard::getObjective(const string& str) {
	return SymCall<Objective*>("?getObjective@Scoreboard@@QEBAPEAVObjective@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z", this, &str);
}

ScoreboardId* Scoreboard::getScoreboardId(const string& str) {
	return SymCall<ScoreboardId*>("?getScoreboardId@Scoreboard@@QEBAAEBUScoreboardId@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &str);
}

vector<Objective*> Scoreboard::getObjectives() {
	vector<Objective*> s;
	SymCall("?getObjectives@Scoreboard@@QEBA?AV?$vector@PEBVObjective@@V?$allocator@PEBVObjective@@@std@@@std@@XZ",
		this, &s);
	return s;
}

auto Scoreboard::getDisplayInfoFiltered(const string& str) {
	return SymCall<vector<PlayerScore>*>("?getDisplayInfoFiltered@Scoreboard@@QEBA?AV?$vector@UPlayerScore@@V?$allocator@UPlayerScore@@@std@@@std@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@3@@Z",
		this, &str);
}

auto Scoreboard::getTrackedIds() {
	return SymCall<vector<ScoreboardId>*>("?getTrackedIds@Scoreboard@@QEBA?AV?$vector@UScoreboardId@@V?$allocator@UScoreboardId@@@std@@@std@@XZ", this);
}

ScoreboardId* Scoreboard::getScoreboardId(Player* a2) {
	return SymCall<ScoreboardId*>("?getScoreboardId@Scoreboard@@QEBAAEBUScoreboardId@@AEBVActor@@@Z", this, a2);
}

int Scoreboard::modifyPlayerScore(ScoreboardId* a3, Objective* a4, int count, int mode) {
	bool a2 = true;
	return SymCall<int>("?modifyPlayerScore@Scoreboard@@QEAAHAEA_NAEBUScoreboardId@@AEAVObjective@@HW4PlayerScoreSetFunction@@@Z",
		this, &a2, a3, a4, count, mode);
}

ScoreboardId* Scoreboard::createScoreBoardId(const string& s) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, &s);
}

ScoreboardId* Scoreboard::createScoreBoardId(Player* player) {
	return SymCall<ScoreboardId*>("?createScoreboardId@ServerScoreboard@@UEAAAEBUScoreboardId@@AEBVPlayer@@@Z", this, player);
}

ScorePacketInfo::ScorePacketInfo(ScoreboardId* s, unsigned num, const string& fake) :
	sid(*s), score(num), fake_name(fake) {
}

int ScoreInfo::getCount() {
	return Dereference<int>( this + 12);
}

uintptr_t PlayerScore::getScore() {
	return Dereference<uintptr_t>( this + 4);
}
