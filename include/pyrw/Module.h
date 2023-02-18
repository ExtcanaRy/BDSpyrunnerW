#pragma once
#include <unordered_map>
#include <vector>
#include "Entity.h"
#include "Event.h"


inline std::unordered_map<EventCode, std::vector<PyObject*>> g_callback_functions;

inline std::unordered_map<std::string, std::pair<std::string, PyObject*>> g_commands;

inline static float g_damage = 0;

extern "C" PyObject * mc_init();

__int64 getCurrentTime();

void ReloadPythonModules(std::string moduleName);