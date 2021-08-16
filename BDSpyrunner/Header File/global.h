#pragma once
#include "Actor.h"
#include "Block.h"
#include "ItemStack.h"
#include "Level.h"
#include "NetWork.h"
#include "ScoreBoard.h"
#include "Structure.h"
#include "Tag.h"
#include "Event.h"
typedef struct _object PyObject;
//ָ�����
extern SPSCQueue* g_command_queue;
//���紦��
extern ServerNetworkHandler* g_server_network_handler;
//����
extern Level* g_level;
//����
extern RakPeer* g_rak_peer;
//�Ʒְ�
extern Scoreboard* g_scoreboard;
//Py������
extern unordered_map<EventCode, std::vector<PyObject*>> g_callback_functions;
//ע������
extern unordered_map<string, std::pair<const char*, PyObject*>> g_commands;
//�˺�
extern int g_damage;
