#include "ApiHandler.h"
#include <iostream>
#include <vector>
using namespace std;

class Stub {
public:
	Stub() {
		x = 11;
	}
	int x;
	int y;
	int z;
};

int main() {
	Stub s;

	ApiHandler *ah = new ApiHandler();

	ah->connect("127.0.0.1");
	
	
	system("pause");
	return 0;
}