//Main.cpp plugin module
#include <mc/Actor.h>
#include <mc/Block.h>
#include <mc/ItemStack.h>
#include <mc/Level.h>
#include <mc/NetWork.h>
#include <mc/ScoreBoard.h>
#include <mc/Structure.h>
#include <mc/Tag.h>
#include <mc/tool.h>
#include <mc/InventoryTransaction.h>

#include <pyrw/Entity.h>
#include <pyrw/Event.h>
#include <pyrw/Module.h>
#include <pyrw/Logger.h>
#include <pyrw/Version.h>

Logger logger("BDSpyrunnerW");

bool load_plugin(void);
bool init_hooks(void);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
		if (!hooker_init())
			break;
        load_plugin();
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        hooker_uninit();
        break;
    }
    return true;
}


#pragma region Function
// request GIL on create
class EventCallBackHelper {
public:
	EventCallBackHelper(EventCode t) :
		type_(t), arg_(nullptr), gil_(PyGILState_Ensure()) {
	}
	~EventCallBackHelper() {
		if (arg_) {
			Py_XDECREF(arg_);
		}
		PyGILState_Release(gil_);
	}
	// Event callback
	bool call() {
		bool intercept = true;
		// if null -> skip
		auto& cbs = g_callback_functions[type_];
		//Py_BEGIN_CALL;
		//Py_XINCREF(arg_);
		for (auto cb : cbs) {
			PyObject* result = PyObject_CallFunction(cb, "O", arg_);
			PrintPythonError();
			if (result == Py_False)
				intercept = false;
			Py_XDECREF(result);
		}
		//Py_END_CALL;
		return intercept;
	}
	EventCallBackHelper& setArg(PyObject* arg) {
		arg_ = arg;
		return *this;
	}
	EventCallBackHelper& setDict() {
		arg_ = PyDict_New();
		return *this;
	}
	EventCallBackHelper& insert(std::string_view key, PyObject* item) {
		if (arg_ == nullptr)
			arg_ = PyDict_New();
		PyDict_SetItemString(arg_, key.data(), item);
		Py_DECREF(item);
		//Py_PRINT_REFCOUNT(item);
		return *this;
	}
	EventCallBackHelper& insert(std::string_view key, std::string_view item) {
		return insert(key, ToPyStr(item));
	}
	EventCallBackHelper& insert(std::string_view key, Actor* item) {
		return insert(key, ToEntity(item));
	}
	EventCallBackHelper& insert(std::string_view key, BlockPos* item) {
		return insert(key, ToList(item));
	}
	EventCallBackHelper& insert(std::string_view key, Vec3* item) {
		return insert(key, ToList(item));
	}
	EventCallBackHelper& insert(std::string_view key, short item) {
		return insert(key, PyLong_FromLong(item));
	}
	EventCallBackHelper& insert(std::string_view key, int item) {
		return insert(key, PyLong_FromLong(item));
	}
	EventCallBackHelper& insert(std::string_view key, unsigned item) {
		return insert(key, PyLong_FromUnsignedLong(item));
	}
	EventCallBackHelper& insert(std::string_view key, float item) {
		return insert(key, PyLong_FromDouble(item));
	}
	EventCallBackHelper& insert(std::string_view key, void* item) {
    	return insert(key, PyLong_FromVoidPtr(item));
	}
private:
	EventCode type_;
	PyObject* arg_;
	PyGILState_STATE gil_;
};
#pragma endregion
#pragma region Hook List
TMHOOK(DedicatedServer_start, int, "?start@DedicatedServer@@QEAA?AW4StartResult@1@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEBVActivationArguments@Bedrock@@@Z",
	std::string &a1, uintptr_t a2) {
	InitPythonInterpreter();
	// logout version info
	logger.info(PYR_VERSION + std::string(" loaded."));
	return DedicatedServer_start.original(a1, a2);
}
// Constructor for Level		
TMHOOK(Level_construct, Level*, "??0Level@@QEAA@AEBV?$not_null@V?$NonOwnerPointer@VSoundPlayerInterface@@@Bedrock@@@gsl@@V?$unique_ptr@VLevelStorage@@U?$default_delete@VLevelStorage@@@std@@@std@@V?$unique_ptr@VLevelLooseFileStorage@@U?$default_delete@VLevelLooseFileStorage@@@std@@@4@AEAVIMinecraftEventing@@_NW4SubClientId@@AEAVScheduler@@V?$not_null@V?$NonOwnerPointer@VStructureManager@@@Bedrock@@@2@AEAVResourcePackManager@@AEBV?$not_null@V?$NonOwnerPointer@VIEntityRegistryOwner@@@Bedrock@@@2@V?$WeakRefT@UEntityRefTraits@@@@V?$unique_ptr@VBlockComponentFactory@@U?$default_delete@VBlockComponentFactory@@@std@@@4@V?$unique_ptr@VBlockDefinitionGroup@@U?$default_delete@VBlockDefinitionGroup@@@std@@@4@VItemRegistryRef@@V?$weak_ptr@VBlockTypeRegistry@@@4@4V?$optional@VDimensionDefinitionGroup@@@4@@Z",
	Level* _this, uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4, uintptr_t a5, uintptr_t a6, uintptr_t a7, uintptr_t a8, uintptr_t a9, uintptr_t a10, uintptr_t a11, uintptr_t a12, uintptr_t a13, uintptr_t a14, uintptr_t a15, uintptr_t a16, uintptr_t a17) {
	return global<Level> = Level_construct.original(_this, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17);
}
// Constructor for SPSCQueue
TMHOOK(SPSCQueue_construct, SPSCQueue*, "??0?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@QEAA@_K@Z",
	SPSCQueue* _this) {
	return global<SPSCQueue> = SPSCQueue_construct.original(_this);
}
// Constructor for RakPeer
TMHOOK(RakPeer_construct, RakPeer*, "??0RakPeer@RakNet@@QEAA@XZ",
	RakPeer* _this) {
	// will be constructed twice, taking the first value
	if (global<RakPeer> == nullptr) {
		global<RakPeer> = RakPeer_construct.original(_this);
		return global<RakPeer>;
	}
	return RakPeer_construct.original(_this);
}
// Constructor for ServerNetworkHandler
TMHOOK(ServerNetworkHandler_construct, uintptr_t, "??0ServerNetworkHandler@@QEAA@AEAVGameCallbacks@@AEBV?$NonOwnerPointer@VILevel@@@Bedrock@@AEAVNetworkSystem@@AEAVPrivateKeyManager@@AEAVServerLocator@@AEAVPacketSender@@AEAVAllowList@@PEAVPermissionsFile@@AEBVUUID@mce@@H_NAEBV?$vector@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@V?$allocator@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@2@@std@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@HAEAVMinecraftCommands@@AEAVIMinecraftApp@@AEBV?$unordered_map@UPackIdVersion@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@U?$hash@UPackIdVersion@@@3@U?$equal_to@UPackIdVersion@@@3@V?$allocator@U?$pair@$$CBUPackIdVersion@@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@std@@@3@@std@@AEAVScheduler@@V?$NonOwnerPointer@VTextFilteringProcessor@@@3@V?$optional_ref@VMinecraftGameTest@@@@V?$ServiceReference@VAppConfigs@@@@V?$ServiceReference@VMultiplayerServiceManager@Social@@@@9@Z",
	ServerNetworkHandler* _this, uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4, uintptr_t a5, uintptr_t a6, uintptr_t a7, uintptr_t a8, uintptr_t a9, uintptr_t a10, uintptr_t a11, uintptr_t a12, uintptr_t a13, uintptr_t a14, uintptr_t a15, uintptr_t a16, uintptr_t a17, uintptr_t a18, uintptr_t a19, uintptr_t a20, uintptr_t a21, uintptr_t a22, uintptr_t a23) {
	global<ServerNetworkHandler> = _this;
	return ServerNetworkHandler_construct.original(_this, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15, a16, a17, a18, a19, a20, a21, a22, a23);
}
// Constructor for Scoreboard
TMHOOK(ServerScoreboard_construct, Scoreboard*, "??0ServerScoreboard@@QEAA@VCommandSoftEnumRegistry@@PEAVLevelStorage@@@Z",
	Scoreboard* _this, uintptr_t a1, uintptr_t a2) {
	return global<Scoreboard> = ServerScoreboard_construct.original(_this, a1, a2);
}
//  Constructor for Player
TMHOOK(Player_construct, Player*, "??0Player@@QEAA@AEAVLevel@@AEAVPacketSender@@W4GameType@@AEBVNetworkIdentifier@@W4SubClientId@@VUUID@mce@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@6V?$unique_ptr@VCertificate@@U?$default_delete@VCertificate@@@std@@@9@AEAVEntityContext@@66@Z",
	void* _this, void* arg1, void* arg2, void* arg3, void* arg4, void* arg5, void* arg6, void* arg7, void* arg8, void* arg9, void* arg10, void* arg11, void* arg12) {
	// will be constructed twice, taking the first value
	if (global<std::vector<Player*>> == nullptr)
		global<std::vector<Player*>> = new std::vector<Player*>;
	auto ret = Player_construct.original(_this, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11, arg12);
	global<std::vector<Player*>>->push_back(ret);
	return ret;
}
// Destructor for Player
TMHOOK(Player_destruct, Player*, "??1Player@@UEAA@XZ", Player* _this) {
	for (std::vector<Player*>::iterator iter = global<std::vector<Player*>>->begin();
		iter != global<std::vector<Player*>>->end(); iter++) {
		if (*iter == _this) {
			global<std::vector<Player*>>->erase(iter);
			break;
		}
	}
	return Player_destruct.original(_this);
}
// Change the establishment of the setup command for the registration command
TMHOOK(ChangeSettingCommand_setup, void, "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z",
	uintptr_t _this) {
	for (auto& [cmd, des] : g_commands) {
		SymCall("?registerCommand@CommandRegistry@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@PEBDW4CommandPermissionLevel@@UCommandFlag@@3@Z",
			_this, &cmd, des.first.c_str(), 0, 0, 0x80/*CommandFlag*/);
	}
	ChangeSettingCommand_setup.original(_this);
}
#pragma endregion
#pragma region Listener
TMHOOK(onServerStarted, void, "?startServerThread@ServerInstance@@QEAAXXZ",
	uintptr_t _this) {
	EventCallBackHelper h(EventCode::onServerStarted);
	h.setArg(Py_None).call();
	onServerStarted.original(_this);
}
// Console output, which is actually the underlying call to ostrram::operator<<
TMHOOK(onConsoleOutput, std::ostream&, "??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z",
	std::ostream& _this, const char* str, uintptr_t size) {
	EventCallBackHelper h(EventCode::onConsoleOutput);
	if (&_this == &std::cout) {
		h.setArg(ToPyStr(str));
		if (!h.call())
			return _this;
	}
	return onConsoleOutput.original(_this, str, size);
}
// Parse command args
std::vector<std::string> parseCmdArgv(const std::string& cmd) {
	std::vector<std::string> cmdArgs;
	std::string arg;
	for (const char& c : cmd) {
		if (c == ' ') {
			cmdArgs.push_back(arg);
			arg.clear();
		}
		else {
			arg += c;
		}
	}
	cmdArgs.push_back(arg);
	return cmdArgs;
}
// Console input, which is actually the bottom of the command queue
TMHOOK(onConsoleInput, bool, "??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
	SPSCQueue* _this, std::string* cmd) {
	EventCallBackHelper h(EventCode::onConsoleInput);
	static bool debug = false;
	auto argv = parseCmdArgv(*cmd);
	if (argv[0] == "stop"){
		return onConsoleInput.original(_this, cmd);
	} else if (argv[0] == "pydebug") {
		if (debug) {
			debug = false;
		}
		else {
			debug = true;
			std::cout << ">>> ";
		}
		return false;
	} else if (argv[0] == "pyreload") {
		if (argv.size() == 1) {
			ReloadPythonModules("");
		}
		else if (argv.size() == 2) {
			LoadPythonModules(argv[1]);
		}
		return false;
	} else if (argv[0] == "pyreinit") {
		//Does not work fully and can cause crash
		InitPythonInterpreter();
		return false;
	}
	if (debug) {
		//Py_BEGIN_CALL;
		PyRun_SimpleString(cmd->c_str());
		//Py_END_CALL;
		std::cout << ">>> ";
		return false;
	}
	h.setArg(ToPyStr(*cmd));
	if (h.call())
		return onConsoleInput.original(_this, cmd);
	return false;
}
// Send packet on player pre-join
TMHOOK(onPreJoin, void, "?sendLoginMessageLocal@ServerNetworkHandler@@QEAAXAEBVNetworkIdentifier@@AEBVConnectionRequest@@AEAVServerPlayer@@@Z",
					   ServerNetworkHandler* _this, NetworkIdentifier* ni, void* a3, Player* sp) {
	EventCallBackHelper h(EventCode::onPreJoin);
	h.setArg(ToEntity(sp));
	h.call();
	onPreJoin.original(_this, ni, a3, sp);
}
// Send packet on player join
TMHOOK(onPlayerJoin, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVSetLocalPlayerAsInitializedPacket@@@Z",
	ServerNetworkHandler* _this, uintptr_t id,/*SetLocalPlayerAsInitializedPacket*/ uintptr_t pkt) {
	EventCallBackHelper h(EventCode::onPlayerJoin);
	Player* p = _this->_getServerPlayer(id, pkt);
	if (p) {
		h.setArg(ToEntity(p)).call();
	}
	onPlayerJoin.original(_this, id, pkt);
}
// on player left server
TMHOOK(onPlayerLeft, void, "?_onPlayerLeft@ServerNetworkHandler@@AEAAXPEAVServerPlayer@@_N@Z",
	uintptr_t _this, Player* p, char v3) {
	EventCallBackHelper h(EventCode::onPlayerLeft);
	h.setArg(ToEntity(p));
	h.call();
	return onPlayerLeft.original(_this, p, v3);
}

