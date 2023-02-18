#pragma once
#include <iostream>
#undef snprintf
#include <nlohmann/json.hpp>

using Json = nlohmann::basic_json<>;
using JsonType = nlohmann::detail::value_t;
// nlohmann from https://json.nlohmann.me , Version 3.11.2
inline Json StringToJson(std::string_view str) {
	try {
		return Json::parse(str);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return nullptr;
	}
}
