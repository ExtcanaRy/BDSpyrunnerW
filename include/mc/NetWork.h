#pragma once
#include <string>

struct NetworkIdentifier {
	char _this[144];
	std::string getAddress();
	std::string toString();
};
struct SystemAddress {
	char _this[136];
	SystemAddress();
	std::string toString();
};
struct RakPeer {
	SystemAddress getSystemAddress(NetworkIdentifier* ni);
};
struct ServerNetworkHandler {
	struct Player* _getServerPlayer(const struct NetworkIdentifier* id, uintptr_t pkt);
};
struct RakNetGUID {
	uint64_t unk;
	short    unk8;
};
