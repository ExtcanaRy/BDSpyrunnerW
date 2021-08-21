#pragma once
#include <string>
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
//ת��std::stringΪPyUnicode
PyObject* toPyUnicode(const std::string&);

PyObject* PyEntity_FromEntity(Actor* ptr);

//��ȡ���Է���
extern PyGetSetDef PyEntity_GetSet[];
//Entity����
extern PyMethodDef PyEntity_Methods[];
//Entity����
extern PyTypeObject PyEntity_Type;
