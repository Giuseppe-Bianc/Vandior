#include <iostream>
#include <cmath>
#include <array>
#include <vector>
using string = std::string;
template <typename T, std::size_t N>
using array = std::array<T, N>;
template <typename T>
using vector = std::vector<T>;
v_test() {return 0;}
int _testPar(int a, int b) {return a + b;}
std::size_t _testPar(string s) {return s.size();}
class Object {
public:
	int a;
	std::string s;
	const int c = 2;
	double f(double b) { return std::pow(b, 2); }
	std::string fs() { return std::string(); }
};
Object _createObject() { return {}; }