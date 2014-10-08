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
	updateRenderStates = (UpdateRenderStates)loadFunctionPointer("UpdateRenderStates");
	getPerspectiveProjection = (GetPerspectiveProjection)loadFunctionPointer("GetPerspectiveProjection");
	adjustCameraSpaceMatrix = (AdjustCameraSpaceMatrix)loadFunctionPointer("AdjustCameraSpaceMatrix");
	 
	

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
	getVAODataPointer = (GetVAODataPointer)loadFunctionPointer("GetVAODataPointer");
	returnVAODataPointer = (ReturnVAODataPointer)loadFunctionPointer("ReturnVAODataPointer");

	// IndexBufferObject
	createIBO = (CreateIBO)loadFunctionPointer("CreateIBO");
	updateIBOIndices = (UpdateIBOIndices)loadFunctionPointer("UpdateIBOIndices");
	bindIBO = (BindIBO)loadFunctionPointer("BindIBO");
	unbindIBO = (UnbindIBO)loadFunctionPointer("UnbindIBO");
	getIBODataPointer = (GetIBODataPointer)loadFunctionPointer("GetIBODataPointer");
	returnIBODataPointer = (ReturnIBODataPointer)loadFunctionPointer("ReturnIBODataPointer");

	// Drawing
	drawVAO = (DrawVAO)loadFunctionPointer("DrawVAO");
	drawIBO = (DrawIBO)loadFunctionPointer("DrawIBO");
	
	// RenderTarget
	createRenderTarget = (CreateRenderTarget)loadFunctionPointer("CreateRenderTarget");
	attachTextureToRenderTarget = (AttachTextureToRenderTarget)loadFunctionPointer("AttachTextureToRenderTarget");
	bindRenderTarget = (BindRenderTarget)loadFunctionPointer("BindRenderTarget");
	unbindRenderTarget = (UnbindRenderTarget)loadFunctionPointer("UnbindRenderTarget");

	// Texture
	createTexture2D = (CreateTexture2D)loadFunctionPointer("CreateTexture2D");
	createTexture2DArray = (CreateTexture2DArray)loadFunctionPointer("CreateTexture2DArray");
	createTextureCube = (CreateTextureCube)loadFunctionPointer("CreateTextureCube");
	createTexture3D = (CreateTexture3D)loadFunctionPointer("CreateTexture3D");
	bindTexture = (BindTexture)loadFunctionPointer("BindTexture");
	unbindTexture = (UnbindTexture)loadFunctionPointer("UnbindTexture");

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