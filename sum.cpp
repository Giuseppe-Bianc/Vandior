#include <base/base.hpp>

int main(int argc, char **argv) {
	const vnd::vector<string> _args = vnd::createArgs(argc, argv);
	i32 _num1{}, _num2{};
	_print(string("Insert the first number:\t"));
	_num1 = _readLine().toI32();
	_print(string("Insert the second number:\t"));
	_num2 = _readLine().toI32();
	const i32 _sum = _num1 + _num2;
	_println(string("Sum:\t{}"), _sum);
	return 0;
}
