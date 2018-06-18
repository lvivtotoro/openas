#ifndef OASEX
	#if defined(_MSC_VER)
		#define OASEX __declspec(dllexport)
	#elif defined(__GNUC__)
		#define OASEX __attribute__((visibility("default")))
	#else
		#define OASEX
		#pragma warning Unknown DLL-export compiler-extension thingymagig.
	#endif
#endif

#ifndef NULL
    #define NULL 0
#endif
