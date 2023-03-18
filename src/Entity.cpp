#include <pyrw/Entity.h>

#include <mc/tool.h>
#include <mc/Actor.h>
#include <mc/ItemStack.h>
#include <mc/NetWork.h>
#include <mc/ScoreBoard.h>
#include <mc/Tag.h>

using namespace std;
struct PyEntity {
	PyObject_HEAD;
	Actor* actor;

	static Actor* asActor(PyObject* self) {
		Actor* entity = reinterpret_cast<PyEntity*>(self)->actor;
		if (entity) {
			if (IsPlayer(entity)) return asPlayer(self);
			return entity;
		}
		else
			Py_RETURN_ERROR("This entity pointer is nullptr");
	}
	static Player* asPlayer(PyObject* self) {
		Actor* entity = reinterpret_cast<PyEntity*>(self)->actor;
		__try {
		// Player::getPlatform return *(unsigned int *)(a1 + 1728);
			if (IsPlayer(entity) /*&& *((char*)entity + 1728)*/)
				return reinterpret_cast<Player*>(reinterpret_cast<PyEntity*>(self)->actor);
		}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
		Py_RETURN_ERROR("This entity pointer is nullptr or is not player pointer");
	}
	static int print(PyObject* self, FILE* file, int) {
		Actor* a = asActor(self);
		if (!a)
			return -1;
		fputs(a->getNameTag().c_str(), file);
		return 0;
	}
	static PyObject* repr(PyObject* self) {
		Actor* a = asActor(self);
		if (!a)
			return ToPyStr("");
		return ToPyStr(a->getNameTag());
	}
	static Py_hash_t hash(PyObject* self) {
		return reinterpret_cast<Py_hash_t>(asActor(self));
	}
	static PyObject* str(PyObject* self) {
		Actor* a = asActor(self);
		if (!a)
			return ToPyStr("");
		return ToPyStr(a->getNameTag());
	}
	static PyObject* rich_compare(PyObject* self, PyObject* other, int op) {
		switch (op) {
			//<
		case Py_LT:break;
			//<=
		case Py_LE:break;
			//==
		case Py_EQ:
			if (PyEntity::asActor(self) == PyEntity::asActor(other))
				Py_RETURN_TRUE;
			else
				Py_RETURN_FALSE;
			break;
			//!=
		case Py_NE:
			if (PyEntity::asActor(self) != PyEntity::asActor(other))
				Py_RETURN_TRUE;
			else
				Py_RETURN_FALSE;
			break;
			//>
		case Py_GT:break;
			//>=
		case Py_GE:break;
		}
		Py_RETURN_NOTIMPLEMENTED;
	}

		static PyObject* getName(PyObject* self, void*) {
		Actor* a = PyEntity::asActor(self);
		if (!a)
			return nullptr;
		return ToPyStr(a->getNameTag());
	}
	static int setName(PyObject* self, PyObject* arg, void*) {
		if (PyUnicode_Check(arg)) {
			Player* p = PyEntity::asPlayer(self);
			if (!p)
				return -1;
			p->setNameTag(PyUnicode_AsUTF8(arg));
			return 0;
		}
		return PyErr_BadArgument(), -1;
	}
static PyObject* getUuid(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	return ToPyStr(p->getUuid());
}
static PyObject* getXuid(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	return ToPyStr(p->getXuid());
}
static PyObject* getPos(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return ToList(a->getPos());
}
static PyObject* getDimensionId(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyLong_FromLong(a->getDimensionId());
}
static PyObject*getIsStand(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyBool_FromLong(a->isStanding());
}
static PyObject* getIsSneaking(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyBool_FromLong(a->isSneaking());
}
static PyObject* getTypeID(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyLong_FromLong(a->getEntityTypeId());
}
static PyObject* getTypeName(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return ToPyStr(a->getEntityTypeName());
}
static PyObject* getNBTInfo(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	//memory leak, wait for fix 2021.10.22
	Tag* t = a->save();
	PyObject* result = ToPyStr(CompoundTagtoJson(t).dump(4));
	t->deleteCompound();
	return result;
}
static PyObject* getHealth(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(a->getHealth()));
}
static int setHealth(PyObject* self, PyObject* arg, void*) {
	if (PyNumber_Check(arg)) {
		Actor* a = PyEntity::asActor(self);
		if (!a)
			return -1;
		a->setHealth(static_cast<float>(PyFloat_AsDouble(arg)));
		return 0;
	}
	return PyErr_BadArgument(), -1;
}
static PyObject* getMaxHealth(PyObject* self, void*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	return PyFloat_FromDouble(static_cast<double>(a->getMaxHealth()));
}
static int setMaxHealth(PyObject* self, PyObject* arg, void*) {
	if (PyNumber_Check(arg)) {
		Actor* a = PyEntity::asActor(self);
		if (!a)
			return -1;
		a->setMaxHealth(static_cast<float>(PyFloat_AsDouble(arg)));
		return 0;
	}
	return PyErr_BadArgument(), -1;
}
static PyObject* getPermissions(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	return PyLong_FromLong(static_cast<int>(p->getPlayerPermissionLevel()));
}
static int setPermissions(PyObject* self, PyObject* arg, void*) {
	if (PyLong_Check(arg)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return -1;
		p->setPermissions(static_cast<PlayerPermissionLevel>(PyLong_AsLong(arg)));
		return 0;
	}
	return PyErr_BadArgument(), -1;
}
static PyObject* getPlatformOnlineId(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	return ToPyStr(p->getPlatformOnlineId());
}
static PyObject* getPlatform(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	return PyLong_FromLong(p->getPlatform());
}
static PyObject* getIP(PyObject* self, void*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	auto ni = p->getClientId();
	return ToPyStr(global<RakPeer>->getSystemAddress(ni).toString());
}
};

