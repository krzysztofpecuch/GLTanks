#include "ApiHandler.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
	ApiHandler *ah = new ApiHandler();

	ah->connect("10.0.80.173");
	
	
	system("pause");
	return 0;
}