// Fix multiple right clicks on Win10 client
Vec3 lastPlayerPosition;
Vec3 lastClickPosition;
NetworkIdentifier* lastPlayerNetworkIdentifier = NULL;
__int64 lastUseItemTime = 0;
TMHOOK(filterInventoryTransaction, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVInventoryTransactionPacket@@@Z", uintptr_t _this, NetworkIdentifier* nid, InventoryTransactionPacket& packet) {
	// logger.debug(to_string(packet.transaction->transactionType), "AEBVInventoryTransactionPacket");
	if(packet.transaction->transactionType == 2) {
		if (lastPlayerNetworkIdentifier == nid && lastPlayerPosition == packet.transaction->playerPosition //&& lastClickPosition == packet.transaction->clickPosition
			&& (int)(lastClickPosition.x * 100) == (int)(packet.transaction->clickPosition.x * 100)
			&& (int)(lastClickPosition.z * 100) == (int)(packet.transaction->clickPosition.z * 100) ) {
			// logger.debug(to_string(getCurrentTime() - lastUseItemTime), "AEBVInventoryTransactionPacket");
			if((getCurrentTime() - lastUseItemTime) < 500) {
				return;
			}
			lastUseItemTime = getCurrentTime();
		} else if(packet.transaction->clickPosition.x != 0.0 || packet.transaction->clickPosition.y != 0.0 || packet.transaction->clickPosition.z != 0.0) {
			lastPlayerNetworkIdentifier = nid;
			lastPlayerPosition = packet.transaction->playerPosition;
			lastClickPosition = packet.transaction->clickPosition;
			lastUseItemTime = getCurrentTime();
		}
		// logger.debug(packet.transaction->playerPosition.toString(), "AEBVInventoryTransactionPacket");
		// logger.debug(packet.transaction->clickPosition.toString(), "AEBVInventoryTransactionPacket");
	}
	return filterInventoryTransaction.original(_this, nid, packet);
}

