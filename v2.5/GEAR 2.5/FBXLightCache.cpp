// GEAR 2.5 - Game Engine Andy Reimann - Author: Andy Reimann <andy@moorlands-grove.de>
// (c) 2014 GEAR 2.5
#include "FBXLightCache.h"


const float ANGLE_TO_RADIAN = 3.1415926f / 180.f;

const GLfloat DEFAULT_LIGHT_POSITION[] = {0.0f, 0.0f, 0.0f, 1.0f};
const GLfloat DEFAULT_DIRECTION_LIGHT_POSITION[] = {0.0f, 0.0f, 1.0f, 0.0f};
const GLfloat DEFAULT_SPOT_LIGHT_DIRECTION[] = {0.0f, 0.0f, -1.0f};
const GLfloat DEFAULT_LIGHT_COLOR[] = {1.0f, 1.0f, 1.0f, 1.0f};
const GLfloat DEFAULT_LIGHT_SPOT_CUTOFF = 180.0f;

int FBXLightCache::sLightCount = 0;

FBXLightCache::FBXLightCache() : mType(FbxLight::ePoint)
{
    mLightIndex = GL_LIGHT0 + sLightCount++;
}

FBXLightCache::~FBXLightCache()
{
    glDisable(mLightIndex);
    --sLightCount;
}

// Bake light properties.
bool FBXLightCache::Initialize(const FbxLight * pLight, FbxAnimLayer * pAnimLayer)
{
    mType = pLight->LightType.Get();

    FbxPropertyT<FbxDouble3> lColorProperty = pLight->Color;
    FbxDouble3 lLightColor = lColorProperty.Get();
    mColorRed.mValue = static_cast<float>(lLightColor[0]);
    mColorGreen.mValue = static_cast<float>(lLightColor[1]);
    mColorBlue.mValue = static_cast<float>(lLightColor[2]);

	if (pAnimLayer)
	{
		mColorRed.mAnimCurve = lColorProperty.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_RED);
		mColorGreen.mAnimCurve = lColorProperty.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		mColorBlue.mAnimCurve = lColorProperty.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COLOR_BLUE);
	}

    if (mType == FbxLight::eSpot)
    {
        FbxPropertyT<FbxDouble> lConeAngleProperty = pLight->InnerAngle;
        mConeAngle.mValue = static_cast<GLfloat>(lConeAngleProperty.Get());
		if (pAnimLayer)
			mConeAngle.mAnimCurve = lConeAngleProperty.GetCurve(pAnimLayer);
    }

    return true;
}

void FBXLightCache::SetLight(const FbxTime & pTime) const
{
    const GLfloat lLightColor[4] = {mColorRed.Get(pTime), mColorGreen.Get(pTime), mColorBlue.Get(pTime), 1.0f};
    const GLfloat lConeAngle = mConeAngle.Get(pTime);

    glColor3fv(lLightColor);

    glPushAttrib(GL_ENABLE_BIT);
    glPushAttrib(GL_POLYGON_BIT);
    // Visible for double side.
    glDisable(GL_CULL_FACE);
    // Draw wire-frame geometry.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (mType == FbxLight::eSpot)
    {
        // Draw a cone for spot light.
        glPushMatrix();
        glScalef(1.0f, 1.0f, -1.0f);
        const double lRadians = ANGLE_TO_RADIAN * lConeAngle;
        const double lHeight = 15.0;
        const double lBase = lHeight * tan(lRadians / 2);
        GLUquadricObj * lQuadObj = gluNewQuadric();
        gluCylinder(lQuadObj, 0.0, lBase, lHeight, 18, 1);
        gluDeleteQuadric(lQuadObj);
        glPopMatrix();
    }
    else
    {
        // Draw a sphere for other types.
        GLUquadricObj * lQuadObj = gluNewQuadric();
        gluSphere(lQuadObj, 1.0, 10, 10);
        gluDeleteQuadric(lQuadObj);
    }
    glPopAttrib();
    glPopAttrib();

    // The transform have been set, so set in local coordinate.
    if (mType == FbxLight::eDirectional)
    {
        glLightfv(mLightIndex, GL_POSITION, DEFAULT_DIRECTION_LIGHT_POSITION);
    }
    else
    {
        glLightfv(mLightIndex, GL_POSITION, DEFAULT_LIGHT_POSITION);
    }

    glLightfv(mLightIndex, GL_DIFFUSE, lLightColor);
    glLightfv(mLightIndex, GL_SPECULAR, lLightColor);
    
    if (mType == FbxLight::eSpot && lConeAngle != 0.0)
    {
        glLightfv(mLightIndex, GL_SPOT_DIRECTION, DEFAULT_SPOT_LIGHT_DIRECTION);

        // If the cone angle is 0, equal to a point light.
        if (lConeAngle != 0.0f)
        {
            // OpenGL use cut off angle, which is half of the cone angle.
            glLightf(mLightIndex, GL_SPOT_CUTOFF, lConeAngle/2);
        }
    }
    glEnable(mLightIndex);
}

void FBXLightCache::IntializeEnvironment(const FbxColor & pAmbientLight)
{
    glLightfv(GL_LIGHT0, GL_POSITION, DEFAULT_DIRECTION_LIGHT_POSITION);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DEFAULT_LIGHT_COLOR);
    glLightfv(GL_LIGHT0, GL_SPECULAR, DEFAULT_LIGHT_COLOR);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, DEFAULT_LIGHT_SPOT_CUTOFF);
    glEnable(GL_LIGHT0);

    // Set ambient light.
    GLfloat lAmbientLight[] = {static_cast<GLfloat>(pAmbientLight[0]), static_cast<GLfloat>(pAmbientLight[1]),
        static_cast<GLfloat>(pAmbientLight[2]), 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lAmbientLight);
}