#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

G2Core::GfxResource* CreateIBO()
{
	G2DX11::IndexBufferObjectResource* resource = new G2DX11::IndexBufferObjectResource(nullptr);
	return resource;
}

ID3D11Buffer* _createStagingBuffer(int bytes, unsigned int const* data)
{

	ID3D11Buffer* stagingIbo = nullptr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_STAGING;					// write access access by CPU and GPU
	bd.ByteWidth = bytes;						// allocate memory
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;		// allow CPU to read in buffer

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = data;
	HRESULT hr = gDevicePtr()->CreateBuffer(&bd, &iinitData, &stagingIbo);    // create the buffer
	return stagingIbo;
}

void UpdateIBOIndices(G2Core::GfxResource* ibo, unsigned int const* data, int numElements)
{
	G2DX11::IndexBufferObjectResource* iboPtr = static_cast<G2DX11::IndexBufferObjectResource*>(ibo);
	HRESULT hr;
	if(iboPtr->ibo == nullptr)
	{
		// create new one
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );

		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(unsigned int) * numElements;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = data;
		hr = gDevicePtr()->CreateBuffer(&bd, &iinitData, &iboPtr->ibo);    // create the buffer
		

		if (!SUCCEEDED(hr))
		{
			G2::logger << "Could not create IBO" << G2::endl;
		}
		// create staging buffer
		iboPtr->stagingIbo = _createStagingBuffer(sizeof(unsigned int) * numElements, data);
		return;
	}
	D3D11_MAPPED_SUBRESOURCE ms;
	hr = gDeviceContextPtr()->Map(iboPtr->ibo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	if (!SUCCEEDED(hr))
	{
		G2::logger << "Could not map IBO" << G2::endl;
	}
	memcpy(ms.pData, data, sizeof(unsigned int) * numElements);						// copy the vertices
	gDeviceContextPtr()->Unmap(iboPtr->ibo, NULL); 
}

void BindIBO(G2Core::GfxResource* ibo)
{
	G2DX11::IndexBufferObjectResource* iboPtr = static_cast<G2DX11::IndexBufferObjectResource*>(ibo);
	gDeviceContextPtr()->IASetIndexBuffer( iboPtr->ibo, DXGI_FORMAT_R32_UINT, 0);
}

void UnbindIBO(G2Core::GfxResource* ibo)
{

}

unsigned int* GetIBODataPointer(G2Core::GfxResource* ibo, G2Core::BufferAccessMode::Name mode)
{
	G2DX11::IndexBufferObjectResource* iboPtr = static_cast<G2DX11::IndexBufferObjectResource*>(ibo);
	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = gDeviceContextPtr()->Map(iboPtr->stagingIbo, NULL, D3D11_MAP_READ, NULL, &ms);

	if (!SUCCEEDED(hr))
	{
		G2::logger << "Could not map IBO" << G2::endl;
	}
	return (unsigned int*)ms.pData;
}

void ReturnIBODataPointer(G2Core::GfxResource* ibo)
{
	G2DX11::IndexBufferObjectResource* iboPtr = static_cast<G2DX11::IndexBufferObjectResource*>(ibo);
	gDeviceContextPtr()->Unmap(iboPtr->stagingIbo, NULL);
	gDeviceContextPtr()->CopyResource(iboPtr->ibo, iboPtr->stagingIbo);
}

void DrawIBO(G2Core::GfxResource* ibo, G2Core::DrawMode::Name drawMode, int numIndices)
{
	gDeviceContextPtr()->IASetPrimitiveTopology(toD3DDrawMode(drawMode));
	gDeviceContextPtr()->DrawIndexed( numIndices, 0, 0 );
}
