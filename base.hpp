#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <memory>
#include <variant>
#include <unordered_map>
#include <any>
#include <iomanip>
#include <sstream>
#include <functional>
#include <cstdint>

#define FOR_LOOP(type, var, initial, final, step) \
for (type var = initial; (step > 0 ? var < final : var > final); var += step)

class string: public std::string {
	public:
		using std::string::string;
		string(const std::string &s): std::string(s) {}
		const char at(int64_t index) const {
			if(index < 0) {
				index += static_cast<int64_t>(size());
			}
			if(index < 0 || index >= size()) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(size())); }
			return std::string::at(index);
		}
};

namespace vnd {
	template <typename T, int64_t N>
	class array: public std::array<T, N> {
		public:
			array(): std::array<T, N>() {};
			array(const std::vector<T>& vec) {
				if (vec.size() == N) {
					std::copy(vec.begin(), vec.end(), this->begin());
				} else {
					throw std::invalid_argument("Vector size does not match array size");
				}
			}
			array(std::initializer_list<T> init) : std::array<T, N>() {
				if (init.size() != N) {
					throw std::runtime_error("Initializer list size does not match array size.");
				}
				std::copy(init.begin(), init.end(), this->begin());
			}
			T& at(int64_t index) {
				if(index < 0) {
					index += N;
				}
				if(index < 0 || index >= N) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(N)); }
				return std::array<T, N>::at(index);
			}
			const T& at(int64_t index) const {
				if(index < 0) {
					index += N;
				}
				if(index < 0 || index >= N) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(N)); }
				return std::array<T, N>::at(index);
			}
	};
	template <typename T>
	class vector: public std::vector<T> {
		public:
			vector(): std::vector<T>() {};
			vector(std::initializer_list<T> init) : std::vector<T>(init) {}
			template <typename InputIt>
			vector(InputIt first, InputIt last) : std::vector<T>(first, last) {}
			T& at(int64_t index) {
				if(index < 0) {
					index += static_cast<int64_t>(this->size());
				}
				if(index < 0 || index >= static_cast<int64_t>(this->size())) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(static_cast<int64_t>(this->size()))); }
				return std::vector<T>::at(index);
			}
			const T& at(int64_t index) const {
				if(index < 0) {
					index += static_cast<int64_t>(this->size());
				}
				if(index < 0 || index >= static_cast<int64_t>(this->size())) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(static_cast<int64_t>(this->size()))); }
				return std::vector<T>::at(index);
			}
			void add(const T elem) {
				std::vector<T>::emplace_back(elem);
			}
	};
	std::unordered_map<std::string, std::any> tmp;
}

int v_test() { return 0; }
int _testPar(int a, int b) { return a + b; }
size_t _testPar(string s) { return s.size(); }
class Object {
	public:
		int getC() { return c; };
		int getA() { return a; };
		void setA(int _a) { a = _a; };
		float getTest() { return test; }
		string getS() { return s; }
		void setS(string _s) { s = _s; }
		virtual float f(double b) { return std::pow(b, 2); }
		virtual std::string fs() { return std::string(); }
		virtual string toString() {
			std::stringstream ss;
			ss << std::hex << std::setw(sizeof(uintptr_t) * 2) << std::setfill('0') << reinterpret_cast<std::uintptr_t>(this);
			std::string hexString = ss.str();
			hexString.erase(0, hexString.find_first_not_of('0'));
			return hexString;
		}
	private:
		const int c = 2;
		const float test = 0;
		int a;
		string s;
};
class Derived: public Object {
	public:
		Derived(): obj(std::make_shared<Object>()) {}
		Derived(std::shared_ptr<Object> _obj, bool derivedProperty): _derivedProperty(derivedProperty), obj(std::make_shared<Object>()) {}
		bool get_derivedProperty() { return _derivedProperty; }
		void set_derivedProperty(bool derivedProperty) { _derivedProperty = derivedProperty; }
		std::shared_ptr<Object> getObj() { return obj; }
		void setObj(std::shared_ptr<Object> _obj) { obj = _obj; }
		bool get_derivedConst() { return _derivedConst; }
		virtual bool derivedFun(std::shared_ptr<Object> obj) { return obj->getS().empty(); }
		virtual std::shared_ptr<Object> object() { return std::make_shared<Object>(); }
	private:
		const bool _derivedConst = true;
		bool _derivedProperty;
		std::shared_ptr<Object> obj;
};
void _print(const std::string_view text, const vnd::vector<std::variant<int, float, char, bool, string, std::shared_ptr<Object>>>& args) {
	
	size_t par = 0;
	std::string_view::iterator iterator = text.begin();
	std::string result;
	std::function<std::string(std::variant<int, float, char, bool, string, std::shared_ptr<Object>>)> format = [](std::variant<int, float, char, bool, string, std::shared_ptr<Object>> param) -> std::string {
		if(std::holds_alternative<int>(param)) {
			return std::to_string(std::get<int>(param));
		} else if(std::holds_alternative<float>(param)) {
			return std::to_string(std::get<float>(param));
		} else if(std::holds_alternative<char>(param)) {
			return std::string(1, std::get<char>(param));
		} else if(std::holds_alternative<bool>(param)) {
			if(std::get<bool>(param) == true) {
				return "true";
			} else {
				return "false";
			}
		} else if(std::holds_alternative<string>(param)) {
			return std::get<string>(param);
		}
		return std::get<std::shared_ptr<Object>>(param)->toString();
	};

	while(iterator != text.end()) {
		if(*iterator == '{') {
			char next = *std::next(iterator);
			if(next == '}') {
				if(par < args.size()) {
					std::variant<int, float, char, bool, string, std::shared_ptr<Object>> param = args.at(par);
					result += format(args.at(par));
					par++;
				}
				iterator++;
			}
		} else {
			result += *iterator;
		}
		iterator++;
	}
	std::cout << result;
	
}
void _println(std::string_view text, const vnd::vector<std::variant<int, float, char, bool, string, std::shared_ptr<Object>>>& args) {
	
	_print(text, args);
	std::cout << std::endl;
	
}
string _readLine() {
	std::string input;
	std::getline (std::cin, input);
	return string(input);
}
std::tuple<int, float> _max(vnd::vector<float>) {
	
	return {0, 0};
	
}
std::shared_ptr<Object> _createObject() { return std::make_shared<Object>(); }
std::shared_ptr<Derived> _createDerived() { return std::make_shared<Derived>(); }