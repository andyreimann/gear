// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <fbxsdk.h>

// Cache for FBX material
class FBXMaterialCache
{
public:
	FBXMaterialCache();
	~FBXMaterialCache();

	bool Initialize(const FbxSurfaceMaterial * pMaterial);
    
	// Set material colors and binding diffuse texture if exists.
	void SetCurrentMaterial() const;

	bool HasTexture() const { return mDiffuse.mTextureName != 0; }

	// Set default green color.
	static void SetDefaultMaterial();

private:
	struct ColorChannel
	{
		ColorChannel() : mTextureName(0)
		{
			mColor[0] = 0.0f;
			mColor[1] = 0.0f;
			mColor[2] = 0.0f;
			mColor[3] = 1.0f;
		}

		unsigned int mTextureName;
		float mColor[4];
	};
	ColorChannel mEmissive;
	ColorChannel mAmbient;
	ColorChannel mDiffuse;
	ColorChannel mSpecular;
	float mShinness;
};

