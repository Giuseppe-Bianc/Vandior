#include <iostream>
#include <cmath>
#include <array>
#include <vector>
#include <cstdint>

class string: public std::string_view {
	public:
		using std::string_view::string_view;
		const char at(int64_t index) const {
			if(index < 0) {
				index += static_cast<int64_t>(size());
			}
			if(index < 0 || index >= size()) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(size())); }
			return std::string_view::at(index);
		}
};

namespace vnd {
	template <typename T, int64_t N>
	class array: public std::array<T, N> {
		public:
			array(): std::array<T, N>() {};
			array(std::initializer_list<T> init) : std::array<T, N>() {
				if (init.size() != N) {
					throw std::runtime_error("Initializer list size does not match array size.");
				}
				std::copy(init.begin(), init.end(), this->begin());
			}
			T at(int64_t index) const {
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
			T at(int64_t index) const {
				if(index < 0) {
					index += static_cast<int64_t>(this->size());
				}
				if(index < 0 || index >= static_cast<int64_t>(this->size())) { throw std::runtime_error("Index " + std::to_string(index) +  " out of bounds for size " + std::to_string(static_cast<int64_t>(this->size()))); }
				return std::vector<T>::at(index);
			}
	};
}

int v_test() { return 0; }
int _testPar(int a, int b) { return a + b; }
size_t _testPar(string s) { return s.size(); }
class Object {
public:
	int a;
	std::string s;
	const int c = 2;
	double f(double b) { return std::pow(b, 2); }
	std::string fs() { return std::string(); }
};
Object _createObject() { return {}; }