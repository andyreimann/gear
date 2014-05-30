// Library search
//#pragma comment(lib, "dgCore.lib")
//#pragma comment(lib, "dgPhysics.lib")
//#pragma comment(lib, "dgThreads.lib")
#ifdef _DEBUG
	#pragma comment(lib, "newton_d.lib")
	#pragma comment(lib, "dMath_d.lib")
#else
	#pragma comment(lib, "newton.lib")
	#pragma comment(lib, "dMath.lib")
#endif