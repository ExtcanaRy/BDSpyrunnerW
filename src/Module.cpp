#include <pyrw/Module.h>
#include <pyrw/Version.h>
#include <pyrw/Logger.h>

#include <mc/tool.h>
#include <mc/Actor.h>
#include <mc/Block.h>
#include <mc/ItemStack.h>
#include <mc/ScoreBoard.h>
#include <mc/Structure.h>
#include <mc/Tag.h>
#include <mc/DataIO.h>

using namespace std;

// hot reload Python modules
void ReloadPythonModules(string moduleName) {
	Logger logger("BDSpyrunnerW");
	if (moduleName.empty()) {
		g_callback_functions.clear();
		g_commands.clear();
	}
	for (auto& [name, pModule] : g_py_modules) {
		if (moduleName.empty() || moduleName == name) {
			logger.info("Reloading " + name);
			PyObject* pNewModule = PyImport_ReloadModule(pModule);
			if (pNewModule == NULL) {
				PrintPythonError();
			}
			else {
				g_py_modules[name] = pNewModule;
			}
		}
	}
}

void LoadPythonModules(string moduleName) {
	Logger logger("BDSpyrunnerW");
	for (auto& info : filesystem::directory_iterator(PLUGIN_PATH)) {
		//whether the file is py
		if (info.path().extension() == ".py") {
			string name(info.path().stem().u8string());
			//ignore files starting with '_'
			if (name.front() == '_')
				continue;
			if (g_py_modules.find(name) != g_py_modules.end()) {
				logger.info(name + " already loaded, reloading...");
				ReloadPythonModules(name);
				continue;
			}
			// check if this is the module we want to load
			if (!moduleName.empty() && name != moduleName) {
				continue;
			}
			logger.info("Loading " + name);
			PyObject* pModule = PyImport_ImportModule(name.c_str());
			if (pModule == NULL) {
				PrintPythonError();
			}
			else {
				g_py_modules[name] = pModule;
			}
		}
	}
}

void InitPythonInterpreter(bool reinit) {
    if (reinit) {
        // shutdown interpreter & release resources
        for (auto& [name, pModule] : g_py_modules) {
            Py_DECREF(pModule);
        }
        g_py_modules.clear();
        Py_FinalizeEx();
    }

    // init interpreter
    if (!filesystem::exists(PLUGIN_PATH))
        filesystem::create_directories(PLUGIN_PATH);

    wstring py_path = PLUGIN_PATH L";" PLUGIN_PATH "Dlls;"
                      PLUGIN_PATH "Lib;" PLUGIN_PATH "Extra;";
	py_path += Py_GetPath();
    Py_SetPath(py_path.c_str());

    // Add a module
    const char* moduleName = (fopen((string(PLUGIN_PATH) + "mc.py").c_str(), "r")) == NULL
                                 ? "mc"
                                 : "mco";
    PyImport_AppendInittab(moduleName, mc_init);

    Py_InitializeEx(0);

    if (PyType_Ready(&PyEntity_Type) < 0) {
        Py_FatalError("Can't initialize entity type");
    }

    PyEval_InitThreads();
    LoadPythonModules("");

    if (!reinit) {
        PyThreadState* threadState = PyThreadState_Get();
        PyEval_ReleaseThread(threadState);
    }
}

//Reload Plugin
static PyObject* reload(PyObject*, PyObject* args) {
	const char* name;
	Py_PARSE("s", &name);
	ReloadPythonModules(name);
	Py_RETURN_NONE;
}

static PyObject* setListener(PyObject*, PyObject* args) {
    const char* name; PyObject* func;
	Py_PARSE("sO", &name, &func);
    auto it = events.find(name);
    if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 is not callable");
        return NULL;
    }
    if (it == events.end()) {
        char err_msg[64];
        snprintf(err_msg, sizeof(err_msg), "Invalid Listener key words: %s", name);
        PyErr_SetString(PyExc_ValueError, err_msg);
        return NULL;
    }
    g_callback_functions[it->second].push_back(func);
    Py_RETURN_NONE;
}


