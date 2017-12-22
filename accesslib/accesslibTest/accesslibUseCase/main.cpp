#include "ApiHandler.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
	ApiHandler *ah = new ApiHandler();

	ah->connect("127.0.0.1");
	
	
	system("pause");
	return 0;
}