PyObject* PyEntity_GetAllItem(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	Json value;

	Json& inventory = value["Inventory"];
	for (auto& i : p->getInventory()->getSlots()) {
		inventory.push_back(CompoundTagtoJson(i->save()));
	}

	Json& endchest = value["EndChest"];
	for (auto& i : p->getEnderChestContainer()->getSlots()) {
		endchest.push_back(CompoundTagtoJson(i->save()));
	}

	Json& armor = value["Armor"];
	for (auto& i : p->getArmorContainer()->getSlots()) {
		armor.push_back(CompoundTagtoJson(i->save()));
	}

	value["OffHand"] = CompoundTagtoJson(p->getOffhandSlot()->save());
	value["Hand"] = CompoundTagtoJson(p->getSelectedItem()->save());

	return ToPyStr(value.dump(4));
}

PyObject* PyEntity_SetAllItem(PyObject* self, PyObject* args) {
	const char* x = "";
	if (PyArg_ParseTuple(args, "s:setAllItem", &x)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		Json value(StringToJson(x));

		if (value.contains("Inventory")) {
			const vector<ItemStack*>& items = p->getInventory()->getSlots();
			Json& inventory = value["Inventory"];
			for (unsigned i = 0; i < inventory.size(); i++) {
				items[i]->fromJson(inventory[i]);
			}
		}

		if (value.contains("EndChest")) {
			const vector<ItemStack*>& items = p->getEnderChestContainer()->getSlots();
			Json& endchest = value["EndChest"];
			for (unsigned i = 0; i < endchest.size(); i++) {
				items[i]->fromJson(endchest[i]);
			}
		}

		if (value.contains("Armor")) {
			const vector<ItemStack*>& items = p->getArmorContainer()->getSlots();
			Json& armor = value["Armor"];
			for (unsigned i = 0; i < armor.size(); i++) {
				items[i]->fromJson(armor[i]);
			}
		}

		if (value.contains("OffHand")) {
			p->getOffhandSlot()->fromJson(value["OffHand"]);
		}
		p->sendInventroy();
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SetHand(PyObject* self, PyObject* args) {
	const char* x;
	if (PyArg_ParseTuple(args, "s:setHand", &x)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		Json json(StringToJson(x));
		p->getSelectedItem()->fromJson(json);
		p->sendInventroy();
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_AddItem(PyObject* self, PyObject* args) {
	const char* x;
	if (PyArg_ParseTuple(args, "s:addItem", &x)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		ItemStack i;
		i.fromJson(StringToJson(x));
		p->addItem(&i);
		p->sendInventroy();
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_RemoveItem(PyObject* self, PyObject* args) {
	int slot, num;
	if (PyArg_ParseTuple(args, "ii:removeItem", &slot, &num)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->getInventory()->clearItem(slot, num);
		p->sendInventroy();
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_OpenInventoryGUI(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	p->openInventoryGUI();
	Py_RETURN_NONE;
}

PyObject* PyEntity_CloseContainerGUI(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	p->doDeleteContainerManager();
	Py_RETURN_NONE;
}

PyObject* PyEntity_Teleport(PyObject* self, PyObject* args) {
	Vec3 pos; int did;
	if (PyArg_ParseTuple(args, "fffi:teleport", &pos.x, &pos.y, &pos.z, &did)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->teleport(&pos, did);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SendTextPacket(PyObject* self, PyObject* args) {
	const char* msg;
	int mode = 0;
	if (PyArg_ParseTuple(args, "s|i:sendTextPacket", &msg, &mode)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendTextPacket(mode, msg);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SendCommandPacket(PyObject* self, PyObject* args) {
	const char* cmd;
	if (PyArg_ParseTuple(args, "s:sendCommandPacket", &cmd)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendCommandRequestPacket(cmd);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_ResendAllChunks(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	p->resendAllChunks();
	Py_RETURN_NONE;
}

PyObject* PyEntity_Disconnect(PyObject* self, PyObject* args) {
	const char* msg;
	if (PyArg_ParseTuple(args, "|s:disconnect", &msg)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendDisconnectPacket(msg);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_GetScore(PyObject* self, PyObject* args) {
	const char* objname;
	if (PyArg_ParseTuple(args, "s:getScore", &objname)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		Objective* testobj = global<Scoreboard>->getObjective(objname);
		if (testobj) {
			auto id = global<Scoreboard>->getScoreboardId(p);
			auto score = testobj->getPlayerScore(id);
			return PyLong_FromLong(score->getCount());
		}
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_ModifyScore(PyObject* self, PyObject* args) {
	const char* objname; int count; int mode;
	if (PyArg_ParseTuple(args, "sii:modifyScore", &objname, &count, &mode)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		Objective* testobj = global<Scoreboard>->getObjective(objname);
		if (testobj) {
			//mode:{set,add,remove}
			global<Scoreboard>->modifyPlayerScore(global<Scoreboard>->getScoreboardId(p), testobj, count, mode);
		}
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_AddLevel(PyObject* self, PyObject* args) {
	int level;
	if (PyArg_ParseTuple(args, "i:addLevel", &level)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->addLevel(level);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_TransferServer(PyObject* self, PyObject* args) {
	const char* address;
	unsigned short port;
	if (PyArg_ParseTuple(args, "sH:transferServer", &address, &port)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendTransferPacket(address, port);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SendCustomForm(PyObject* self, PyObject* args) {
	const char* str;
	PyObject* callback = nullptr;
	if (PyArg_ParseTuple(args, "s|O:sendCustomForm", &str, &callback)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		auto formid = p->sendModalFormRequestPacket(str);
		if (callback)
			g_forms[formid] = callback;
		return PyLong_FromLong(formid);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SendSimpleForm(PyObject* self, PyObject* args) {
	const char* title;
	const char* content;
	const char* buttons;
	PyObject* callback = nullptr;
	if (PyArg_ParseTuple(args, "sss|O:sendSimpleForm", &title, &content, &buttons, &callback)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		char str[4096];
		sprintf_s(str, 4096, R"({"title":"%s","content":"%s","buttons":%s,"type":"form"})", title, content, buttons);
		auto formid = p->sendModalFormRequestPacket(str);
		if (callback)
			g_forms[formid] = callback;
		return PyLong_FromLong(formid);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SendModalForm(PyObject* self, PyObject* args) {
	const char* title;
	const char* content;
	const char* button1;
	const char* button2;
	PyObject* callback = nullptr;
	if (PyArg_ParseTuple(args, "ssss|O:sendModalForm", &title, &content, &button1, &button2, &callback)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		char str[4096];
		sprintf_s(str, 4096, R"({"title":"%s","content":"%s","button1":"%s","button2":"%s","type":"modal"})", title, content, button1, button2);
		auto formid = p->sendModalFormRequestPacket(str);
		if (callback)
			g_forms[formid] = callback;
		return PyLong_FromLong(formid);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_SetSideBar(PyObject* self, PyObject* args) {
	const char* title;
	const char* data;
	if (PyArg_ParseTuple(args, "ss:setSidebar", &title, &data)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendsetDisplayObjectivePacket(title);
		Json value = StringToJson(data);
		vector<ScorePacketInfo> info;
		if (value.is_object())
			for (auto& [key, val] : value.items()) {
				ScorePacketInfo o(global<Scoreboard>->createScoreBoardId(key),
					val.get<int>(), key);
				info.push_back(o);
			}
		p->sendSetScorePacket(0, info);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_RemoveSideBar(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	p->sendsetDisplayObjectivePacket("", "");
	Py_RETURN_NONE;
}

PyObject* PyEntity_SetBossBar(PyObject* self, PyObject* args) {
	const char* name;
	float per;
	if (PyArg_ParseTuple(args, "sf:setBossbar", &name, &per)) {
		Player* p = PyEntity::asPlayer(self);
		if (!p)
			return nullptr;
		p->sendBossEventCodePacket(name, per, 0);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_RemoveBossBar(PyObject* self, PyObject*) {
	Player* p = PyEntity::asPlayer(self);
	if (!p)
		return nullptr;
	p->sendBossEventCodePacket("", 0, 2);
	Py_RETURN_NONE;
}

PyObject* PyEntity_AddTag(PyObject* self, PyObject* args) {
	const char* tag;
	if (PyArg_ParseTuple(args, "s:addTag", &tag)) {
		Actor* a = PyEntity::asActor(self);
		if (!a)
			return nullptr;
		a->addTag(tag);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_RemoveTag(PyObject* self, PyObject* args) {
	const char* tag;
	if (PyArg_ParseTuple(args, "s:removeTag", &tag)) {
		Actor* a = PyEntity::asActor(self);
		if (!a)
			return nullptr;
		a->removeTag(tag);
	}
	Py_RETURN_NONE;
}

PyObject* PyEntity_GetTags(PyObject* self, PyObject*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	span<string> tags = a->getTags();
	PyObject* list = PyList_New(0);
	for (size_t i = 0; i < tags.size; i++) {
		PyList_Append(list, ToPyStr(tags.data[i]));
	}
	return list;
}

PyObject* PyEntity_Kill(PyObject* self, PyObject*) {
	Actor* a = PyEntity::asActor(self);
	if (!a)
		return nullptr;
	a->kill();
	Py_RETURN_NONE;
}

PyGetSetDef PyEntity_GetSet[]{
	{"name", PyEntity::getName, PyEntity::setName, nullptr},
	{"uuid", PyEntity::getUuid, nullptr, nullptr},
	{"xuid", PyEntity::getXuid, nullptr, nullptr},
	{"pos", PyEntity::getPos, nullptr, nullptr},
	{"did", PyEntity::getDimensionId, nullptr, nullptr},
	{"is_standing", PyEntity::getIsStand, nullptr, nullptr},
	{"is_sneaking", PyEntity::getIsSneaking, nullptr, nullptr},
	{"typeid", PyEntity::getTypeID, nullptr, nullptr},
	{"typename", PyEntity::getTypeName, nullptr, nullptr},
	{"NBT", PyEntity::getNBTInfo, nullptr, nullptr},
	{"health", PyEntity::getHealth, PyEntity::setHealth, nullptr},
	{"maxhealth", PyEntity::getMaxHealth, PyEntity::setMaxHealth, nullptr},
	{"perm", PyEntity::getPermissions, PyEntity::setPermissions, nullptr},
	{"platform_online_id", PyEntity::getPlatformOnlineId, nullptr, nullptr},
	{"platform", PyEntity::getPlatform, nullptr, nullptr},
	{"IP", PyEntity::getIP, nullptr, nullptr},
	/*Deprecated*/{"ip", PyEntity::getIP, nullptr, nullptr},
	/*Deprecated*/{"nbt", PyEntity::getNBTInfo, nullptr, nullptr},
	/*Deprecated*/{"isstand", PyEntity::getIsStand, nullptr, nullptr},
	/*Deprecated*/{"issneak", PyEntity::getIsSneaking, nullptr, nullptr},
	/*Deprecated*/{"deviceos", PyEntity::getPlatform, nullptr, nullptr},
	/*Deprecated*/{"deviceid", PyEntity::getPlatformOnlineId, nullptr, nullptr},
	{nullptr}
};
//Entity Methods
PyMethodDef PyEntity_Methods[]{
	{"getAllItem", PyEntity_GetAllItem, METH_VARARGS, nullptr},
	{"setAllItem", PyEntity_SetAllItem, METH_VARARGS, nullptr},
	{"setHand", PyEntity_SetHand, METH_VARARGS, nullptr},
	{"addItem", PyEntity_AddItem, METH_VARARGS, nullptr},
	{"removeItem", PyEntity_RemoveItem, METH_VARARGS, nullptr},
	{"openInventoryGUI", PyEntity_OpenInventoryGUI, METH_VARARGS, nullptr},
	/*not working*/{"closeContainerGUI", PyEntity_CloseContainerGUI, METH_VARARGS, nullptr},
	{"teleport", PyEntity_Teleport, METH_VARARGS, nullptr},
	{"sendTextPacket", PyEntity_SendTextPacket, METH_VARARGS, nullptr},
	{"sendCommandPacket", PyEntity_SendCommandPacket, METH_VARARGS, nullptr},
	{"resendAllChunks", PyEntity_ResendAllChunks, METH_NOARGS, nullptr},
	{"disconnect", PyEntity_Disconnect, METH_VARARGS, nullptr},
	{"getScore", PyEntity_GetScore, METH_VARARGS, nullptr},
	{"modifyScore", PyEntity_ModifyScore, METH_VARARGS, nullptr},
	{"addLevel", PyEntity_AddLevel, METH_VARARGS, nullptr},
	{"transferServer", PyEntity_TransferServer, METH_VARARGS, nullptr},
	{"sendCustomForm", PyEntity_SendCustomForm, METH_VARARGS, nullptr},
	{"sendSimpleForm", PyEntity_SendSimpleForm, METH_VARARGS, nullptr},
	{"sendModalForm", PyEntity_SendModalForm, METH_VARARGS, nullptr},
	{"setSideBar", PyEntity_SetSideBar, METH_VARARGS, nullptr},
	{"removeSideBar", PyEntity_RemoveSideBar, METH_NOARGS, nullptr},
	{"setBossBar", PyEntity_SetBossBar, METH_VARARGS, nullptr},
	{"removeBossBar", PyEntity_RemoveBossBar, METH_NOARGS, nullptr},
	{"addTag", PyEntity_AddTag, METH_VARARGS, nullptr},
	{"removeTag", PyEntity_RemoveTag, METH_VARARGS, nullptr},
	{"getTags", PyEntity_GetTags, METH_NOARGS, nullptr},
	{"kill", PyEntity_Kill, METH_NOARGS, nullptr},
	{nullptr}
};
//Entity Types
PyTypeObject PyEntity_Type{
	PyVarObject_HEAD_INIT(nullptr, 0)
	"Entity",				/* tp_name */
	sizeof(PyEntity),		/* tp_basicsize */
	0,						/* tp_itemsize */
	nullptr,				/* tp_dealloc */
	(Py_ssize_t)PyEntity::print,	/* tp_print */
	nullptr,				/* tp_getattr */
	nullptr,				/* tp_setattr */
	nullptr,				/* tp_reserved */
	PyEntity::repr,			/* tp_repr */
	nullptr,				/* tp_as_number */
	nullptr,				/* tp_as_sequence */
	nullptr,				/* tp_as_mapping */
	PyEntity::hash,			/* tp_hash */
	nullptr,				/* tp_call */
	PyEntity::str,			/* tp_str */
	nullptr,				/* tp_getattro */
	nullptr,				/* tp_setattro */
	nullptr,				/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,		/* tp_flags */
	"Entities in Minecraft",/* tp_doc */
	nullptr,				/* tp_traverse */
	nullptr,				/* tp_clear */
	PyEntity::rich_compare,	/* tp_richcompare */
	0,						/* tp_weaklistoffset */
	nullptr,				/* tp_iter */
	nullptr,				/* tp_iternext */
	PyEntity_Methods,		/* tp_methods */
	nullptr,				/* tp_members */
	PyEntity_GetSet,		/* tp_getset */
	nullptr,				/* tp_base */
	nullptr,				/* tp_dict */
	nullptr,				/* tp_descr_get */
	nullptr,				/* tp_descr_set */
	0,						/* tp_dictoffset */
	nullptr,				/* tp_init */
	nullptr,				/* tp_alloc */
	nullptr,				/* tp_new */
	nullptr,				/* tp_free */
	nullptr,				/* tp_is_gc */
	nullptr,				/* tp_bases */
	nullptr,				/* tp_mro */
	nullptr,				/* tp_cache */
	nullptr,				/* tp_subclasses */
	nullptr,				/* tp_weaklist */
	nullptr,				/* tp_del */
	0,						/* tp_version_tag */
	nullptr,				/* tp_finalize */
};

PyObject* ToEntity(Actor* ptr) {
	PyEntity* obj = nullptr;
	//Py_BEGIN_CALL;
	obj = PyObject_New(PyEntity, &PyEntity_Type);
	//Py_END_CALL;
	obj->actor = ptr;
	return reinterpret_cast<PyObject*>(obj);
}