static PyObject* removeListener(PyObject*, PyObject* args) {
	const char* name; PyObject* func;
	Py_PARSE("sO", &name, &func);
	auto it = events.find(name);
	if (!PyCallable_Check(func)) {
        PyErr_SetString(PyExc_TypeError, "Parameter 2 is not callable");
        return NULL;
    }
    if (it == events.end()) {
        char err_msg[64];
        snprintf(err_msg, sizeof(err_msg), "Invalid Listener key words: %s", name);
        PyErr_SetString(PyExc_ValueError, err_msg);
        return NULL;
    }

	auto& callbacks = g_callback_functions[it->second];
	auto iter = std::find(callbacks.begin(), callbacks.end(), func);
	if (iter != callbacks.end()) {
		callbacks.erase(iter);
		Py_RETURN_NONE;
	}
	else {
		Py_RETURN_ERROR("Listener not found");
	}
}

static PyObject* minVersionRequire(PyObject*, PyObject* args) {
	int major, minor, micro;
	Py_PARSE("iii", &major, &minor, &micro);
	if (major > PYR_MAJOR_VERSION || minor > PYR_MINOR_VERSION || micro > PYR_MICRO_VERSION)
		Py_RETURN_ERROR("The plugin version does not meet the minimum requirements");
	Py_RETURN_NONE;
}

static PyObject* getBDSVersion(PyObject*, PyObject* args) {
	string version = SymCall<std::string>("?getServerVersionString@Common@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ");
	return ToPyStr(version);
}

static PyObject* logout(PyObject*, PyObject* args) {
	const char* msg;
	Py_PARSE("s", &msg);
	SymCall<ostream&>("??$_Insert_string@DU?$char_traits@D@std@@_K@std@@YAAEAV?$basic_ostream@DU?$char_traits@D@std@@@0@AEAV10@QEBD_K@Z",
		&cout, msg, strnlen_s(msg, sizeof(msg)));
	Py_RETURN_NONE;
}

constexpr int IsSlimeChunk(unsigned x, unsigned z) {
	unsigned mt0 = (x * 0x1F1F1F1F) ^ z;
	unsigned mt1 = (1812433253u * (mt0 ^ (mt0 >> 30u)) + 1);
	unsigned mt2 = mt1;
	for (unsigned i = 2; i < 398; ++i)
		mt2 = (1812433253u * (mt2 ^ (mt2 >> 30u)) + i);
	unsigned k = (mt0 & 0x80000000u) + (mt1 & 0x7FFFFFFFU);
	mt0 = mt2 ^ (k >> 1u);
	if (k & 1)
		mt0 ^= 2567483615u;
	mt0 ^= (mt0 >> 11u);
	mt0 ^= (mt0 << 7u) & 0x9D2C5680u;
	mt0 ^= (mt0 << 15u) & 0xEFC60000u;
	mt0 ^= (mt0 >> 18u);
	return !(mt0 % 10);
}

