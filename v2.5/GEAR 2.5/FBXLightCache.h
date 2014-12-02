// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#pragma once
#include "Defines.h"

#include <fbxsdk.h>

// Property cache, value and animation curve.
struct PropertyChannel
{
    PropertyChannel() : mAnimCurve(NULL), mValue(0.0f) {}
    // Query the channel value at specific time.
    float Get(const FbxTime & pTime) const
    {
        if (mAnimCurve)
        {
            return mAnimCurve->Evaluate(pTime);
        }
        else
        {
            return mValue;
        }
    }

    FbxAnimCurve * mAnimCurve;
	float mValue;
};

// Cache for FBX lights
class FBXLightCache
{
public:
    FBXLightCache();
    ~FBXLightCache();

    // Set ambient light. Turn on light0 and set its attributes to default (white directional light in Z axis).
    // If the scene contains at least one light, the attributes of light0 will be overridden.
    static void IntializeEnvironment(const FbxColor & pAmbientLight);

    bool Initialize(const FbxLight * pLight, FbxAnimLayer * pAnimLayer);

    // Draw a geometry (sphere for point and directional light, cone for spot light).
    // And set light attributes.
    void SetLight(const FbxTime & pTime) const;

private:
    static int sLightCount;         // How many lights in this scene.

	unsigned int mLightIndex;
    FbxLight::EType mType;
    PropertyChannel mColorRed;
    PropertyChannel mColorGreen;
    PropertyChannel mColorBlue;
    PropertyChannel mConeAngle;
};