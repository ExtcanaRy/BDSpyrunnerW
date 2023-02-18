#pragma once
#include "Position.h"

struct InventoryTransaction {
	char filler[8];
	int transactionType;
	char filler2[58 * 4 - 4 - 8];
	Vec3 playerPosition;
	Vec3 clickPosition;
};
struct InventoryTransactionPacket {
	char filler[88];
	InventoryTransaction* transaction;
};