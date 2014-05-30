//#ifdef PHYSICSDLL_EXPORTS
//#define PHYSICSDLL_API __declspec(dllexport) 
//#else
//#define PHYSICSDLL_API __declspec(dllimport) 
//#endif

// Library search
#pragma comment(lib, "BulletCollision.lib")
#pragma comment(lib, "BulletDynamics.lib")
#pragma comment(lib, "BulletSoftBody.lib")