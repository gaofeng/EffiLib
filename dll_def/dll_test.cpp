// dll_test.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "dll_test.h"


// This is an example of an exported variable
DLL_TEST_API int ndll_test=0;

// This is an example of an exported function.
DLL_TEST_API int fndll_test(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see dll_test.h for the class definition
Cdll_test::Cdll_test()
{
	return;
}
