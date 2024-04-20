#include <base/base.hpp>

int main(int argc, char **argv) {
	const vnd::vector<string> _args = vnd::createArgs(argc, argv);
	_println(string("Hello World!"));
	return 0;
}
