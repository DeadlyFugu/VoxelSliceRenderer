#pragma once
#include <map>
#include <fstream>
#include <iostream>
#include <string>

class Config {
private:
	std::map<std::string, std::string> map_string;
	std::map<std::string, int> map_int;
	std::map<std::string, float> map_float;
	std::map<std::string, float> map_bool;
public:
	std::string get_string(std::string key, const char* def) {
		auto value = map_string.find(key);
		if (value == map_string.end()) {
			return std::string(def);
		} else {
			return value->second;
		}
	}

	int get_int(std::string key, int def) {
		auto value = map_int.find(key);
		if (value == map_int.end()) {
			return def;
		} else {
			return value->second;
		}
	}

	float get_float(std::string key, float def) {
		auto value = map_float.find(key);
		if (value == map_float.end()) {
			return def;
		} else {
			return value->second;
		}
	}

	bool get_bool(std::string key, bool def) {
		auto value = map_bool.find(key);
		if (value == map_bool.end()) {
			return def;
		} else {
			return value->second;
		}
	}

	void set_string(std::string key, const char* value) {
		map_string[key] = std::string(value);
	}

	void set_int(std::string key, int value) {
		map_int[key] = value;
	}

	void set_float(std::string key, float value) {
		map_float[key] = value;
	}

	void set_bool(std::string key, bool value) {
		map_bool[key] = value;
	}

	bool has_key(std::string key) {
		return map_string.count(key) +
			map_int.count(key) +
			map_float.count(key) +
			map_bool.count(key) > 0;
	}

	void load(const char* path) {
		std::ifstream in(path);
		if (!in) {
			std::cout << "ERROR: settings.conf not found" << std::endl;
		}
		std::string line;
		int ln = 0;
		while (std::getline(in, line)) {
			std::cout << "LINE(" << ln << "): " << line << std::endl;
			if (line[0] == '#') continue;
			ln++;
			int delim = line.find_first_of(',');
			if (delim == std::string::npos) {
				std::cout << "WARNING: Comma missing on line " << ln << std::endl;
			}
			std::cout << "line.substr(0," << delim << ") => " << line.substr(0, delim).c_str() << std::endl;
			std::string key = line.substr(0, delim).c_str();
			std::cout << "key = " << key << ";" << std::endl;
			std::string value = line.substr(delim + 1, line.size() - delim - 1);
			std::cout << "[K] = V -- [" << key << "] = " << value << std::endl;
			auto trim_start = value.find_first_not_of(' ');
			auto trim_end = value.find_last_not_of(' ');
			if (trim_start == std::string::npos) {
				std::cout << "WARNING: Value missing on line " << ln << std::endl;
				continue;
			}
			value = value.substr(trim_start, trim_end - trim_start + 1);

			if (value[0] == '"') {
				if (value.back() != '"') {
					std::cout << "WARNING: Unclosed quote on line " << ln << std::endl;
				}
				set_string(key, value.substr(1, value.size() - 2).c_str());
			} else if ((value[0] >= '0' && value[0] <= '9') || value[0] == '-') {
				if (value.find('.') != std::string::npos) {
					std::cout << "ADD ENTRY: " << key << " = " << value << std::endl;
					set_float(key, std::stof(value));
				} else {
					set_int(key, std::stoi(value));
				}
			} else if (value == "true") {
				set_bool(key, true);
			} else if (value == "false") {
				set_bool(key, false);
			} else {
				std::cout << "WARNING: Unrecognised type of '" << value << "' on line " << ln << std::endl;
			}
		}
	}

	void save(const char* path) {
		std::ofstream out(path);
		for (auto entry : map_string) {
			out << entry.first << ", \"" << entry.second << "\"" << std::endl;
		}
		for (auto entry : map_int) {
			out << entry.first << ", " << entry.second << std::endl;
		}
		for (auto entry : map_float) {
			out << entry.first << ", " << entry.second << std::endl;
		}
		for (auto entry : map_bool) {
			out << entry.first << ", " << (entry.second ? "true" : "false") << std::endl;
		}
	}

	void debug_print() {
		using std::cout;

		cout << "strings:" << std::endl;
		for (auto entry : map_string) {
			cout << entry.first << ", \"" << entry.second << "\"" << std::endl;
		}
		cout << "ints:" << std::endl;
		for (auto entry : map_int) {
			cout << entry.first << ", " << entry.second << std::endl;
		}
		cout << "floats:" << std::endl;
		for (auto entry : map_float) {
			cout << entry.first << ", " << entry.second << std::endl;
		}
		cout << "bools:" << std::endl;
		for (auto entry : map_bool) {
			cout << entry.first << ", " << (entry.second ? "true" : "false") << std::endl;
		}
	}
};