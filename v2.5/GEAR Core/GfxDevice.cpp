#include "GfxDevice.h"
#include <utility>
#include <iostream>

using namespace G2;

GfxDevice* gGfxDevice = nullptr;

GfxDevice::GfxDevice(std::string const& gfxDevicelib) 
	: mGfxLibName(gfxDevicelib)
{
	HMODULE libHandle = LoadLibrary(gfxDevicelib.c_str());
	if(loadGfxLibrary())
	{
		loadFunctionPointers();
	}
}

bool
GfxDevice::loadGfxLibrary() 
{
	mGfxLibHandle = LoadLibrary(mGfxLibName.c_str());
	return mGfxLibHandle != nullptr;
}

void
GfxDevice::loadFunctionPointers() 
{
	// General
	init = (Init)loadFunctionPointer("Init");
	shutdown = (Shutdown)loadFunctionPointer("Shutdown");
	gfxVersion = (GfxVersion)loadFunctionPointer("GfxVersion");
	supportsShadingLanguage = (SupportsShadingLanguage)loadFunctionPointer("SupportsShadingLanguage");
	clearColor = (ClearColor)loadFunctionPointer("ClearColor");
	setViewport = (SetViewport)loadFunctionPointer("SetViewport");
	clearBuffers = (ClearBuffers)loadFunctionPointer("ClearBuffers");

	// Shader
	compileShader = (CompileShader)loadFunctionPointer("CompileShader");
	bindShader = (BindShader)loadFunctionPointer("BindShader");
	setShaderUniformMat4 = (SetShaderUniformMat4)loadFunctionPointer("SetShaderUniformMat4");
	setShaderUniformMat3 = (SetShaderUniformMat3)loadFunctionPointer("SetShaderUniformMat3");
	setShaderUniformVec4 = (SetShaderUniformVec4)loadFunctionPointer("SetShaderUniformVec4");
	setShaderUniformVec3 = (SetShaderUniformVec3)loadFunctionPointer("SetShaderUniformVec3");
	setShaderUniformVec2 = (SetShaderUniformVec2)loadFunctionPointer("SetShaderUniformVec2");
	setShaderUniformFloat = (SetShaderUniformFloat)loadFunctionPointer("SetShaderUniformFloat");
	setShaderUniformInt = (SetShaderUniformInt)loadFunctionPointer("SetShaderUniformInt");
	freeGfxResource = (FreeGfxResource)loadFunctionPointer("FreeGfxResource");

	// VAO
	createVAO = (CreateVAO)loadFunctionPointer("CreateVAO");
	updateVAOVertexBufferVec4 = (UpdateVAOVertexBufferVec4)loadFunctionPointer("UpdateVAOVertexBufferVec4");
	updateVAOVertexBufferVec3 = (UpdateVAOVertexBufferVec3)loadFunctionPointer("UpdateVAOVertexBufferVec3");
	updateVAOVertexBufferVec2 = (UpdateVAOVertexBufferVec2)loadFunctionPointer("UpdateVAOVertexBufferVec2");
	bindVAO = (BindVAO)loadFunctionPointer("BindVAO");
	unbindVAO = (UnbindVAO)loadFunctionPointer("UnbindVAO");
	getVaoDataPointer = (GetVaoDataPointer)loadFunctionPointer("GetVaoDataPointer");
	returnVaoDataPointer = (ReturnVaoDataPointer)loadFunctionPointer("ReturnVaoDataPointer");

	// Drawing
	drawVAO = (DrawVAO)loadFunctionPointer("DrawVAO");
}

FARPROC
GfxDevice::loadFunctionPointer(std::string const& name) 
{
	FARPROC func = GetProcAddress(mGfxLibHandle, name.c_str());
	if(func == nullptr)
	{
		std::cout << "[WARNING] Could not get function pointer to function " << name << std::endl;
	}
	return func;
}

G2::GfxDevice* G2_gfxDevice()
{
	return gGfxDevice;
}

void G2_loadGfxDevice(std::string const& gfxDeviceLib) 
{
	gGfxDevice = new GfxDevice(gfxDeviceLib);
	assert(gGfxDevice != nullptr);
}

void G2_gfxDeviceDestroy()
{
	if(gGfxDevice != nullptr && gGfxDevice->shutdown != nullptr)
	{
		gGfxDevice->shutdown();
	}
	delete gGfxDevice;
	gGfxDevice = nullptr;
}