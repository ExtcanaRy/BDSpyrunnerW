#pragma once
#pragma comment(lib,"lib/python37.lib")
#define PY_SSIZE_T_CLEAN
#include "../include/Python.h"

//�ַ���תUnicode
inline PyObject* StringToPyUnicode(std::string_view str) {
	return PyUnicode_FromStringAndSize(str.data(), str.length());
}
//��ӡ������Ϣ
inline void PrintPythonError() {
	if (PyErr_Occurred()) {
		PyErr_Print();
	}
}