#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateVAO()
{
	return new G2DX11::VertexArrayObjectResource();
} 

void UpdateVAOVertexBufferVec4(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec4 const* data, int numElements)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	unsigned int bytes = (sizeof(float) * 4 * numElements);
	if(vaoPtr->vbos.find(semantic) == vaoPtr->vbos.end())
	{
		// create new one
		ID3D11Buffer* vbo;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
		bd.ByteWidth = bytes;							// allocate memory
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

		gDevicePtr()->CreateBuffer(&bd, NULL, &vbo);    // create the buffer

		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = gDeviceContextPtr()->Map(vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (!SUCCEEDED(hr))
		{
			std::cout << "Could not load vertex buffer data!\n";
			exit(-1);
		}
		memcpy(ms.pData, data, bd.ByteWidth);           // copy the vertices
		gDeviceContextPtr()->Unmap(vbo, NULL); 

		std::vector<G2DX11::InputLayoutVertexBufferFragment> vertexLayout;

		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = toD3DSemanticString(semantic);
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.InputSlot = (unsigned int)vaoPtr->vbos.size();
		desc.AlignedByteOffset = 0;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		vertexLayout.push_back(G2DX11::InputLayoutVertexBufferFragment(desc,0,sizeof(float) * 4));

		vaoPtr->vbos[semantic] = new G2DX11::VertexBufferObjectResource(vbo,vertexLayout);
		return;
	}
	G2DX11::VertexBufferObjectResource* vboPtr = vaoPtr->vbos[semantic];
	D3D11_MAPPED_SUBRESOURCE ms;
	gDeviceContextPtr()->Map(vboPtr->vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, bytes);						// copy the vertices
	gDeviceContextPtr()->Unmap(vboPtr->vbo, NULL); 

	return;
}

void UpdateVAOVertexBufferVec3(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec3 const* data, int numElements)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	unsigned int bytes = (sizeof(float) * 3 * numElements);
	if(vaoPtr->vbos.find(semantic) == vaoPtr->vbos.end())
	{
		// create new one
		ID3D11Buffer* vbo;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
		bd.ByteWidth = bytes;							// allocate memory
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

		HRESULT hr = gDevicePtr()->CreateBuffer(&bd, NULL, &vbo);    // create the buffer
		if (SUCCEEDED(hr))
		{
			std::cout << "Created vertex buffer!\n";
		}
		else
		{
			std::cout << "Could not create vertex buffer!\n";
			exit(-1);
		}

		D3D11_MAPPED_SUBRESOURCE ms;
		hr = gDeviceContextPtr()->Map(vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (!SUCCEEDED(hr))
		{
			std::cout << "Could not load vertex buffer data!\n";
			exit(-1);
		}
		memcpy(ms.pData, data, bd.ByteWidth);           // copy the vertices
		gDeviceContextPtr()->Unmap(vbo, NULL); 

		std::vector<G2DX11::InputLayoutVertexBufferFragment> vertexLayout;

		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = toD3DSemanticString(semantic);
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		desc.InputSlot = (unsigned int)vaoPtr->vbos.size();
		desc.AlignedByteOffset = 0;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		vertexLayout.push_back(G2DX11::InputLayoutVertexBufferFragment(desc,0,sizeof(float) * 3));

		vaoPtr->vbos[semantic] = new G2DX11::VertexBufferObjectResource(vbo,vertexLayout);
		return;
	}
	G2DX11::VertexBufferObjectResource* vboPtr = vaoPtr->vbos[semantic];
	D3D11_MAPPED_SUBRESOURCE ms;
	gDeviceContextPtr()->Map(vboPtr->vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, bytes);						// copy the vertices
	gDeviceContextPtr()->Unmap(vboPtr->vbo, NULL); 
	return;
}

void UpdateVAOVertexBufferVec2(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, glm::vec2 const* data, int numElements)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	unsigned int bytes = (sizeof(float) * 2 * numElements);
	if(vaoPtr->vbos.find(semantic) == vaoPtr->vbos.end())
	{
		// create new one
		ID3D11Buffer* vbo;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;					// write access access by CPU and GPU
		bd.ByteWidth = bytes;							// allocate memory
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		// use as a vertex buffer
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;		// allow CPU to write in buffer

		gDevicePtr()->CreateBuffer(&bd, NULL, &vbo);    // create the buffer

		D3D11_MAPPED_SUBRESOURCE ms;
		gDeviceContextPtr()->Map(vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, data, bd.ByteWidth);           // copy the vertices
		gDeviceContextPtr()->Unmap(vbo, NULL); 

		std::vector<G2DX11::InputLayoutVertexBufferFragment> vertexLayout;

		D3D11_INPUT_ELEMENT_DESC desc;
		desc.SemanticName = toD3DSemanticString(semantic);
		desc.SemanticIndex = 0;
		desc.Format = DXGI_FORMAT_R32G32_FLOAT;
		desc.InputSlot = (unsigned int)vaoPtr->vbos.size();
		desc.AlignedByteOffset = 0;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		vertexLayout.push_back(G2DX11::InputLayoutVertexBufferFragment(desc,0,sizeof(float) * 2));
		return;
	}
	G2DX11::VertexBufferObjectResource* vboPtr = vaoPtr->vbos[semantic];
	D3D11_MAPPED_SUBRESOURCE ms;
	gDeviceContextPtr()->Map(vboPtr->vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, data, bytes);						// copy the vertices
	gDeviceContextPtr()->Unmap(vboPtr->vbo, NULL); 
	return;
}

