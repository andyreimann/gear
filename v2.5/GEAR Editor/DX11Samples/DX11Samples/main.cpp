#include "DXWindow.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// Only for end user using DX:
// macro redefinition problem: http://stackoverflow.com/questions/12663091/macro-definition-clash-between-directx-headers-and-winerror-h

int main(int argc, char *argv[])
{
	DXWindow window;

	int cnt = 0;
	while(true)
	{
		window.processEvents(cnt++);
		window.swapBuffer();
	}
	return 0;
}