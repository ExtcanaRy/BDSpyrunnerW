#include <mc/Structure.h>
#include <mc/tool.h>
#include <mc/Tag.h>

using namespace std;
StructureSettings::StructureSettings(BlockPos* size, bool IgnoreEntities, bool IgnoreBlocks) {
	SymCall("??0StructureSettings@@QEAA@XZ", this);
	Dereference<bool>( _this + 32) = IgnoreEntities;
	Dereference<bool>( _this + 34) = IgnoreBlocks;
	Dereference<BlockPos>( _this + 36) = *size;
	Dereference<BlockPos>( _this + 48) = { 0,0,0 };
}

StructureSettings::~StructureSettings() {
	reinterpret_cast<string*>(this)->~basic_string();
}

StructureTemplate::StructureTemplate(std::string_view s) {
	char v32[16];
	Level *Camera = global<Level>;
	uintptr_t v11 = VirtualCall<uintptr_t>(2440, Camera, v32);
	SymCall("??0StructureTemplate@@QEAA@V?$basic_string_view@DU?$char_traits@D@std@@@std@@V?$NonOwnerPointer@VIUnknownBlockTypeRegistry@@@Bedrock@@@Z",
		this, s, v11);
}

StructureTemplate::~StructureTemplate() {
	SymCall("??1StructureTemplate@@UEAA@XZ", this);
}

Tag* StructureTemplate::save() {
	__int64 v41[1][1]{0};
	return *SymCall<Tag**>("?save@StructureTemplate@@QEBA?AV?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@std@@XZ",
		this, v41);
	/*Tag* t = 0;
	SymCall<Tag*>("?save@StructureTemplateData@@QEBA?AV?$unique_ptr@VCompoundTag@@U?$default_delete@VCompoundTag@@@std@@@std@@XZ",
		_this + 32, &t);
	return t;*/
}

void StructureTemplate::load(Tag* t) {
	SymCall<bool>("?load@StructureTemplateData@@QEAA_NAEBVCompoundTag@@@Z",
		_this + 32, t);
}

void StructureTemplate::fromJson(const Json& value) {
	Tag* t = ObjecttoTag(value);
	load(t);
	t->deleteCompound();
	delete t;
}

void StructureTemplate::fromCompound(Tag* t) {
	load(t);
}

void StructureTemplate::fillFromWorld(BlockSource* a2, BlockPos* a3, StructureSettings* a4) {
	SymCall("?fillFromWorld@StructureTemplate@@QEAAXAEAVBlockSource@@AEBVBlockPos@@AEBVStructureSettings@@@Z",
		this, a2, a3, a4);
}

void StructureTemplate::placeInWorld(BlockSource* a2, BlockPalette* a3, BlockPos* a4, StructureSettings* a5) {
	SymCall("?placeInWorld@StructureTemplate@@QEBAXAEAVBlockSource@@AEBVBlockPalette@@AEBVBlockPos@@AEBVStructureSettings@@PEAVStructureTelemetryServerData@@_N@Z",
		this, a2, a3, a4, a5);
}
