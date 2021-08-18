#pragma once
#include <unordered_map>
#include <vector>
#include <string>

typedef struct _object PyObject;
//ָ�����
extern struct SPSCQueue* g_command_queue;
//���紦��
extern struct ServerNetworkHandler* g_server_network_handler;
//����
extern struct Level* g_level;
//����
extern struct RakPeer* g_rak_peer;
//�Ʒְ�
extern struct Scoreboard* g_scoreboard;
//Py������
extern std::unordered_map<enum class EventCode, std::vector<PyObject*>> g_callback_functions;
//ע������
extern std::unordered_map<std::string, std::pair<std::string, PyObject*>> g_commands;
//�˺�
extern int g_damage;
