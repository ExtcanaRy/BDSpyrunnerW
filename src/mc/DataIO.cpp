#include <mc/DataIO.h>
#include <mc/tool.h>

BinaryStream::BinaryStream() {
	SymCall("??0BinaryStream@@QEAA@XZ",
		this);
}

BinaryStream::BinaryStream(std::string* buffer, bool copy) {
	SymCall("??0BinaryStream@@QEAA@AEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
		this, buffer, copy);
}

std::string* BinaryStream::GetAndReleaseData() {
	return SymCall<std::string*>("?getAndReleaseData@BinaryStream@@QEAA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@XZ",
		this);
}

void BinaryStream::writeUnsignedInt(unsigned int num, const char *str1, const char *str2) {
	SymCall("?writeUnsignedInt@BinaryStream@@QEAAXIPEBD0@Z", this, num, str1, str2);
}

BinaryStream::~BinaryStream() {
	SymCall("??1BinaryStream@@UEAA@XZ",
		this);
}

ReadOnlyBinaryStream::ReadOnlyBinaryStream() {}

ReadOnlyBinaryStream::ReadOnlyBinaryStream(std::string* buffer) {
	SymCall("??0ReadOnlyBinaryStream@@QEAA@$$QEAV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",
		this, buffer);
}

ReadOnlyBinaryStream::ReadOnlyBinaryStream(const std::string* buffer, bool copyBuffer) {
	SymCall("??0ReadOnlyBinaryStream@@QEAA@AEBV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@_N@Z",
		this, buffer, copyBuffer);
}

ReadOnlyBinaryStream::~ReadOnlyBinaryStream() {
	SymCall("??1ReadOnlyBinaryStream@@UEAA@XZ", this);
}
