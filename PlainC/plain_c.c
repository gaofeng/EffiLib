#include <windows.h>
#include <objbase.h>
#include <INITGUID.H>

typedef HRESULT STDMETHODCALLTYPE QueryInterfacePtr(IExample *, REFIID, void **); 
typedef ULONG STDMETHODCALLTYPE AddRefPtr(IExample *); 
typedef ULONG STDMETHODCALLTYPE ReleasePtr(IExample *);

typedef HRESULT STDMETHODCALLTYPE SetStringPtr(IExample *, char *); 
typedef HRESULT STDMETHODCALLTYPE GetStringPtr(IExample *, char *, long);

HRESULT STDMETHODCALLTYPE SetString(IExample *this, char * str) 
{
	return(0); 
}

HRESULT STDMETHODCALLTYPE GetString(IExample *this, char *buffer, long value) 
{
	return(0);
}

typedef struct {
   // First 3 members must be called QueryInterface, AddRef, and Release
   QueryInterfacePtr  *QueryInterface;    
   AddRefPtr          *AddRef;    
   ReleasePtr         *Release;    
   SetStringPtr       *SetString;    
   GetStringPtr       *GetString; } 
IExampleVtbl;

int main(int argc, _TCHAR* argv[])
{
	return 0;
}

