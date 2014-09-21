#pragma once

#include <windowsx.h>
#include <d3d11.h>		// core
#include <d3dx11.h>		// extensions
#include <d3dx10.h>		// dx 11 is ext of dx 10 -> useful macros usable

#include <Cg/cgD3D11.h>

#include <glm/glm.hpp>

#include <glm/ext.hpp>

class Scene
{
	public:
		Scene(unsigned int width, unsigned int height, ID3D11Device* dev, ID3D11DeviceContext* devcon, CGcontext cgContext, CGprofile cgVertexShaderProfile, CGprofile cgFragmentShaderProfile);

		void render();

		~Scene(void);
	private:

		void loadMesh();
		void loadShader();
		void bindShader(CGprogram vs, CGprogram fs);

		ID3D11Device* mDevice;					// the pointer to our Direct3D device interface
		ID3D11DeviceContext* mDeviceContext;	// the pointer to our Direct3D device context
		CGcontext mCgContext;
		CGprofile mCgVertexShaderProfile; 
		CGprofile mCgFragmentShaderProfile;
		unsigned int mWidth;
		unsigned int mHeight;
		
		ID3D11Buffer* mPlaneVbo[2];
		ID3D11Buffer* mPlaneIbo;	

		ID3D11Buffer* mVbo[2];	
		ID3D11Buffer* mIbo;	
		ID3D11InputLayout *mVboLayout;


		CGprogram mVertexShaderId;
		CGprogram mFragmentShaderId;

		float mCubeRotation;
		glm::quat mRotation;
		glm::mat4 mCube1World;
		glm::mat4 mCube2World;
};

