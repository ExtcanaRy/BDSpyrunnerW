#include <global.h>

using namespace std;
//ָ�����
SPSCQueue* g_command_queue = nullptr;
//���紦��
ServerNetworkHandler* g_server_network_handler = nullptr;
//����
Level* g_level = nullptr;
//����
RakPeer* g_rak_peer = nullptr;
//�Ʒְ�
Scoreboard* g_scoreboard = nullptr;
//Py������
unordered_map<EventCode, vector<PyObject*>> g_callback_functions;
//ע������
unordered_map<string, pair<string, PyObject*>> g_commands;
//�˺�
int g_damage = 0;
