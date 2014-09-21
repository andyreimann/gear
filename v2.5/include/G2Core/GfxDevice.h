#pragma once
#include "Defines.h"
#include "VertexInputLayout.h"

#include <glm/glm.hpp>
#include <string>
#include <windows.h>

namespace G2 
{

	typedef glm::detail::tvec4<int> rect;
	
	// General
	typedef bool (*Init)(void* data);
	typedef void (*Shutdown)();
	typedef int (*GfxVersion)();
	typedef bool (*SupportsShadingLanguage)(std::string const& shadingLanguage);
	typedef void (*ClearColor)(glm::vec4 const& color);
	typedef void (*SetViewport)(G2::rect const& viewport);
	typedef void (*ClearBuffers)(G2Core::BufferFlags flags, G2Core::GfxResource* buffer);
	typedef void (*UpdateRenderStates)(G2Core::FaceCulling::Name cullFaceState, G2Core::PolygonDrawMode::Name polygonDrawMode, float polygonOffsetFactor, float polygonOffsetUnits, G2Core::BlendFactor::Name blendFuncSourceFactor, G2Core::BlendFactor::Name blendFuncDestinationFactor);

	// Shader
	typedef G2Core::GfxResource* (*CompileShader)(G2Core::VertexInputLayout const& vertexInputLayout, std::string const& shadingLanguage, std::string const& vertexCode, std::string const& geometryCode, std::string const& fragmentCode);
	typedef void (*BindShader)(G2Core::GfxResource const* shaderResource);
	typedef void (*SetShaderUniformMat4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat4 const& value);
	typedef void (*SetShaderUniformMat3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::mat3 const& value);
	typedef void (*SetShaderUniformVec4)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec4 const& value);
	typedef void (*SetShaderUniformVec3)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec3 const& value);
	typedef void (*SetShaderUniformVec2)(G2Core::GfxResource* shaderResource, std::string const& property, glm::vec2 const& value);
	typedef void (*SetShaderUniformFloat)(G2Core::GfxResource* shaderResource, std::string const& property, float value);
	typedef void (*SetShaderUniformInt)(G2Core::GfxResource* shaderResource, std::string const& property, int value);
	typedef void (*FreeGfxResource)(G2Core::GfxResource* shaderResource);

	// VAO
	typedef G2Core::GfxResource* (*CreateVAO)();
	typedef void (*UpdateVAOVertexBufferVec4)(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements);
	typedef void (*UpdateVAOVertexBufferVec3)(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements);
	typedef void (*UpdateVAOVertexBufferVec2)(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements);
	typedef void (*BindVAO)(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader);
	typedef void (*UnbindVAO)(G2Core::GfxResource* vao);
	typedef void* (*GetVAODataPointer)(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode);
	typedef void (*ReturnVAODataPointer)(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic);
	
	// IndexBufferObject
	typedef G2Core::GfxResource* (*CreateIBO)();
	typedef void (*UpdateIBOIndices)(G2Core::GfxResource* ibo, unsigned int const* data, int numElements);
	typedef void (*BindIBO)(G2Core::GfxResource* ibo);
	typedef void (*UnbindIBO)(G2Core::GfxResource* ibo);
	typedef unsigned int* (*GetIBODataPointer)(G2Core::GfxResource* ibo, G2Core::BufferAccessMode::Name mode);
	typedef void (*ReturnIBODataPointer)(G2Core::GfxResource* ibo);
	
	// Drawing
	typedef void (*DrawVAO)(G2Core::GfxResource* vao, G2Core::DrawMode::Name drawMode, int numVertices);
	typedef void (*DrawIBO)(G2Core::GfxResource* ibo, G2Core::DrawMode::Name drawMode, int numIndices);
	
	// RenderTarget
	typedef G2Core::GfxResource* (*CreateRenderTarget)(unsigned int width, unsigned int height, G2Core::DataFormat::Name format);
	typedef void (*AttachTextureToRenderTarget)(G2Core::GfxResource* renderTarget, G2Core::GfxResource* texture, G2Core::FrameBufferAttachmentPoint::Name attachment, G2Core::TextureFormat::Name texTarget, int mipmapLevel, int layer);
	typedef void (*BindRenderTarget)(G2Core::GfxResource* renderTarget);
	typedef void (*UnbindRenderTarget)(G2Core::GfxResource* renderTarget);	

	// Texture
	typedef G2Core::GfxResource* (*CreateTexture2D)(unsigned int width, unsigned int height, G2Core::DataFormat::Name format, G2Core::DataFormat::Name internalFormat, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT, unsigned char * data);
	typedef G2Core::GfxResource* (*CreateTexture2DArray)(unsigned int width, unsigned int height, unsigned int size, G2Core::DataFormat::Name format, G2Core::DataFormat::Name internalFormat, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT, unsigned char * data);
	typedef G2Core::GfxResource* (*CreateTextureCube)(unsigned int width, unsigned int height, G2Core::DataFormat::Name format, G2Core::DataFormat::Name internalFormat, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT);
	typedef G2Core::GfxResource* (*CreateTexture3D)(unsigned int width, unsigned int height, unsigned int size, G2Core::DataFormat::Name format, G2Core::DataFormat::Name internalFormat, G2Core::FilterMode::Name minFilter, G2Core::FilterMode::Name magFilter, G2Core::WrapMode::Name wrapS, G2Core::WrapMode::Name wrapT, G2Core::WrapMode::Name wrapR, unsigned char * data);
	typedef void (*BindTexture)(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot);
	typedef void (*UnbindTexture)(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot);
	

	/** This class declares the interface of a graphics device.
	 * @created:	2014/09/11
	 * @author Andy Reimann <a.reimann@moorlands-grove.de>
	 */
	class GfxDevice 
	{
		public:
			/// This constructs a new GfxDevice.
			COREDLL_API GfxDevice(std::string const& gfxDevicelib);

			~GfxDevice() {};

			// START GFX LIB FUNCTION POINTERS

			// General
			Init init;
			Shutdown shutdown;
			GfxVersion gfxVersion;
			SupportsShadingLanguage supportsShadingLanguage;
			ClearColor clearColor;
			SetViewport setViewport;
			ClearBuffers clearBuffers;
			UpdateRenderStates updateRenderStates;

			// Shader
			CompileShader compileShader;
			BindShader bindShader;
			SetShaderUniformMat4 setShaderUniformMat4;
			SetShaderUniformMat3 setShaderUniformMat3;
			SetShaderUniformVec4 setShaderUniformVec4;
			SetShaderUniformVec3 setShaderUniformVec3;
			SetShaderUniformVec2 setShaderUniformVec2;
			SetShaderUniformFloat setShaderUniformFloat;
			SetShaderUniformInt setShaderUniformInt;
			FreeGfxResource freeGfxResource;

			// VAO
			CreateVAO createVAO;
			UpdateVAOVertexBufferVec4 updateVAOVertexBufferVec4;
			UpdateVAOVertexBufferVec3 updateVAOVertexBufferVec3;
			UpdateVAOVertexBufferVec2 updateVAOVertexBufferVec2;
			BindVAO bindVAO;
			UnbindVAO unbindVAO;
			GetVAODataPointer getVAODataPointer;
			ReturnVAODataPointer returnVAODataPointer;

			// IndexBufferObject
			CreateIBO createIBO;
			UpdateIBOIndices updateIBOIndices;
			BindIBO bindIBO;
			UnbindIBO unbindIBO;
			GetIBODataPointer getIBODataPointer;
			ReturnIBODataPointer returnIBODataPointer;

			// Drawing
			DrawVAO drawVAO;
			DrawIBO drawIBO;

			// RenderTarget
			CreateRenderTarget createRenderTarget;
			AttachTextureToRenderTarget attachTextureToRenderTarget;
			BindRenderTarget bindRenderTarget;
			UnbindRenderTarget unbindRenderTarget;

			// Texture
			CreateTexture2D createTexture2D;
			CreateTexture2DArray createTexture2DArray;
			CreateTextureCube createTextureCube;
			CreateTexture3D createTexture3D;
			BindTexture bindTexture;
			UnbindTexture unbindTexture;

			// END GFX LIB FUNCTION POINTERS
		private:
			
			bool loadGfxLibrary();
			
			void loadFunctionPointers();
			
			FARPROC loadFunctionPointer(std::string const& name);

			std::string mGfxLibName;
			HMODULE		mGfxLibHandle;

	};
};

extern "C"
{
	COREDLL_API void G2_loadGfxDevice(std::string const& gfxDeviceLib);
	COREDLL_API void G2_gfxDeviceDestroy();
	COREDLL_API G2::GfxDevice* G2_gfxDevice();
};