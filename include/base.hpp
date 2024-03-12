#pragma once
#include "headers.hpp"
#include "primitives.hpp"

#define FOR_LOOP(type, var, initial, final, step) \
for (type var = initial; (step > 0 ? var < final : var > final); var += step)

#define printTypes i8, i64, u8, u16, u32, u64, f64, char, bool, string, std::shared_ptr<Object>

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
			const T at(int64_t index) const {
				if(index < 0) {
					index += static_cast<int64_t>(this->size());
				}
				if(index < 0 || index >= static_cast<int64_t>(this->size())) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(static_cast<int64_t>(this->size()))); }
				return std::vector<T>::at(index);
			}
			void addVector(const vnd::vector<T> elems) {
				for(const T &i: elems) {
					std::vector<T>::emplace_back(i);
				}
			}
			void addAll(const vnd::vector<T> elems) {
				for(const T &i: elems) {
					std::vector<T>::emplace_back(i);
				}
			}
			void add(const T elem) {
				std::vector<T>::emplace_back(elem);
			}
	};
	std::unordered_map<std::string, std::any> tmp;
	vnd::vector<string> createArgs(int argc, char **argv) {
		vnd::vector<string> args;
		FOR_LOOP(size_t, i, 0, argc, 1) {
			args.add(std::string(argv[i]));
		}
		return args;
	}
	template<typename T1, typename T2>
	std::common_type_t<T1, T2> mod(T1 a, T2 b) {
		using result_type = std::common_type_t<T1, T2>;
		if constexpr (std::is_same_v<result_type, int>) {
			return a % b;
		}
		return std::fmod(static_cast<result_type>(a), static_cast<result_type>(b));	
	}
	template<typename T1, typename T2>
	std::common_type_t<T1, T2> pow(T1 a, T2 b)  {
		return static_cast<std::common_type_t<T1, T2>>(std::pow(a, b));
	}
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
		int a{};
		string s{};
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
		bool _derivedProperty{};
		std::shared_ptr<Object> obj{};
};
void _print(const string text, const vnd::vector<std::variant<printTypes>>& args) {
	
	size_t par = 0;
	auto iterator = text.begin();
	std::string result;
	std::function<std::string(std::variant<printTypes>)> format = [](auto param) -> std::string {
		if(std::holds_alternative<i8>(param)) {
			return std::to_string(std::get<i8>(param));
		}
		if(std::holds_alternative<i64>(param)) {
			return std::to_string(std::get<i64>(param));
		}
		if(std::holds_alternative<u8>(param)) {
			return std::to_string(std::get<u8>(param));
		}
		if(std::holds_alternative<u16>(param)) {
			return std::to_string(std::get<u16>(param));
		}
		if(std::holds_alternative<u32>(param)) {
			return std::to_string(std::get<u32>(param));
		}
		if(std::holds_alternative<u64>(param)) {
			return std::to_string(std::get<u64>(param));
		}
		if(std::holds_alternative<f64>(param)) {
			std::string value = std::to_string(std::get<f64>(param));
			while(value.ends_with('0')) {
				value.pop_back();
			}
			if(value.ends_with('.')) {
				value.pop_back();
			}
			return value;
		}
		if(std::holds_alternative<char>(param)) {
			return std::string(1, std::get<char>( param));
		}
		if(std::holds_alternative<bool>(param)) {
			if(std::get<bool>(param) == true) {
				return "true";
			} else {
				return "false";
			}
		}
		if(std::holds_alternative<string>(param)) {
			return std::get<string>(param);
		}
		return std::get<std::shared_ptr<Object>>(param)->toString();
	};

	while(iterator != text.end()) {
		if(*iterator == '{') {
			char next = *std::next(iterator);
			if(next == '}') {
				if(par < args.size()) {
					auto param = args.at(par);
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
void _println(const string text, const vnd::vector<std::variant<printTypes>>& args) {
	
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
vnd::vector<int> _arrayTest() {
	return {1, 2, 3};
}
std::shared_ptr<Object> _createObject() { return std::make_shared<Object>(); }
std::shared_ptr<Derived> _createDerived() { return std::make_shared<Derived>(); }