static PyObject* runCommand(PyObject*, PyObject* args) {
	const char* cmd;
	Py_PARSE("s", &cmd);
	if (global<SPSCQueue> == nullptr)
		Py_RETURN_ERROR("Command queue is not initialized");
	SymCall<bool, SPSCQueue*, const string&>("??$inner_enqueue@$0A@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@?$SPSCQueue@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@$0CAA@@@AEAA_NAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		global<SPSCQueue>, cmd);
	Py_RETURN_NONE;
}

static PyObject* setCommandDescription(PyObject*, PyObject* args) {
	const char* cmd;
	const char* des;
	PyObject* callback = nullptr;
	Py_PARSE("ss|O", &cmd, &des, &callback);
	g_commands[cmd] = { des, callback };
	Py_RETURN_NONE;
}

static PyObject* getPlayerByXuid(PyObject*, PyObject* args) {
	const char* xuid;
	Py_PARSE("s", &xuid);
	Player* p = global<Level>->getPlayerByXuid(xuid);
	if (p == nullptr)
		Py_RETURN_ERROR("Failed to find player");
	return ToEntity(p);
}

static PyObject* getPlayerList(PyObject*, PyObject* args) {
	PyObject* list = PyList_New(0);
	if (global<std::vector<Player*, std::allocator<Player*>>> != nullptr)
		for (std::vector<Player*>::iterator iter = global<std::vector<Player*>>->begin();
			iter != global<std::vector<Player*>>->end(); iter++) {
		PyList_Append(list, ToEntity(*iter));
	}
	return list;
}

static PyObject* setDamage(PyObject*, PyObject* args) {
	Py_PARSE("i", &g_damage);
	Py_RETURN_NONE;
}
static PyObject* setServerMotd(PyObject*, PyObject* args) {
	const char* name;
	Py_PARSE("s", &name);
	if (global<ServerNetworkHandler> == nullptr)
		Py_RETURN_ERROR("Server did not finish loading");
	SymCall<uintptr_t, ServerNetworkHandler*, const string&, bool>("?allowIncomingConnections@ServerNetworkHandler@@QEAAXAEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
		global<ServerNetworkHandler>, name, true);
	Py_RETURN_NONE;
}

static PyObject* getBlock(PyObject*, PyObject* args) {
	BlockPos bp; int did;
	Py_PARSE("iiii", &bp.x, &bp.y, &bp.z, &did);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	BlockLegacy* bl = bs->getBlock(&bp)->getBlockLegacy();
	return Py_BuildValue("{s:s:s:i}",
		"blockname", bl->getBlockName().c_str(),
		"blockid", bl->getBlockItemID()
	);
}
static PyObject* setBlock(PyObject*, PyObject* args) {
	const char* name;
	BlockPos bp; int did;
	Py_PARSE("siiii", &name, &bp.x, &bp.y, &bp.z, &did);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	Block* b = *reinterpret_cast<Block**>(SYM((string("?m") + name + "@VanillaBlocks@@3PEBVBlock@@EB").c_str()));
	if (b == nullptr)
		Py_RETURN_ERROR("Unknown Block");
	bs->setBlock(&bp, b);
	Py_RETURN_NONE;
}

static PyObject* getStructure(PyObject*, PyObject* args) {
	BlockPos pos1, pos2; int did;
	Py_PARSE("iiiiiii",
		&pos1.x, &pos1.y, &pos1.z,
		&pos2.x, &pos2.y, &pos2.z, &did
	);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	BlockPos start{
		min(pos1.x, pos2.x),
		min(pos1.y, pos2.y),
		min(pos1.z, pos2.z)
	};
	BlockPos size{
		max(pos1.x, pos2.x) - start.x,
		max(pos1.y, pos2.y) - start.y,
		max(pos1.z, pos2.z) - start.z
	};
	StructureSettings ss(&size, false, false);
	StructureTemplate st("tmp");
	st.fillFromWorld(bs, &start, &ss);

	return ToPyStr(CompoundTagtoJson(st.save()).dump(4));
}
static PyObject* setStructure(PyObject*, PyObject* args, PyObject* kwds) {
	Py_KERWORDS_LIST("data", "x", "y", "x", "dim", "update");
	bool update = true;
	const char* data;
	BlockPos pos; int did;
	Py_PARSE_WITH_KERWORDS("siiii|b", &data, &pos.x, &pos.y, &pos.z, &did, &update);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	Json value = StringToJson(data);
	Json& arr = value["size9"];
	if (!arr.is_array())
		Py_RETURN_ERROR("Invalid json string");
	BlockPos size{
		arr[0].get<int>(),
		arr[1].get<int>(),
		arr[2].get<int>()
	};
	StructureSettings ss(&size, true, false);
	StructureTemplate st("tmp");
	st.fromJson(value);
	st.placeInWorld(bs, global<Level>->getBlockPalette(), &pos, &ss);
	if (update) {
		for (int x = 0; x != size.x; ++x) {
			for (int y = 0; y != size.y; ++y) {
				for (int z = 0; z != size.z; ++z) {
					BlockPos bp{ x, y, z };
					bs->neighborChanged(&bp);
				}
			}
		}
	}
	Py_RETURN_NONE;
}

static PyObject* getStructureRaw(PyObject*, PyObject* args) {
	BlockPos pos1, pos2; int did;
	Py_PARSE("iiiiiii",
		&pos1.x, &pos1.y, &pos1.z,
		&pos2.x, &pos2.y, &pos2.z, &did
	);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	BlockPos start{
		min(pos1.x, pos2.x),
		min(pos1.y, pos2.y),
		min(pos1.z, pos2.z)
	};
	BlockPos size{
		max(pos1.x, pos2.x) - start.x,
		max(pos1.y, pos2.y) - start.y,
		max(pos1.z, pos2.z) - start.z
	};
	StructureSettings ss(&size, false, false);
	StructureTemplate st("tmp");
	st.fillFromWorld(bs, &start, &ss);
	CompoundTag* t = st.save();
	BinaryStream* stream = new BinaryStream();
	serialize<CompoundTag>::write(t, stream);
	size_t sizet = stream->mBuffer->length();
	auto result = PyBytes_FromStringAndSize(stream->GetAndReleaseData()->c_str(), sizet);
	stream->~BinaryStream();
	return result;
}

static PyObject* setStructureRaw(PyObject*, PyObject* args, PyObject* kwds) {
	Py_KERWORDS_LIST("data", "x", "y", "x", "dim", "update");
	bool update = true;
	const char* data;
	Py_ssize_t datasize;
	//Py_buffer data;
	BlockPos pos; int did;
	Py_PARSE_WITH_KERWORDS("y#iiii|b", &data, &datasize, &pos.x, &pos.y, &pos.z, &did, &update);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	ReadOnlyBinaryStream* stream = new ReadOnlyBinaryStream(new std::string(data, datasize));
	//printf("bufferlength: %d\n",stream->mBuffer->length());
	CompoundTag* tag = serialize<CompoundTag>::read(stream);
	//printf("deserialized.\n");
	if (tag->getVariantType() != TagType::Compound)
		Py_RETURN_ERROR("Invalid Tag");
	auto& t_C = tag->asCompound();
	if (t_C.find("size") == t_C.end() || t_C["size"].getVariantType() != TagType::List)
		Py_RETURN_ERROR("Invalid Tag");
	auto& t_C_Lsize = t_C["size"].asList();

	BlockPos size{
		t_C_Lsize[0]->asInt(),
		t_C_Lsize[1]->asInt(),
		t_C_Lsize[2]->asInt()
	};
	StructureSettings ss(&size, true, false);
	StructureTemplate st("tmp");
	st.fromCompound(tag);
	st.placeInWorld(bs, global<Level>->getBlockPalette(), &pos, &ss);
	if (update) {
		for (int x = 0; x != size.x; ++x) {
			for (int y = 0; y != size.y; ++y) {
				for (int z = 0; z != size.z; ++z) {
					BlockPos bp{ x, y, z };
					bs->neighborChanged(&bp);
				}
			}
		}
	}
	Py_RETURN_NONE;
}

static PyObject* explode(PyObject*, PyObject* args) {
	Vec3 pos; int did;
	float power; bool destroy;
	float range; bool fire;
	Py_PARSE("fffifbfb",
		&pos.x, &pos.y, &pos.z, &did, &power, &destroy, &range, &fire
	);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (!bs)
		Py_RETURN_ERROR("Unknown dimension ID");
	SymCall<bool>("?explode@Level@@UEAAXAEAVBlockSource@@PEAVActor@@AEBVVec3@@M_N3M3@Z",
		global<Level>, bs, nullptr, pos, power, fire, destroy, range, true);
	Py_RETURN_NONE;
}

static PyObject* spawnItem(PyObject*, PyObject* args) {
	const char* data;
	Vec3 pos; int did;
	Py_PARSE("sfffi", &data, &pos.x, &pos.y, &pos.z, &did);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (!bs)
		Py_RETURN_ERROR("Unknown dimension ID");
	ItemStack item(StringToJson(data));
	global<Level>->getSpawner()->spawnItem(bs, &item, &pos);
	Py_RETURN_NONE;
}

static PyObject* isSlimeChunk(PyObject*, PyObject* args) {
	unsigned x, z;
	Py_PARSE("II", &x, &z);
	if (IsSlimeChunk(x, z))
		Py_RETURN_TRUE;
	else
		Py_RETURN_FALSE;
}

static PyObject* setSignBlockMessage(PyObject*, PyObject* args) {
	const char* name;
	BlockPos bp; int did;
	Py_PARSE("siiii", &name, &bp.x, &bp.y, &bp.z, &did);
	if (global<Level> == nullptr)
		Py_RETURN_ERROR("Level is not set");
	BlockSource* bs = global<Level>->getBlockSource(did);
	if (bs == nullptr)
		Py_RETURN_ERROR("Unknown dimension ID");
	BlockActor* sign = bs->getBlockEntity(&bp);
	SymCall<void, BlockActor*, const string&, const string&>("?setMessage@SignBlockActor@@QEAAXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z",
		sign, name, name);
	sign->setChanged();
	Py_RETURN_NONE;
}

static PyMethodDef Methods[]{
	{ "minVersionRequire", minVersionRequire, METH_VARARGS, nullptr },
	{ "getBDSVersion", getBDSVersion, METH_NOARGS, nullptr },
	{ "logout", logout, METH_VARARGS, nullptr },
	{ "runcmd", runCommand, METH_VARARGS, nullptr },
	{ "setListener", setListener, METH_VARARGS, nullptr },
	{ "removeListener", removeListener, METH_VARARGS, nullptr },
	{ "reload", reload, METH_VARARGS, nullptr },
	{ "setCommandDescription", setCommandDescription, METH_VARARGS, nullptr },
	{ "getPlayerByXuid", getPlayerByXuid, METH_VARARGS, nullptr },
	{ "getPlayerList", getPlayerList, METH_NOARGS, nullptr },
	{ "setDamage", setDamage, METH_VARARGS, nullptr },
	{ "setServerMotd", setServerMotd, METH_VARARGS, nullptr },
	{ "getBlock", getBlock, METH_VARARGS, nullptr },
	{ "setBlock", setBlock, METH_VARARGS, nullptr },
	{ "getStructure", getStructure, METH_VARARGS, nullptr },
	{ "setStructure", (PyCFunction)setStructure, METH_VARARGS | METH_KEYWORDS, nullptr },
	{ "getStructureRaw", getStructureRaw, METH_VARARGS, nullptr },
	{ "setStructureRaw", (PyCFunction)setStructureRaw, METH_VARARGS | METH_KEYWORDS, nullptr },
	{ "explode", explode, METH_VARARGS, nullptr },
	{ "spawnItem", spawnItem, METH_VARARGS, nullptr },
	{ "isSlimeChunk", isSlimeChunk, METH_VARARGS, nullptr },
	{ "setSignBlockMessage", setSignBlockMessage, METH_VARARGS, nullptr },
	{ nullptr }
};

static PyModuleDef Module{
	PyModuleDef_HEAD_INIT,
		"mc",
		"API functions",
		-1,
		Methods,
		nullptr,
		nullptr,
		nullptr,
		nullptr
};

extern "C" PyObject * mc_init() {
	PyObject* module = PyModule_Create(&Module);
	PyModule_AddObject(module, "Entity", reinterpret_cast<PyObject*>(&PyEntity_Type));
	return module;
}

__int64 getCurrentTime() {
	time_t now = clock();
	return now;
}