// use item
TMHOOK(onUseItem, bool, "?useItemOn@GameMode@@UEAA_NAEAVItemStack@@AEBVBlockPos@@EAEBVVec3@@PEBVBlock@@@Z",
	uintptr_t _this, ItemStack* item, BlockPos* bp, char a4, uintptr_t a5, Block* b) {
	EventCallBackHelper h(EventCode::onUseItem);
	Player* p = Dereference<Player*>(_this, 8);
	BlockLegacy* bl = b->getBlockLegacy();
	h
		.insert("player", p)
		.insert("itemid", item->getId())
		.insert("itemaux", item->getAuxValue())
		.insert("itemcount", item->getCount())
		.insert("itemname", item->getName())
		.insert("blockname", bl->getBlockName())
		.insert("blockid", bl->getBlockItemID())
		.insert("position", bp);
	if (h.call())
		return onUseItem.original(_this, item, bp, a4, a5, b);
	return false;
}
// before(try) use item
TMHOOK(onUseItemEx, bool, "?baseUseItem@GameMode@@QEAA_NAEAVItemStack@@@Z", 
	uintptr_t* _this, ItemStack* item) {
	EventCallBackHelper h(EventCode::onUseItemEx);
	Player* p = Dereference<Player*>(_this, 8);
	h
		.insert("player", p)
		.insert("itemid", item->getId())
		.insert("itemaux", item->getAuxValue())
		.insert("itemcount", item->getCount())
		.insert("itemname", item->getName());
	if (h.call())
		return onUseItemEx.original(_this, item);
	return false;
}
// Place item
TMHOOK(onPlaceBlock, bool, "?mayPlace@BlockSource@@QEAA_NAEBVBlock@@AEBVBlockPos@@EPEAVActor@@_NVVec3@@@Z",
	BlockSource* _this, Block* b, BlockPos* bp, unsigned char a4, Actor* p, char a6, Vec3 a7) {
	EventCallBackHelper h(EventCode::onPlaceBlock);
	if (IsPlayer(p)) {
		BlockLegacy* bl = b->getBlockLegacy();
		h
			.insert("player", p)
			.insert("blockname", bl->getBlockName())
			.insert("blockid", bl->getBlockItemID())
			.insert("position", bp);
		if (!h.call())
			return false;
	}
	return onPlaceBlock.original(_this, b, bp, a4, p, a6, a7);
}
// placed block
TMHOOK(onPlacedBlock, void, "?sendBlockPlacedByPlayer@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlock@@AEBVBlockPos@@_N@Z",
    uintptr_t _this, Player* p, Block* b, BlockPos* bp, bool _bool) {
	EventCallBackHelper h(EventCode::onPlacedBlock);
	if (IsPlayer(p)) {
		BlockLegacy* bl = b->getBlockLegacy();
		h
			.insert("player", p)
			.insert("blockname", bl->getBlockName())
			.insert("blockid", bl->getBlockItemID())
			.insert("position", bp);
		h.call();
	}
	onPlacedBlock.original(_this, p, b, bp, _bool);
}
// destroy block
TMHOOK(onDestroyBlock, bool, "?checkBlockDestroyPermissions@BlockSource@@QEAA_NAEAVActor@@AEBVBlockPos@@AEBVItemStackBase@@_N@Z",
	BlockSource* _this, Actor* a1, BlockPos* a2, ItemStack* a3, bool a4) {
	EventCallBackHelper h(EventCode::onDestroyBlock);
	if (IsPlayer(a1)) {
		BlockLegacy* bl = _this->getBlock(a2)->getBlockLegacy();
		h
			.insert("player", a1)
			.insert("blockname", bl->getBlockName())
			.insert("blockid", bl->getBlockItemID())
			.insert("position", a2);
		if (!h.call())
			return false;
	}
	return onDestroyBlock.original(_this, a1, a2, a3, a4);
}
// destroyed block
TMHOOK(onDestroyedBlock, void, "?sendBlockDestroyedByPlayer@BlockEventCoordinator@@QEAAXAEAVPlayer@@AEBVBlock@@AEBVBlockPos@@@Z",
    uintptr_t _this, Player* p, Block* b, BlockPos* bp) {
	EventCallBackHelper h(EventCode::onDestroyedBlock);
	if (IsPlayer(p)) {
		BlockLegacy* bl = b->getBlockLegacy();
		h
			.insert("player", p)
			.insert("blockname", bl->getBlockName())
			.insert("blockid", bl->getBlockItemID())
			.insert("position", bp);
		h.call();
	}
	onDestroyedBlock.original(_this, p, b, bp);
}
// open chest
TMHOOK(onOpenChest, bool, "?use@ChestBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
	uintptr_t _this, Player* p, BlockPos* bp) {
	EventCallBackHelper h(EventCode::onOpenChest);
	h
		.insert("player", p)
		.insert("position", bp);
	if (h.call())
		return onOpenChest.original(_this, p, bp);
	return false;
}
// Open barrel, can not intercept, intercept please see square interaction
TMHOOK(onOpenBarrel, bool, "?use@BarrelBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
	uintptr_t _this, Player* p, BlockPos* bp) {
	EventCallBackHelper h(EventCode::onOpenBarrel);
	h
		.insert("player", p)
		.insert("position", bp);
	if (h.call())
		return onOpenBarrel.original(_this, p, bp);
	return false;
}
// close chest
TMHOOK(onCloseChest, void, "?stopOpen@ChestBlockActor@@UEAAXAEAVPlayer@@@Z",
	uintptr_t _this, Player* p) {
	//p->setPermissions(static_cast<PlayerPermissionLevel>(0));
	EventCallBackHelper h(EventCode::onCloseChest);
	auto bp = (BlockPos*)(_this - 196);
	h
		.insert("player", p)
		.insert("position", bp);
	if(h.call())
		onCloseChest.original(_this, p);
}
// close barrel
TMHOOK(onCloseBarrel, void, "?stopOpen@BarrelBlockActor@@UEAAXAEAVPlayer@@@Z",
	uintptr_t _this, Player* p) {
	EventCallBackHelper h(EventCode::onCloseBarrel);
	auto bp = (BlockPos*)(_this - 196);
	h
		.insert("player", p)
		.insert("position", bp);
	if (h.call())
		onCloseBarrel.original(_this, p);
}
// Putting in and taking out items is actually a container change
TMHOOK(onContainerChange, void, "?containerContentChanged@LevelContainerModel@@UEAAXH@Z",
	uintptr_t _this, unsigned slot) {
	EventCallBackHelper h(EventCode::onContainerChange);
	//EventCallBackHelper h(EventCode::onTest);
	Player* p = Dereference<Player*>(_this, 208);//IDA LevelContainerModel::_getContainer line 15 25 v3
	BlockSource* bs = p->getRegion();
	BlockPos* bp = reinterpret_cast<BlockPos*>(_this + 216);
	BlockLegacy* bl = bs->getBlock(bp)->getBlockLegacy();
	short bid = bl->getBlockItemID();
	if (bid == 54 || bid == 130 || bid == 146 || bid == -203 || bid == 205 || bid == 218) {	//非箱子、桶、潜影盒的情况不作处理
		uintptr_t v5 = (*reinterpret_cast<uintptr_t(**)(uintptr_t)>(Dereference<uintptr_t>(_this) + 160))(_this);
		if (v5) {
			ItemStack* item = (*reinterpret_cast<ItemStack * (**)(uintptr_t, uintptr_t)>(Dereference<uintptr_t>(v5) + 40))(v5, slot);
			h
				.insert("player", p)
				.insert("itemid", item->getId())
				.insert("itemaux", item->getAuxValue())
				.insert("itemcount", item->getCount())
				.insert("itemname", item->getName())
				.insert("blockname", bl->getBlockName())
				.insert("blockid", bid)
				.insert("position", bp)
				.insert("slot", slot);
			h.call();
		}
	}
	onContainerChange.original(_this, slot);
}
// player inventory change
TMHOOK(onPlayerInventoryChange, void, "?containerContentChanged@HudContainerModel@@UEAAXH@Z",
	uintptr_t _this, unsigned slot) {
	EventCallBackHelper h(EventCode::onPlayerInventoryChange);
	Player* p = Dereference<Player*>(_this, 208);
	uintptr_t v5 = (*reinterpret_cast<uintptr_t(**)(uintptr_t)>(Dereference<uintptr_t>(_this) + 160))(_this);
	if (v5) {
		ItemStack* item = (*reinterpret_cast<ItemStack * (**)(uintptr_t, uintptr_t)>(Dereference<uintptr_t>(v5) + 40))(v5, slot);
		h
			.insert("player", p)
			.insert("itemid", item->getId())
			.insert("itemaux", item->getAuxValue())
			.insert("itemcount", item->getCount())
			.insert("itemname", item->getName())
			.insert("slot", slot);
		h.call();
	}
	onPlayerInventoryChange.original(_this, slot);
}
// player attack
float attackDamage = 0;
TMHOOK(onAttack, bool, "?attack@Player@@UEAA_NAEAVActor@@AEBW4ActorDamageCause@@@Z",
	Player* p, Actor* a, struct ActorDamageCause* c) {
	EventCallBackHelper h(EventCode::onPlayerAttack);
	attackDamage = SymCall<float>("?calculateAttackDamage@Actor@@QEAAMAEAV1@@Z", p, a);
	h
		.insert("player", p)
		.insert("actor", a)
		.insert("damage", &attackDamage);
	if (h.call())
		return onAttack.original(p, a, c);
	return false;
}
// calculate attack damage
TMHOOK(onCalcDamage, float, "?calculateAttackDamage@Actor@@QEAAMAEAV1@@Z",
	Actor* p, Actor* a) {
	if (attackDamage == 0) {
		return onCalcDamage.original(p, a);
	} else {
		float attackDamageTmp = attackDamage;
		attackDamage = 0;
		return attackDamageTmp;
	}
}
// player changed imension
TMHOOK(onChangeDimension, bool, "?requestPlayerChangeDimension@Level@@UEAAXAEAVPlayer@@V?$unique_ptr@VChangeDimensionRequest@@U?$default_delete@VChangeDimensionRequest@@@std@@@std@@@Z",
	uintptr_t _this, Player* p, uintptr_t req) {
	EventCallBackHelper h(EventCode::onChangeDimension);
	bool result = onChangeDimension.original(_this, p, req);
	if (result) {
		h.setArg(ToEntity(p)).call();
	}
	return result;
}
// mob die
TMHOOK(onMobDie, void, "?die@Mob@@UEAAXAEBVActorDamageSource@@@Z",
	Mob* _this, uintptr_t dmsg) {
	EventCallBackHelper h(EventCode::onMobDie);
	char v71[8];
	//IDA Mob::die Line142  v18 = (_QWORD *)(*(__int64 (__fastcall **)(const struct ActorDamageSource *, char *))(*(_QWORD *)a2 + 104i64))(a2, v71);
	Actor* sa = _this->getLevel()->fetchEntity(*(uintptr_t*)((*(uintptr_t(__fastcall**)(uintptr_t, char (*)[8]))(*(uintptr_t*)dmsg + 104))(dmsg, &v71)));
	h
		.insert("actor1", _this)
		.insert("actor2", sa)
		.insert("dmcase", Dereference<unsigned>(dmsg, 8))
		;
	if (h.call())
		onMobDie.original(_this, dmsg);
}
// mob hurt
TMHOOK(onMobHurt, bool, "?_hurt@Mob@@MEAA_NAEBVActorDamageSource@@M_N1@Z",
	Mob* _this, uintptr_t dmsg, float a3, bool a4, bool a5) {
	EventCallBackHelper h(EventCode::onMobHurt);
	g_damage = a3; // Set the value of Biological Injury to Adjustable
	char v71[8];
	// getSourceUniqueId
	//v21 = (_QWORD *)(*(__int64 (__fastcall **)(const struct ActorDamageSource *, char *))(*(_QWORD *)a2 + 104i64))(a2, v30);
	Actor* sa = _this->getLevel()->fetchEntity(*(uintptr_t*)((*(uintptr_t(__fastcall**)(uintptr_t, char (*)[8]))(*(uintptr_t*)dmsg + 104))(dmsg, &v71)));
	h
		.insert("actor1", _this)
		.insert("actor2", sa)
		.insert("dmcase", Dereference<unsigned>(dmsg, 8))
		.insert("damage", a3)
		;
	if (h.call())
		return onMobHurt.original(_this, dmsg, g_damage, a4, a5);
	return false;
}
// palyer respawn
TMHOOK(onRespawn, void, "?respawn@Player@@UEAAXXZ",
	Player* p) {
	EventCallBackHelper h(EventCode::onRespawn);
	h.setArg(ToEntity(p)).call();
	onRespawn.original(p);
}
// chat, title, msg, w...
TMHOOK(onChat, void, "?fireEventPlayerMessage@MinecraftEventing@@AEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@000@Z",
	uintptr_t _this, std::string* sender, std::string* target, std::string* msg, std::string* style) {
	EventCallBackHelper h(EventCode::onChat);
	
	if(sender && msg) {
		void* a1 = *(void**)sender;
		void* a2 = *(void**)msg;
		if(a1 && a2) {
			h.insert("sender", *sender)
				.insert("target", *target)
				.insert("msg", *msg)
				.insert("style", *style);
			h.call();
		}
	}
	onChat.original(_this, sender, target, msg, style);
}
// player send chat message
TMHOOK(onInputText, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVTextPacket@@@Z",
	ServerNetworkHandler* _this, uintptr_t id, /*TextPacket*/uintptr_t pkt) {
	EventCallBackHelper h(EventCode::onInputText);
	Player* p = _this->_getServerPlayer(id, pkt);
	if (p) {
		const std::string& msg = Dereference<std::string>(pkt, 88);
		h.insert("player", p)
			.insert("msg", msg);
		if (!h.call())
			return;
	}
	onInputText.original(_this, id, pkt);
}
// player execute command
TMHOOK(onInputCommand, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandRequestPacket@@@Z",
	ServerNetworkHandler* _this, uintptr_t id, /*CommandRequestPacket*/uintptr_t pkt) {
	EventCallBackHelper h(EventCode::onInputCommand);
	Player* p = _this->_getServerPlayer(id, pkt);
	if (p) {
		const std::string& cmd = Dereference<std::string>(pkt, 48);
		auto data = g_commands.find(cmd.c_str() + 1);
		// If this command is available and the callback function is not nullptr
		if (data != g_commands.end() && data->second.second) {
			//Py_BEGIN_CALL;
			PyObject_CallFunction(data->second.second, "O", ToEntity(p));
			PrintPythonError();
			//Py_END_CALL;
			return;
		}
		h.insert("player", p)
			.insert("cmd", cmd);
		if (h.call())
			onInputCommand.original(_this, id, pkt);
	}
}
// player select form
TMHOOK(onSelectForm, void, "?handle@?$PacketHandlerDispatcherInstance@VModalFormResponsePacket@@$0A@@@UEBAXAEBVNetworkIdentifier@@AEAVNetEventCallback@@AEAV?$shared_ptr@VPacket@@@std@@@Z",
	uintptr_t _this, uintptr_t id, ServerNetworkHandler* handle, uintptr_t* ppkt) {
	EventCallBackHelper h(EventCode::onSelectForm);
	uintptr_t pkt = *ppkt;
	Player* p = handle->_getServerPlayer(id, pkt);
	std::string data;
	int formid = Dereference<int>(pkt, 48);

	if (!Dereference<bool>(pkt, 81)) {
		if (Dereference<bool>(pkt, 72)) {
			//Json::value_type* json = Dereference<Json::value_type*>(pkt, 56);
			std::string dst;
			data = 
				SymCall<std::string&>("?toStyledString@Value@Json@@QEBA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
					pkt + 56, &dst);
		}
	}

	if (data.empty())
		data = "null";
	
	if (data.back() == '\n')
		data.pop_back();

	auto iter = g_forms.find(formid);
	if (iter != g_forms.end()) {
		//Py_BEGIN_CALL;
		PyObject* func = g_forms[formid];
		PyObject_CallFunction(func, "OO", ToEntity(p), PyUnicode_FromStringAndSize(data.c_str(), data.size()));
		PrintPythonError();
		//Py_END_CALL;
		g_forms.erase(iter);
		return onSelectForm.original(_this, id, handle, ppkt);
	}

	h
		.insert("player", p)
		.insert("selected", data)
		.insert("formid", formid);
	h.call();
	onSelectForm.original(_this, id, handle, ppkt);
}
// command block content update
TMHOOK(onCommandBlockUpdate, void, "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVCommandBlockUpdatePacket@@@Z",
	ServerNetworkHandler* _this, uintptr_t id, /*CommandBlockUpdatePacket*/uintptr_t pkt) {
	EventCallBackHelper h(EventCode::onCommandBlockUpdate);
	Player* p = _this->_getServerPlayer(id, pkt);
	if (p) {
		BlockPos bp = Dereference<BlockPos>(pkt, 48);
		unsigned short mode = Dereference<unsigned short>(pkt, 60);
		bool condition = Dereference<bool>(pkt, 62);
		bool redstone = Dereference<bool>(pkt, 63);
		std::string cmd = Dereference<std::string>(pkt, 72);
		std::string output = Dereference<std::string>(pkt, 104);
		std::string rawname = Dereference<std::string>(pkt, 136);
		int delay = Dereference<int>(pkt, 168);
		h.insert("player", ToEntity(p))
			.insert("mode", mode)
			.insert("condition", condition)
			.insert("redstone", redstone)
			.insert("cmd", cmd)
			.insert("output", output)
			.insert("rawname", rawname)
			.insert("delay", delay)
			.insert("position", &bp);
		if (!h.call())
			return;
	}
	onCommandBlockUpdate.original(_this, id, pkt);
}
// level block
TMHOOK(onLevelExplode, bool, "?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
	Level* _this, BlockSource* bs, Actor* a3, Vec3 pos, float a5, bool a6, bool a7, float a8, bool a9) {
	EventCallBackHelper h(EventCode::onLevelExplode);
	h
		.insert("actor", a3)
		.insert("position", &pos)
		.insert("dimensionid", bs->getDimensionId())
		.insert("power", a5);
	if (h.call())
		return onLevelExplode.original(_this, bs, a3, pos, a5, a6, a7, a8, a9);
	return false;
}
// command block execute
TMHOOK(onCommandBlockPerform, bool, "?_execute@CommandBlock@@AEBAXAEAVBlockSource@@AEAVCommandBlockActor@@AEBVBlockPos@@_N@Z",
	uintptr_t _this, BlockSource* a2, uintptr_t a3, BlockPos* bp, bool a5) {
	EventCallBackHelper h(EventCode::onCommandBlockPerform);
	// Pulse:0, Repeat:1, Chain:2
	int mode = SymCall<int>("?getMode@CommandBlockActor@@QEBA?AW4CommandBlockMode@@AEAVBlockSource@@@Z", a3, a2);
	// Unconditional:0, Conditional:1
	bool condition = SymCall<bool>("?getConditionalMode@CommandBlockActor@@QEBA_NAEAVBlockSource@@@Z", a3, a2);
	//SymCall<std::string&>("?getName@BaseCommandBlock@@QEBAAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
	//	a3 + 200);
	//a3 + 200 BaseCommandBlock
	std::string cmd = Dereference<std::string>(a3, 256);
	std::string rawname = Dereference<std::string>(a3, 288);
	h
		.insert("mode", mode)
		.insert("condition", condition)
		.insert("cmd", cmd)
		.insert("rawname", rawname)
		.insert("position", bp);
	if (h.call())
		return onCommandBlockPerform.original(_this, a2, a3, bp, a5);
	return false;
}
// player move
TMHOOK(onMove, void*, "??0MovePlayerPacket@@QEAA@AEBVPlayer@@W4PositionMode@PlayerPositionModeComponent@@HH@Z",
	uintptr_t _this, Player* p, char a3, int a4, int a5) {
	float speed = p->getSpeedInMetersPerSecond();
	//cout << "getSpeedInMetersPerSecond: " << speed << endl;
	if (speed > 1 && IsPlayerInit(p)) {
		EventCallBackHelper h(EventCode::onMove);
		h.setArg(ToEntity(p)).call();
		return onMove.original(_this, p, a3, a4, a5);
	}
	return onMove.original(_this, p, a3, a4, a5);
}
// player set armor
TMHOOK(onSetArmor, void, "?setArmor@Player@@UEAAXW4ArmorSlot@@AEBVItemStack@@@Z",
	Player* p, unsigned slot, ItemStack* i) {
	EventCallBackHelper h(EventCode::onSetArmor);
	h
		.insert("player", ToEntity(p))
		.insert("itemid", i->getId())
		.insert("itemcount", i->getCount())
		.insert("itemname", i->getName())
		.insert("itemaux", i->getAuxValue())
		.insert("slot", slot);
	if (h.call())
		return onSetArmor.original(p, slot, i);
	return;
}
// scoreboard objective changed
//when create scoreboard：/scoreboard objectives <add|remove> <objectivename> dummy <objectivedisplayname>
//when modify scoreboard(this func hook here)：/scoreboard players <add|remove|set> <playersname> <objectivename> <playersnum>
TMHOOK(onScoreChanged, void, "?onScoreChanged@ServerScoreboard@@UEAAXAEBUScoreboardId@@AEBVObjective@@@Z",
	Scoreboard* _this, ScoreboardId* a1, Objective* a2) {
	EventCallBackHelper h(EventCode::onScoreChanged);
	h.insert("scoreboardid", a1->id)
		.insert("playersnum", a2->getPlayerScore(a1)->getCount())
		.insert("objectivename", a2->getScoreName())
		.insert("objectivedisname", a2->getScoreDisplayName());
	h.call();
	onScoreChanged.original(_this, a1, a2);
}
// farm land destory
TMHOOK(onFallBlockTransform, void, "?transformOnFall@FarmBlock@@UEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@M@Z",
	uintptr_t _this, BlockSource* a1, BlockPos* a2, Actor* p, uintptr_t a4) {
	EventCallBackHelper h(EventCode::onFallBlockTransform);
	if (IsPlayer(p)) {
		h.insert("player", p)
			.insert("position", a2)
			.insert("dimensionid", a1->getDimensionId());
		if (!h.call())
			return;
	}
	onFallBlockTransform.original(_this, a1, a2, p, a4);
}
// use respawn anchor block
TMHOOK(onUseRespawnAnchorBlock, bool, "?trySetSpawn@RespawnAnchorBlock@@CA_NAEAVPlayer@@AEBVBlockPos@@AEAVBlockSource@@AEAVLevel@@@Z",
	Player* p, BlockPos* a2, BlockSource* a3, Level* a4) {
	EventCallBackHelper h(EventCode::onUseRespawnAnchorBlock);
	h.insert("player", ToEntity(p))
		.insert("position", ToList(a2))
		.insert("dimensionid", a3->getDimensionId());
	if (h.call())
		return onUseRespawnAnchorBlock.original(p, a2, a3, a4);
	return false;
}
// push piston
TMHOOK(onPistonPush, bool, "?_attachedBlockWalker@PistonBlockActor@@AEAA_NAEAVBlockSource@@AEBVBlockPos@@EE@Z",
	BlockActor* _this, BlockSource* bs, BlockPos* bp, unsigned a3, unsigned a4) {
	EventCallBackHelper h(EventCode::onPistonPush);
	BlockLegacy* blg = bs->getBlock(bp)->getBlockLegacy();
	std::string bn = blg->getBlockName();
	short bid = blg->getBlockItemID();
	//BlockPos* bp2 = _this->getPosition();//will cause a crash
	h.insert("blockname", bn)
		.insert("blockid", bid)
		.insert("blockpos", bp)
		//.insert("pistonpos", bp2)//will cause a crash
		.insert("dimensionid", bs->getDimensionId());
	if (h.call())
		return onPistonPush.original(_this, bs, bp, a3, a4);
	return false;
}
// enderman random teleport
TMHOOK(onEndermanRandomTeleport, bool, "?randomTeleport@TeleportComponent@@QEAA_NAEAVActor@@@Z",
	uintptr_t _this, Actor* a1) {
	EventCallBackHelper h(EventCode::onEndermanRandomTeleport);
	h.setArg(ToEntity(a1)).call();
	if (h.call())
		return onEndermanRandomTeleport.original(_this, a1);
	return false;
}
// player drop item
TMHOOK(onDropItem, bool, "?drop@Player@@UEAA_NAEBVItemStack@@_N@Z",
	Player* _this, ItemStack* a2, bool a3) {
	EventCallBackHelper h(EventCode::onDropItem);
	h.insert("player", _this)
		.insert("itemid", a2->getId())
		.insert("itemcount", a2->getCount())
		.insert("itemname", a2->getName())
		.insert("itemaux", a2->getAuxValue());
	if (h.call())
		return onDropItem.original(_this, a2, a3);
	return false;
}
// player take item
TMHOOK(onTakeItem, bool, "?take@Player@@QEAA_NAEAVActor@@HH@Z",
	Player* _this, Actor* actor, int a2, int a3) {
	EventCallBackHelper h(EventCode::onTakeItem);
	h.insert("player", _this)
		.insert("actor", actor);
	if (h.call())
		return onTakeItem.original(_this, actor, a2, a3);
	return false;
}
// ride
TMHOOK(onRide, bool, "?canAddPassenger@Actor@@UEBA_NAEAV1@@Z",
	Actor* a1, Actor* a2) {
	EventCallBackHelper h(EventCode::onRide);
	h.insert("actor1", a1)
		.insert("actor2", a2);
	if (h.call())
		return onRide.original(a1, a2);
	return false;
}
// Place the item in the frame block of the removed item
TMHOOK(onUseFrameBlock, bool, "?use@ItemFrameBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
	uintptr_t _this, Player* a2, BlockPos* a3) {
	EventCallBackHelper h(EventCode::onUseFrameBlock);
	h.insert("player", a2)
		.insert("blockpos", a3)
		.insert("dimensionid", a2->getDimensionId());
	if (h.call())
		return onUseFrameBlock.original(_this, a2, a3);
	return false;
}
// click frame block
TMHOOK(onUseFrameBlock2, bool, "?attack@ItemFrameBlock@@UEBA_NPEAVPlayer@@AEBVBlockPos@@@Z",
	uintptr_t _this, Player* a2, BlockPos* a3) {
	EventCallBackHelper h(EventCode::onUseFrameBlock);
	h.insert("player", a2)
		.insert("blockpos", a3)
		.insert("dimensionid", a2->getDimensionId());
	if (h.call())
		return onUseFrameBlock2.original(_this, a2, a3);
	return false;
}
// player jump
TMHOOK(onJump, void, "?jumpFromGround@Player@@UEAAXAEBVIConstBlockSource@@@Z",
	Player* _this, uintptr_t a2) {
	EventCallBackHelper h(EventCode::onJump);
	h.setArg(ToEntity(_this));
	if (h.call())
		return onJump.original(_this, a2);
}
// player sneak
TMHOOK(onSneak, void, "?sendActorSneakChanged@ActorEventCoordinator@@QEAAXAEAVActor@@_N@Z",
	uintptr_t _this, Actor* a1, bool a2) {
	EventCallBackHelper h(EventCode::onSneak);
	h.setArg(ToEntity(a1));
	if (h.call())
		return onSneak.original(_this, a1, a2);
}
// fire spread
TMHOOK(onFireSpread, bool, "?_trySpawnBlueFire@FireBlock@@AEBA_NAEAVBlockSource@@AEBVBlockPos@@@Z",
	uintptr_t _this, BlockSource* bs, BlockPos* bp) {
	EventCallBackHelper h(EventCode::onFireSpread);
	BlockLegacy* bl = bs->getBlock(bp)->getBlockLegacy();
	h.insert("blockpos", bp)
		.insert("blockname", bl->getBlockName())
		.insert("blockid", bl->getBlockItemID())
		.insert("dimensionid", bs->getDimensionId());
	if (h.call())
		return onFireSpread.original(_this, bs, bp);
	return false;
}
// Square interaction (except container, workbenches)
TMHOOK(onBlockInteracted, void, "?onBlockInteractedWith@VanillaServerGameplayEventListener@@UEAA?AW4EventResult@@AEAVPlayer@@AEBVBlockPos@@@Z",
	uintptr_t _this, Player* pl, BlockPos* bp) {
	EventCallBackHelper h(EventCode::onBlockInteracted);
	BlockSource* bs = global<Level>->getBlockSource(pl->getDimensionId());
	BlockLegacy* bl = bs->getBlock(bp)->getBlockLegacy();
	h.insert("player", pl)
		.insert("blockpos", bp)
		.insert("blockname", bl->getBlockName())
		.insert("blockid", bl->getBlockItemID())
		.insert("dimensionid", bs->getDimensionId());
	if (h.call())
		return onBlockInteracted.original(_this, pl, bp);
}
// Block destroyed by explosion
TMHOOK(onBlockExploded, void, "?onExploded@Block@@QEBAXAEAVBlockSource@@AEBVBlockPos@@PEAVActor@@@Z",
	Block* _this, BlockSource* bs, BlockPos* bp, Actor* actor) {
	EventCallBackHelper h(EventCode::onBlockExploded);
	BlockLegacy* bl = bs->getBlock(bp)->getBlockLegacy();
	h.insert("actor", actor)
		.insert("blockpos", bp)
		.insert("blockname", bl->getBlockName())
		.insert("blockid", bl->getBlockItemID())
		.insert("dimensionid", bs->getDimensionId());
	if (h.call())
		return onBlockExploded.original(_this, bs, bp, actor);
}
// player click sign brand
TMHOOK(onUseSignBlock, uintptr_t, "?use@SignBlock@@UEBA_NAEAVPlayer@@AEBVBlockPos@@E@Z",
	uintptr_t _this, Player* a1, BlockPos* a2) {
	EventCallBackHelper h(EventCode::onUseSignBlock);
	BlockSource* bs = a1->getRegion();
	BlockActor* ba = bs->getBlockEntity(a2);
	std::string text;
	// get sign block content
	SymCall<std::string&>("?getImmersiveReaderText@SignBlockActor@@UEAA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AEAVBlockSource@@@Z",
		ba, &text, bs);
	h.insert("player", a1)
		.insert("text", text)
		.insert("position", a2);
	if (h.call())
		return onUseSignBlock.original(_this, a1, a2);
	return 0;
}