void BindVAO(G2Core::GfxResource* vao, G2Core::GfxResource* alreadyboundShader)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	std::vector<ID3D11Buffer*> finalBufferPtr;
	std::vector<unsigned int> finalStrides;
	std::vector<unsigned int> finalOffsets;
	if(alreadyboundShader != nullptr)
	{
		G2DX11::ShaderResource* shaderPtr = static_cast<G2DX11::ShaderResource*>(alreadyboundShader);
	
		size_t numShaderSemantics = shaderPtr->inputLayoutShaderFragment.semantics.size();

		finalBufferPtr.resize(numShaderSemantics);
		finalStrides.resize(numShaderSemantics);
		finalOffsets.resize(numShaderSemantics);

		std::vector<D3D11_INPUT_ELEMENT_DESC> finalVertexLayout(numShaderSemantics);

		for(size_t i = 0; i < numShaderSemantics; ++i)
		{
			G2Core::Semantics::Name semantic = shaderPtr->inputLayoutShaderFragment.semantics[i];
			G2DX11::VertexBufferObjectResource* vbo = vaoPtr->vbos[semantic];
			G2DX11::InputLayoutVertexBufferFragment const& vboLayout = vbo->vertexLayout[0];
			// Remember: VBOs inside of VAOs always have one InputLayoutVertexBufferFragment object in their vector!
			D3D11_INPUT_ELEMENT_DESC desc;
			desc.SemanticName = shaderPtr->inputLayoutShaderFragment.semanticNames[i];	// Shader driven
			desc.SemanticIndex = vboLayout.vertexLayout.SemanticIndex;					// Vertex Array driven
			desc.Format = vboLayout.vertexLayout.Format;								// Vertex Array driven
			desc.InputSlot = (unsigned int)i;															// Vertex Array driven
			desc.AlignedByteOffset = vboLayout.vertexLayout.AlignedByteOffset;			// Vertex Array driven
			desc.InputSlotClass = vboLayout.vertexLayout.InputSlotClass;				// Vertex Array driven
			desc.InstanceDataStepRate = vboLayout.vertexLayout.InstanceDataStepRate;	// Vertex Array driven

			finalVertexLayout[i] = desc;
			finalStrides[i] = vboLayout.offset;
			finalOffsets[i] = vboLayout.stride;
			finalBufferPtr[i] = vbo->vbo;
		}
		ID3D11InputLayout* renderLayout;
		HRESULT hr = gDevicePtr()->CreateInputLayout(&finalVertexLayout[0], finalVertexLayout.size(), shaderPtr->vertexShaderBlob->GetBufferPointer(), shaderPtr->vertexShaderBlob->GetBufferSize(), &renderLayout);
		gDeviceContextPtr()->IASetInputLayout(renderLayout); // this needs to be done to really set it
	}
	else
	{
		// bind without having a shader as a context
		// TODO Check if we can simply do nothing in this case!
		return;
		size_t numSemantics = vaoPtr->vbos.size();
		
		finalBufferPtr.resize(numSemantics);
		finalStrides.resize(numSemantics);
		finalOffsets.resize(numSemantics);

		int i = 0;
		for(auto it = vaoPtr->vbos.begin(); it != vaoPtr->vbos.end(); ++it)
		{
			finalBufferPtr[i] = it->second->vbo;
			finalStrides[i] = it->second->vertexLayout[0].stride;
			finalOffsets[i] = it->second->vertexLayout[0].offset;
			++i;
		}
	}

	gDeviceContextPtr()->IASetVertexBuffers(0, finalBufferPtr.size(), &finalBufferPtr[0], &finalStrides[0], &finalOffsets[0]);
	return;
}

void UnbindVAO(G2Core::GfxResource* vao)
{
	// nothing to do here so far
	return;
}

void DrawVAO(G2Core::GfxResource* vao, G2Core::DrawMode::Name drawMode, int numVertices)
{
	gDeviceContextPtr()->IASetPrimitiveTopology(toD3DDrawMode(drawMode));
	gDeviceContextPtr()->Draw(numVertices, 0);
	return;
}


	
void* GetVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic, G2Core::BufferAccessMode::Name mode)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	if(vaoPtr->vbos.find(semantic) != vaoPtr->vbos.end())
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		gDeviceContextPtr()->Map(vaoPtr->vbos[semantic]->vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
		return ms.pData;
	}
	return nullptr;
}

void ReturnVAODataPointer(G2Core::GfxResource* vao, G2Core::Semantics::Name semantic)
{
	G2DX11::VertexArrayObjectResource* vaoPtr = static_cast<G2DX11::VertexArrayObjectResource*>(vao);
	if(vaoPtr->vbos.find(semantic) != vaoPtr->vbos.end())
	{
		gDeviceContextPtr()->Unmap(vaoPtr->vbos[semantic]->vbo, NULL);
	}
	return;
}