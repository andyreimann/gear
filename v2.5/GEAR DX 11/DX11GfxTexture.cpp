#include "DX11GfxApi.h"
#include "DX11GfxData.h"
#include "DX11GfxMappings.h"

#include <G2/Logger.h>

D3D11_SAMPLER_DESC _samplerState(
	D3D11_TEXTURE_ADDRESS_MODE u, 
	D3D11_TEXTURE_ADDRESS_MODE v, 
	D3D11_TEXTURE_ADDRESS_MODE w,
	D3D11_FILTER filter,
	unsigned int maxAnisotropy)
{
	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.AddressU = u;
	sd.AddressV = v;
	sd.AddressW = w;
	sd.MipLODBias = 0.0f;
	sd.MaxAnisotropy = 16;
	sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = D3D11_FLOAT32_MAX;

	return sd;
}

static D3D11_SAMPLER_DESC samplerStatePointDesc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_MIN_MAG_MIP_POINT, 1);
}

D3D11_SAMPLER_DESC _samplerStateLinearDesc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_MIN_MAG_MIP_LINEAR, 1);
}
D3D11_SAMPLER_DESC _samplerStateAnisotropic2Desc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_ANISOTROPIC, 2);
}
D3D11_SAMPLER_DESC _samplerStateAnisotropic4Desc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_ANISOTROPIC, 4);
}
D3D11_SAMPLER_DESC _samplerStateAnisotropic8Desc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_ANISOTROPIC, 8);
}

D3D11_SAMPLER_DESC _samplerStateAnisotropic16Desc(D3D11_TEXTURE_ADDRESS_MODE u, D3D11_TEXTURE_ADDRESS_MODE v, D3D11_TEXTURE_ADDRESS_MODE w)
{
	return _samplerState(u, v, w, D3D11_FILTER_ANISOTROPIC, 16);
}

G2Core::GfxResource* CreateTexture2D(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	unsigned char * data)
{
	DXGI_FORMAT dxFormat = toD3DFormat(internalFormat);
	if(dxFormat == DXGI_FORMAT_UNKNOWN)
	{
		std::cout << "Not supported format for 2D Texture!\n";
		return error();
	}
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = dxFormat;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	HRESULT hr = gDevicePtr()->CreateTexture2D(&desc, &initData, &tex);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create 2D Texture!\n";
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.Format = dxFormat;
	shaderDesc.Texture2D.MipLevels = 1;
	
	ID3D11ShaderResourceView* resView;
	hr = gDevicePtr()->CreateShaderResourceView(tex, &shaderDesc, &resView);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create shader resource view for Texture!\n";
		exit(-1);
	}

	ID3D11SamplerState* samplerState = nullptr;
	hr = gDevicePtr()->CreateSamplerState(
		&_samplerStateLinearDesc(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP), 
		&samplerState
	);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create sampler state for Texture!\n";
		exit(-1);
	}

	return new G2DX11::Texture2DResource(tex, resView, samplerState);
}

G2Core::GfxResource* CreateTexture2DArray(
	unsigned int width, 
	unsigned int height, 
	unsigned int size, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	unsigned char * data)
{
	DXGI_FORMAT dxFormat = toD3DFormat(internalFormat);
	if (dxFormat == DXGI_FORMAT_UNKNOWN)
	{
		std::cout << "Not supported format for 2D Texture Array!\n";
		return error();
	}
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = size;
	desc.Format = dxFormat;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture2D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	HRESULT hr = gDevicePtr()->CreateTexture2D(&desc, &initData, &tex);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create 2D Texture Array!\n";
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.Format = dxFormat;
	shaderDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* resView;
	hr = gDevicePtr()->CreateShaderResourceView(tex, &shaderDesc, &resView);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create shader resource view for Texture!\n";
		exit(-1);
	}

	ID3D11SamplerState* samplerState = nullptr;
	hr = gDevicePtr()->CreateSamplerState(
		&_samplerStateLinearDesc(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP),
		&samplerState
	);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create sampler state for Texture!\n";
		exit(-1);
	}
	return new G2DX11::Texture2DArrayResource(tex, resView, samplerState);
}

G2Core::GfxResource* CreateTextureCube(
	unsigned int width, 
	unsigned int height, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT)
{
	DXGI_FORMAT dxFormat = toD3DFormat(internalFormat);
	if (dxFormat == DXGI_FORMAT_UNKNOWN)
	{
		std::cout << "Not supported format for Cubemap Texture!\n";
		return error();
	}
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.Format = dxFormat;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; // mark as cubemap texture

	ID3D11Texture2D* tex = nullptr;
	HRESULT hr = gDevicePtr()->CreateTexture2D(&desc, nullptr, &tex);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create Cubemap Texture!\n";
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.Format = dxFormat;
	shaderDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* resView;
	hr = gDevicePtr()->CreateShaderResourceView(tex, &shaderDesc, &resView);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create shader resource view for Texture!\n";
		exit(-1);
	}

	ID3D11SamplerState* samplerState = nullptr;
	hr = gDevicePtr()->CreateSamplerState(
		&_samplerStateLinearDesc(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP),
		&samplerState
	);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create sampler state for Texture!\n";
		exit(-1);
	}

	return new G2DX11::TextureCubeResource(tex, resView, samplerState);
}

G2Core::GfxResource* CreateTexture3D(
	unsigned int width, 
	unsigned int height, 
	unsigned int depth, 
	G2Core::DataFormat::Name format, 
	G2Core::DataFormat::Name internalFormat, 
	G2Core::FilterMode::Name minFilter, 
	G2Core::FilterMode::Name magFilter, 
	G2Core::WrapMode::Name wrapS, 
	G2Core::WrapMode::Name wrapT,
	G2Core::WrapMode::Name wrapR,
	unsigned char * data)
{
	DXGI_FORMAT dxFormat = toD3DFormat(internalFormat);
	if (dxFormat == DXGI_FORMAT_UNKNOWN)
	{
		std::cout << "Not supported format for 3D Texture!\n";
		return error();
	}
	D3D11_TEXTURE3D_DESC desc;
	desc.Width = width;
	desc.Height = height;
	desc.Depth = depth;
	desc.MipLevels = 0;
	desc.Format = dxFormat;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	ID3D11Texture3D* tex = nullptr;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	HRESULT hr = gDevicePtr()->CreateTexture3D(&desc, &initData, &tex);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create 3D Texture Array!\n";
		exit(-1);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderDesc;
	shaderDesc.Format = dxFormat;
	shaderDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* resView;
	hr = gDevicePtr()->CreateShaderResourceView(tex, &shaderDesc, &resView);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create shader resource view for Texture!\n";
		exit(-1);
	}

	ID3D11SamplerState* samplerState = nullptr;
	hr = gDevicePtr()->CreateSamplerState(
		&_samplerStateLinearDesc(D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP),
		&samplerState
	);

	if (!SUCCEEDED(hr))
	{
		std::cout << "Could not create sampler state for Texture!\n";
		exit(-1);
	}
	return new G2DX11::Texture3DResource(tex, resView, samplerState);
}

void BindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2DX11::TextureResource* texPtr = static_cast<G2DX11::TextureResource*>(texResource);

	gDeviceContextPtr()->PSSetShaderResources(toD3DTexSlot(texSlot), 1, &texPtr->shaderResourceView);
}

void UnbindTexture(G2Core::GfxResource* texResource, G2Core::TexSlot::Name texSlot)
{
	G2DX11::TextureResource* texPtr = static_cast<G2DX11::TextureResource*>(texResource);
}
	