TMHOOK(onLiquidSpread, bool, 
	"?_canSpreadTo@LiquidBlockDynamic@@AEBA_NAEAVBlockSource@@AEBVBlockPos@@1E@Z",
	uintptr_t _this, BlockSource *a2, BlockPos *dst_pos, BlockPos *src_pos, char a5)
{
	std::string src_name = a2->getBlock(src_pos)->getBlockLegacy()->getBlockName();
	std::string dst_name = a2->getBlock(dst_pos)->getBlockLegacy()->getBlockName();

	EventCallBackHelper h(EventCode::onLiquidSpread);
	h
		.insert("src_name", src_name)
		.insert("src_pos", src_pos)
		.insert("dst_name", dst_name)
		.insert("dst_pos", dst_pos);

	if (!h.call())
		return false;
	
	return onLiquidSpread.original(_this, a2, dst_pos, src_pos, a5);
}
TMHOOK(onChatPkt, uintptr_t, 
	"?createChat@TextPacket@@SA?AV1@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@000@Z",
	uintptr_t a1, std::string *name, std::string *msg, std::string *xuid, std::string &a4)
{
	Player *player = global<Level>->getPlayerByXuid(*xuid);

	const char *name_c_str = name->c_str();
	const char *msg_c_str = msg->c_str();
	
	EventCallBackHelper h(EventCode::onChatPkt);
	h
		.insert("player", player)
		.insert("name_ptr", &name_c_str)
		.insert("msg_ptr", &msg_c_str)
		.insert("xuid", xuid);

	h.call();

	std::string name_mod(name_c_str);
	std::string msg_mod(msg_c_str);

	return onChatPkt.original(a1, &name_mod, &msg_mod, xuid, a4);
}
#pragma endregion


