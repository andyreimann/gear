#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <xnamath.h>

#include "DXWindow.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgD3D10.lib")
#pragma comment(lib, "cgD3D11.lib")

// Only for end user using DX:
// macro redefinition problem: http://stackoverflow.com/questions/12663091/macro-definition-clash-between-directx-headers-and-winerror-h

DXWindow window;


int main(int argc, char *argv[])
{
	
	while(TRUE)
	{
		window.startFrame();
		window.endFrame();
	}
	return 0;
}