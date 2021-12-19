#pragma once
#include <iostream>
#include "json.hpp"

using json = nlohmann::basic_json<>;
using json_t = nlohmann::detail::value_t;
//�ַ���תJSON����������� https://json.nlohmann.me ��JSON��3.10.4�汾
inline json StringToJson(std::string_view str) {
	try { return json::parse(str); }
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}