bool load_plugin(void)
{
    init_hooks();

    return true;
}

bool init_hooks(void)
{
	DedicatedServer_start.init(&DedicatedServer_start);
	Level_construct.init(&Level_construct);
	SPSCQueue_construct.init(&SPSCQueue_construct);
	RakPeer_construct.init(&RakPeer_construct);
	ServerNetworkHandler_construct.init(&ServerNetworkHandler_construct);
	ServerScoreboard_construct.init(&ServerScoreboard_construct);
	Player_construct.init(&Player_construct);
	Player_destruct.init(&Player_destruct);
	ChangeSettingCommand_setup.init(&ChangeSettingCommand_setup);

	onServerStarted.init(&onServerStarted);
	onConsoleOutput.init(&onConsoleOutput);
	onConsoleInput.init(&onConsoleInput);
	onPreJoin.init(&onPreJoin);
	onPlayerJoin.init(&onPlayerJoin);
	onPlayerLeft.init(&onPlayerLeft);
	filterInventoryTransaction.init(&filterInventoryTransaction);
	onUseItem.init(&onUseItem);
	onUseItemEx.init(&onUseItemEx);
	onPlaceBlock.init(&onPlaceBlock);
	onPlacedBlock.init(&onPlacedBlock);
	onDestroyBlock.init(&onDestroyBlock);
	onDestroyedBlock.init(&onDestroyedBlock);
	onOpenChest.init(&onOpenChest);
	onOpenBarrel.init(&onOpenBarrel);
	onCloseChest.init(&onCloseChest);
	onCloseBarrel.init(&onCloseBarrel);
	onContainerChange.init(&onContainerChange);
	onPlayerInventoryChange.init(&onPlayerInventoryChange);
	onAttack.init(&onAttack);
	onCalcDamage.init(&onCalcDamage);
	onChangeDimension.init(&onChangeDimension);
	onMobDie.init(&onMobDie);
	onMobHurt.init(&onMobHurt);
	onRespawn.init(&onRespawn);
	onChat.init(&onChat);
	onInputText.init(&onInputText);
	onInputCommand.init(&onInputCommand);
	onSelectForm.init(&onSelectForm);
	onCommandBlockUpdate.init(&onCommandBlockUpdate);
	onLevelExplode.init(&onLevelExplode);
	onCommandBlockPerform.init(&onCommandBlockPerform);
	onMove.init(&onMove);
	onSetArmor.init(&onSetArmor);
	onScoreChanged.init(&onScoreChanged);
	onFallBlockTransform.init(&onFallBlockTransform);
	onUseRespawnAnchorBlock.init(&onUseRespawnAnchorBlock);
	onPistonPush.init(&onPistonPush);
	onEndermanRandomTeleport.init(&onEndermanRandomTeleport);
	onDropItem.init(&onDropItem);
	onTakeItem.init(&onTakeItem);
	onRide.init(&onRide);
	onUseFrameBlock.init(&onUseFrameBlock);
	onUseFrameBlock2.init(&onUseFrameBlock2);
	onJump.init(&onJump);
	onFireSpread.init(&onFireSpread);
	onBlockInteracted.init(&onBlockInteracted);
	onBlockExploded.init(&onBlockExploded);
	onUseSignBlock.init(&onUseSignBlock);
	onLiquidSpread.init(&onLiquidSpread);
	onChatPkt.init(&onChatPkt);

	hooker_enable_all_hook();

    return true;
}
