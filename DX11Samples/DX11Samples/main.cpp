#include "DXWindow.h"
#include "Scene.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

#pragma comment(lib, "cg.lib")
#pragma comment(lib, "cgD3D11.lib")

// Only for end user using DX:
// macro redefinition problem: http://stackoverflow.com/questions/12663091/macro-definition-clash-between-directx-headers-and-winerror-h

int main(int argc, char *argv[])
{
	DXWindow window;
	Scene scene(window.getWidth(), window.getHeight(), window.getDevice(), window.getDeviceContext(), window.getCgContext(), window.getVSProfile(), window.getFSProfile());
	
	int cnt = 0;
	while(true)
	{
		window.startFrame();
		window.processEvents(cnt++);
		scene.render();
		window.swapBuffer();
	}
	return 0;
}