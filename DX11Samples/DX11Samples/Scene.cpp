#include "Scene.h"

#include <string>
#include <fstream>
#include <sstream>

#include <glm/ext.hpp>

static void checkForCgError( const char * situation, CGcontext myCgContext, bool _exit = true )
{
	CGerror error;
	const char *string = cgGetLastErrorString(&error);

	if( error != CG_NO_ERROR ) 
	{
		if( error == CG_COMPILER_ERROR ) 
		{
			fprintf(stderr,
				"Situation: %s\n"
				"Error: %s\n\n"
				"Cg compiler output...\n%s",
				situation, string,
				cgGetLastListing(myCgContext));
		} 
		else 
		{
			fprintf(stderr,
				"Situation: %s\n"
				"Error: %s", situation, string);
		}

		if( _exit )
			exit(1);
	}
}

std::string
read(std::string const& file) 
{
	std::ifstream t(file);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

Scene::Scene(unsigned int width, unsigned int height, ID3D11Device* dev, ID3D11DeviceContext* devcon, CGcontext cgContext, CGprofile cgVertexShaderProfile, CGprofile cgFragmentShaderProfile) :
	mWidth(width),
	mHeight(height),
	mCubeRotation(0.f),
	mDevice(dev),
	mDeviceContext(devcon),
	mCgContext(cgContext),
	mCgVertexShaderProfile(cgVertexShaderProfile),
	mCgFragmentShaderProfile(cgFragmentShaderProfile)
{
	loadShader();
	loadMesh();
}


Scene::~Scene(void)
{
}

void
Scene::loadMesh() 
{
	glm::vec3 planeVertices[4];
	planeVertices[0] = glm::vec3(-0.5f, -0.5f, 0.5f);
	planeVertices[1] = glm::vec3(-0.5f,  0.5f, 0.5f);
	planeVertices[2] = glm::vec3( 0.5f,  0.5f, 0.5f);
	planeVertices[3] = glm::vec3( 0.5f, -0.5f, 0.5f);

	glm::vec4 planeColors[4];
	planeColors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	planeColors[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	planeColors[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	planeColors[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	
	DWORD planeIndices[] = {
		0, 1, 2,
		0, 2, 3,
	};
	
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof(bd) );

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(DWORD) * 2 * 3;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = planeIndices;
	mDevice->CreateBuffer(&bd, &iinitData, &mPlaneIbo);       // create the buffer

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = (sizeof(glm::vec3) * 4); // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	mDevice->CreateBuffer(&bd, NULL, &mPlaneVbo[0]);       // create the buffer

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = (sizeof(glm::vec4) * 4); // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	mDevice->CreateBuffer(&bd, NULL, &mPlaneVbo[1]);       // create the buffer

	D3D11_MAPPED_SUBRESOURCE ms;
	mDeviceContext->Map(mPlaneVbo[0], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, planeVertices, sizeof(glm::vec3) * 4);                // copy the vertices
	mDeviceContext->Unmap(mPlaneVbo[0], NULL); 

	mDeviceContext->Map(mPlaneVbo[1], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, planeColors, sizeof(glm::vec4) * 4);                // copy the colors
	mDeviceContext->Unmap(mPlaneVbo[1], NULL); 
	
	
	glm::vec3 vertices[8];
	vertices[0] = glm::vec3(-1.0f, -1.0f, -1.0f);
	vertices[1] = glm::vec3(-1.0f, +1.0f, -1.0f);
	vertices[2] = glm::vec3(+1.0f, +1.0f, -1.0f);
	vertices[3] = glm::vec3(+1.0f, -1.0f, -1.0f);
	vertices[4] = glm::vec3(-1.0f, -1.0f, +1.0f);
	vertices[5] = glm::vec3(-1.0f, +1.0f, +1.0f);
	vertices[6] = glm::vec3(+1.0f, +1.0f, +1.0f);
	vertices[7] = glm::vec3(+1.0f, -1.0f, +1.0f);

	glm::vec4 colors[8];
	colors[0] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	colors[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[2] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	colors[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[4] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	colors[5] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[6] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	colors[7] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	WORD indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3, 
		4, 3, 7
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = (sizeof(WORD) * 36); 
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = indices;
	mDevice->CreateBuffer(&bd, &InitData, &mIbo);       // create the buffer

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = (sizeof(glm::vec3) * 8); // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	mDevice->CreateBuffer(&bd, NULL, &mVbo[0]);       // create the buffer
	
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = (sizeof(glm::vec4) * 8); // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	mDevice->CreateBuffer(&bd, NULL, &mVbo[1]);       // create the buffer

	
	mDeviceContext->Map(mVbo[0], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, vertices, sizeof(vertices));                // copy the vertices
	mDeviceContext->Unmap(mVbo[0], NULL); 

	mDeviceContext->Map(mVbo[1], NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
	memcpy(ms.pData, colors, sizeof(colors));                // copy the colors
	mDeviceContext->Unmap(mVbo[1], NULL); 

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	1, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3D10Blob* vs = cgD3D11GetCompiledProgram(mVertexShaderId);

	mDevice->CreateInputLayout(ied, 2, vs->GetBufferPointer(), vs->GetBufferSize(), &mVboLayout);	

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = mWidth;
	viewport.Height = mHeight;
	// add depth values to viewport
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	mDeviceContext->RSSetViewports(1, &viewport);
}

void
Scene::loadShader() 
{
	std::string sourcePtr = read("vshader.cg");

	auto optimal = cgD3D11GetOptimalOptions(mCgVertexShaderProfile);

	mVertexShaderId = cgCreateProgram(mCgContext, CG_SOURCE, sourcePtr.c_str(), mCgVertexShaderProfile, "main", optimal);

	if(mVertexShaderId != nullptr)
	{
		optimal = cgD3D11GetOptimalOptions(mCgFragmentShaderProfile);

		HRESULT res = cgD3D11LoadProgram(mVertexShaderId, NULL);

		CGerror error;
		const char *errorString = cgGetLastErrorString(&error);
		sourcePtr = read("fshader.cg");
		mFragmentShaderId = cgCreateProgram(mCgContext, CG_SOURCE, sourcePtr.c_str(), mCgFragmentShaderProfile, "main", optimal);
		errorString = cgGetLastErrorString(&error);
		res = cgD3D11LoadProgram(mFragmentShaderId, NULL);
	}

	// here the uniform can be set 
	CGparameter location = cgGetNamedParameter(mVertexShaderId, "ambient");
	checkForCgError("could not get uniform color location", mCgContext, false);
	cgSetParameter4fv(location, glm::value_ptr(glm::vec4(1.0,1.0,1.0,1.0)));
	checkForCgError("could not set uniform color", mCgContext, false);

	bindShader(mVertexShaderId, mFragmentShaderId);

}

void
Scene::bindShader(CGprogram vs, CGprogram fs) 
{
	cgD3D11BindProgram(vs);
	cgD3D11BindProgram(fs);

}

void
Scene::render() 
{
	//Keep the cubes rotating
	mCubeRotation += .05f;
	if(mCubeRotation > 1.0f)
		mCubeRotation = 0.0f;

	////Reset cube1World
	//mCube1World = glm::mat4();

	////Define cube1's world space matrix
	//mRotation = glm::cross(mRotation, glm::angleAxis(mCubeRotation * 180.f / 3.14f, glm::vec3(0,1,0))); 
	//glm::mat4 translation = glm::translate( 0.0f, 0.0f, 4.0f );

	////Set cube1's world space using the transformations
	//mCube1World = translation * glm::toMat4(mRotation);

	////Reset cube2World
	//mCube1World = glm::mat4();

	////Define cube2's world space matrix
	//mRotation = glm::cross(-mRotation, glm::angleAxis(mCubeRotation * 180.f / 3.14f, glm::vec3(0,1,0))); 
	//glm::mat4 scale = glm::scale( 1.3f, 1.3f, 1.3f );

	////Set cube2's world space matrix
	//mCube2World = glm::toMat4(mRotation) * scale;

	//glm::vec4 camPos = glm::vec4( 0.0f, 3.0f, -8.0f, 0.0f );

	//glm::mat4 camProjection = glm::perspective(70.f, mWidth / (float)mHeight, 0.1f, 1000.0f);

	//glm::mat4 camView = glm::translate(glm::vec3(0,0,-3.5));

	//glm::mat4 WVP = camProjection * camView * mCube1World;
	//
	//CGparameter location = cgGetNamedParameter(mVertexShaderId, "matrices.WVP");
	//cgSetMatrixParameterfc(location, glm::value_ptr(WVP));

	// do 3D rendering on the back buffer here
	//UINT stride = sizeof(VERTEX);
	//UINT offset = 0;
	//mDeviceContext->Draw(4, 0);

	

	unsigned int strides[2] = { sizeof(glm::vec3), sizeof(glm::vec4) };
	unsigned int offsets[2] = { 0, 0 };
	//dev->IASetVertexBuffers(0, 2, &pVBuffer, strides, offsets );
	//mDeviceContext->IASetInputLayout(mVboLayout); // this needs to be done to really set it
	//mDeviceContext->IASetVertexBuffers(0, 2, mVbo, strides, offsets);
	//mDeviceContext->IASetIndexBuffer( mIbo, DXGI_FORMAT_R16_UINT, 0 );
	//mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//mDeviceContext->DrawIndexed(36, 0, 0);

	
	// GETTING A LOCATION CAN BE DONE WHEN A SHADER IS BOUND
	CGparameter location2 = cgGetNamedParameter(mVertexShaderId, "ambient");
	
	cgD3D11UnbindProgram(mVertexShaderId);
	cgD3D11UnbindProgram(mFragmentShaderId);

	// We can only set the uniform if the shader is NOT BOUND!
	// In OpenGL we have to check that!
	// FOUND OUT: a location can be retrieved and set to a value when a shader is not bound!

	// BUT SETTING IT's VALUE CAN ONLY BE DONE WHEN IT's UNBOUND!
	cgSetParameter4fv(location2, glm::value_ptr(glm::vec4(0.5,mCubeRotation,1.0,1.0)));
	checkForCgError("could not set uniform color", mCgContext, false);
	
	bindShader(mVertexShaderId, mFragmentShaderId);


	mDeviceContext->IASetInputLayout(mVboLayout); // this needs to be done to really set it
	mDeviceContext->IASetVertexBuffers(0, 2, mPlaneVbo, strides, offsets);
	mDeviceContext->IASetIndexBuffer( mPlaneIbo, DXGI_FORMAT_R32_UINT, 0 );
	mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	mDeviceContext->DrawIndexed(6, 0, 0);

}
