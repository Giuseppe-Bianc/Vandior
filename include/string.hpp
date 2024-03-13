#pragma once
#include "primitives.hpp"

class string {
	public:
		string(const std::string &s = ""): str(string::create(s)) {}
		string(const string &s): str(string::create(*(s.str))) {}
		~string() {
			string::deleteString(str);
		}
		const i64 size() const {
			return str->size();
		}
		const char at(i64 index) const {
			if(index < 0) {
				index += static_cast<i64>(size());
			}
			if(index < 0 || index >= size()) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(size())); }
			return str->at(index);
		}
		const bool empty() {
			return str->empty();
		}
		const i32 toI32() {
			return static_cast<i32>(std::stoi(*str));
		}
		const f32 toF32() {
			return static_cast<f32>(std::stof(*str));
		}
		/*const c32 toC32() {
			std::istringstream iss(*str);
			float realPart, imaginaryPart;
			if (iss >> realPart >> std::ws && iss.peek() == '+' && iss >> std::ws && iss >> imaginaryPart >> std::ws && iss.peek() == 'i') {
				return c32(realPart, imaginaryPart);
			}
			return c32(0, 0);
		}*/
		std::string::const_iterator begin() const noexcept {
			return str->begin();
		}
		std::string::const_iterator end() const noexcept {
			return str->end();
		}
		string operator=(const string &s) {
			if (this != &s) {
				std::shared_ptr<const std::string> old = str;
				str = string::create(*(s.str));
				string::deleteString(old);
			}
			return *this;
		}
		friend bool operator==(const string &s1, const string &s2) {
			return *s1.str == *s2.str;
		}
		friend std::ostream& operator<<(std::ostream& os, const string &s) {
			os << *(s.str);
			return os;
		}
		operator std::string() {
			return *str;
		}
	private:
		static std::vector<std::pair<std::shared_ptr<const std::string>, i64>> pool;
		static std::shared_ptr<const std::string> create(const std::string &str) {
			for(auto &i: string::pool) {
				if(str == *i.first) {
					i.second++;
					return i.first;
				}
			}
			string::pool.emplace_back(std::make_pair(std::make_shared<std::string>(str), 1));
			return string::pool.back().first;
		}
		static void deleteString(const std::shared_ptr<const std::string> str) {
			auto iterator = string::pool.begin();
			while(iterator != string::pool.end()) {
				if(str == iterator->first) {
					iterator->second--;
					if(iterator->second == 0) {
						string::pool.erase(iterator);
					}
					return;
				}
				++iterator;
			}
		}
		std::shared_ptr<const std::string> str;
};
std::vector<std::pair<std::shared_ptr<const std::string>, i64>> string::pool{};