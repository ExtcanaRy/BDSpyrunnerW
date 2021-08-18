#pragma once
#define PY_SSIZE_T_CLEAN
#include "include/Python.h"

#define Py_RETURN_ERROR(str) return PyErr_SetString(PyExc_Exception, str), nullptr
#define Py_CALL_BEGIN\
	int _has_gil = PyGILState_Check();\
	PyGILState_STATE _gil_state = PyGILState_LOCKED;\
	if (!_has_gil)_gil_state = PyGILState_Ensure();\
	Py_BEGIN_ALLOW_THREADS;\
	Py_BLOCK_THREADS
#define Py_CALL_END\
	Py_UNBLOCK_THREADS;\
	Py_END_ALLOW_THREADS;\
	if (!_has_gil)PyGILState_Release(_gil_state)

struct Actor;
struct Player;
// ʵ������
struct PyEntity {
	PyObject_HEAD;
	Actor* actor;
};

Actor* PyEntity_AsActor(PyObject* self);
Player* PyEntity_AsPlayer(PyObject* self);

//��ʼ��
PyObject* PyEntity_New(PyTypeObject* type, PyObject*, PyObject*);
//���캯��
int PyEntity_Init(PyObject* self, PyObject* args, PyObject*);
//����
void PyEntity_Dealloc(PyObject* obj);
//ת�ַ���
PyObject* PyEntity_Str(PyObject* self);
//��ϣ
Py_hash_t PyEntity_Hash(PyObject* self);
//�Ƚ�
PyObject* PyEntity_RichCompare(PyObject* self, PyObject* other, int op);

//��ȡ����
PyObject* PyEntity_GetName(PyObject* self, void*);
int PyEntity_SetName(PyObject* self, PyObject* arg, void*);
//��ȡUUID
PyObject* PyEntity_GetUuid(PyObject* self, void*);
//��ȡXUID
PyObject* PyEntity_GetXuid(PyObject* self, void*);
//��ȡ����
PyObject* PyEntity_GetPos(PyObject* self, void*);
//��ȡά��ID
PyObject* PyEntity_GetDimensionId(PyObject* self, void*);
//�Ƿ��ŵ�
PyObject* PyEntity_GetIsStand(PyObject* self, void*);
//�Ƿ�Ǳ��
PyObject* PyEntity_GetIsSneaking(PyObject* self, void*);
//��ȡ����
PyObject* PyEntity_GetTypeID(PyObject* self, void*);
//��ȡ�����ַ���
PyObject* PyEntity_GetTypeName(PyObject* self, void*);
//��ȡnbt����
PyObject* PyEntity_GetNBTInfo(PyObject* self, void*);
//��ȡ����ֵ
PyObject* PyEntity_GetHealth(PyObject* self, void*);
int PyEntity_SetHealth(PyObject* self, PyObject* arg, void*);
//��ȡ�������ֵ
PyObject* PyEntity_GetMaxHealth(PyObject* self, void*);
int PyEntity_SetMaxHealth(PyObject* self, PyObject* arg, void*);
//��ȡȨ��
PyObject* PyEntity_GetPermissions(PyObject* self, void*);
int PyEntity_SetPermissions(PyObject* self, PyObject* arg, void*);
//��ȡ�豸id
PyObject* PyEntity_GetDeviceId(PyObject* self, void*);
//��ȡ�豸����
PyObject* PyEntity_GetDeviceOS(PyObject* self, void*);
//��ȡIP
PyObject* PyEntity_GetIP(PyObject* self, void*);

//��ȡ/�������������Ʒ
PyObject* PyEntity_GetAllItem(PyObject* self, PyObject*);
PyObject* PyEntity_SetAllItem(PyObject* self, PyObject* args);
PyObject* PyEntity_SetHand(PyObject* self, PyObject* args);
//����/�Ƴ���Ʒ
PyObject* PyEntity_AddItem(PyObject* self, PyObject* args);
PyObject* PyEntity_RemoveItem(PyObject* self, PyObject* args);
//����
PyObject* PyEntity_Teleport(PyObject* self, PyObject* args);
//�������ݰ�
PyObject* PyEntity_SendTextPacket(PyObject* self, PyObject* args);
PyObject* PyEntity_SendCommandPacket(PyObject* self, PyObject* args);
PyObject* PyEntity_ResendAllChunks(PyObject* self, PyObject*);
PyObject* PyEntity_Disconnect(PyObject* self, PyObject* args);
//�Ʒְ����
PyObject* PyEntity_GetScore(PyObject* self, PyObject* args);
PyObject* PyEntity_ModifyScore(PyObject* self, PyObject* args);
//���ӵȼ�
PyObject* PyEntity_AddLevel(PyObject* self, PyObject* args);
//�������
PyObject* PyEntity_TransferServer(PyObject* self, PyObject* args);
//���ͱ�
PyObject* PyEntity_SendCustomForm(PyObject* self, PyObject* args);
PyObject* PyEntity_SendSimpleForm(PyObject* self, PyObject* args);
PyObject* PyEntity_SendModalForm(PyObject* self, PyObject* args);
//���ò����
PyObject* PyEntity_SetSidebar(PyObject* self, PyObject* args);
PyObject* PyEntity_RemoveSidebar(PyObject* self, PyObject*);
//Boss��
PyObject* PyEntity_SetBossbar(PyObject* self, PyObject* args);
PyObject* PyEntity_RemoveBossbar(PyObject* self, PyObject*);
//��ǩ
PyObject* PyEntity_AddTag(PyObject* self, PyObject* args);
PyObject* PyEntity_RemoveTag(PyObject* self, PyObject* args);
PyObject* PyEntity_GetTags(PyObject* self, PyObject*);

PyObject* PyEntity_FromEntity(Actor* ptr);

//��ȡ���Է���
extern PyGetSetDef PyEntity_GetSet[];
//Entity����
extern PyMethodDef PyEntity_Methods[];
//Entity����
extern PyTypeObject PyEntity_Type;
