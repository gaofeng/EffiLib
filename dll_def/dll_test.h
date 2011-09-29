// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DLL_TEST_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DLL_TEST_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DLL_TEST_EXPORTS
#define DLL_TEST_API __declspec(dllexport)
#else
#define DLL_TEST_API __declspec(dllimport)
#endif

// This class is exported from the dll_test.dll
class DLL_TEST_API Cdll_test {
public:
	Cdll_test(void);
	// TODO: add your methods here.
};

extern DLL_TEST_API int ndll_test;

DLL_TEST_API int fndll_test(void);
