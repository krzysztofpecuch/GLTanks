#ifndef _TEST_H
#define _TEST_H

#ifdef TEST_EXPORTS  
#define TEST_API __declspec(dllexport)   
#else  
#define TEST_API __declspec(dllimport)   
#endif  


	// This class is exported from the MathLibrary.dll  
TEST_API void getTest();


#endif // !_TEST_H
