#pragma once
#include <string>
#include "CPython.h"
//Entity����
extern PyTypeObject PyEntity_Type;
PyObject* ToEntity(struct Actor* ptr);
