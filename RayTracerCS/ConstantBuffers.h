// ================================================================================ //
// Copyright (c) 2011 Arturo Garcia, Francisco Avila, Sergio Murguia and Leo Reyes	//
//																					//
// Permission is hereby granted, free of charge, to any person obtaining a copy of	//
// this software and associated documentation files (the "Software"), to deal in	//
// the Software without restriction, including without limitation the rights to		//
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies	//
// of the Software, and to permit persons to whom the Software is furnished to do	//
// so, subject to the following conditions:											//
//																					//
// The above copyright notice and this permission notice shall be included in all	//
// copies or substantial portions of the Software.									//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR		//
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,			//
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE		//
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER			//
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,	//
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE	//
// SOFTWARE.																		//
// ================================================================================ //

#include "Camera.h"
#include "Light.h"
#include "Eye.h"
#include "ReductionData.h"

// Constant buffer structures for the GPU.
// * They must implement an Update function.
// * Its size must be a multiple of 32 bytes or the app will crash.

// Camera
struct cbCamera
{
	//Multiple of 32 bytes
	Matrix4	mWorldMatrix;				// 64 bytes - World matrix

	void Update( Camera* pCamera )
	{
		mWorldMatrix = pCamera->GetInverseMatrix();
	}
};

// User input-output
struct cbInputOutput
{
	//Multiple of 32 bytes
	int		bIsShadowOn;				// 4 bytes
	int		bIsPhongShadingOn;			// 4 bytes
	int		bIsNormalMappingOn;			// 4 bytes
	int		bIsGlossMappingOn;			// 4 bytes
	int		iAccelerationStructure;		// 4 bytes
	int		iEnvMapping;				// 4 bytes
	UINT	iNumBounces;				// 4 bytes
	int		viPadding;					// 4 bytes

	cbInputOutput()
	{
		bIsShadowOn	= 1;
		bIsPhongShadingOn = 0;
		bIsNormalMappingOn = 1;
		bIsGlossMappingOn = 1;
		iEnvMapping = -1;
		iNumBounces = 0;
	}
	
	void Update( Input* pInput )
	{
		bIsShadowOn = static_cast<int>(pInput->IsShadow());
		bIsPhongShadingOn = static_cast<int>(pInput->IsFlatShading());
		iEnvMapping = static_cast<int>(pInput->GetEnvMappingFlag());
		bIsNormalMappingOn = static_cast<int>(pInput->IsNormalMappingOn());
		bIsGlossMappingOn = static_cast<int>(pInput->IsGlossMappingOn());
		iAccelerationStructure = static_cast<int>(pInput->GetAccelerationStructureFlag());
		iNumBounces = static_cast<int>(pInput->GetNumBounces());
	}
};

// Light (It just support one point light, extending this should be easy)
struct cbLight
{
	Vector3 g_vfLightPosition;
	float g_Brightness;

	void Update( Light* pLight )
	{
		g_vfLightPosition = pLight->GetPosition();
		g_Brightness = pLight->GetBrightness();
	}
};

// Global illumination variables
struct cbGlobalIllumination
{
	float	fRandX,fRandY;				// 8 bytes - Rand Offset
	float	fFactorAnt;					// 4 bytes
	float	fFactorAct;					// 4 bytes
	UINT	uiNumMuestras;				// 4 bytes
	int		vfPadding[3];				// 12 bytes

	cbGlobalIllumination()
	{
		uiNumMuestras = 0;
		fRandX = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
		fRandY = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
		fFactorAnt = 0.f;
		fFactorAct = 0.f;
	}

	void Reset()
	{
		uiNumMuestras = 0;
	}

	void Update( void* pObj )
	{
		fRandX = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
		fRandY = static_cast<float>(rand())/static_cast<float>(RAND_MAX);
		fFactorAnt = static_cast<float>(uiNumMuestras)/static_cast<float>(uiNumMuestras+1);
		fFactorAct = 1.0f/static_cast<float>(uiNumMuestras+1);
		++uiNumMuestras;
	}
};


struct cbEyeData
{
	float	g_Pupil2Retina;
	float	g_PupilRadius;
	float	g_FocalDistance;
	float	g_FocalLengthPrimary;
	float	g_AstigmatismAxisAngle;
	float	g_FocalLengthSecondary;
	float	g_AstigmatismAngle_Secondary;
	int		g_bIsAstigmatismOn;

	//cbEyeData()
	//{
	//	g_Retina2Len = 0.024f;
	//	g_LenRadius = 0.005f;
	//	g_FocalDistance = 10.0f;
	//	g_bIsAstigmatismOn = 0.0f;
	//	g_FocalDistanceInEye_Primary = g_Retina2Len;
	//	g_AstigmatismAngle_Primary = 0.0f;
	//	g_FocalDistanceInEye_Secondary = g_FocalDistanceInEye_Primary;
	//	g_AstigmatismAngle_Secondary = 90.0f;
	//}

	void Update(Eye* pEye)
	{
		g_Pupil2Retina = pEye->GetLengthPupil2Retina();
		g_PupilRadius = pEye->GetPupilRadius();
		g_FocalDistance = pEye->GetFocalDistance();
		//g_bIsAstigmatismOn = cbData->g_bIsAstigmatismOn;
		g_AstigmatismAxisAngle = pEye->GetAstigmatismAxisAngle();
		g_FocalLengthPrimary = pEye->GetLengthFocalLengthPrimary();
		g_FocalLengthSecondary = pEye->GetLengthFocalLengthSecondary();
		//g_AstigmatismAngle_Secondary = cbData->g_AstigmatismAngle_Secondary;
	}
};

struct cbSampleIndex
{
	int		g_SampleIndexX;
	int		g_SampleIndexY;
	float	g_SampleGaussianSum;
	int 	g_Padding;

	cbSampleIndex(int sampleX, int sampleY, float sampleGaussianSum)
	{
		g_SampleIndexX = sampleX;
		g_SampleIndexY = sampleY;
		g_SampleGaussianSum = sampleGaussianSum;
	}

	void Update(cbSampleIndex* pCbSampleIndex)
	{
		g_SampleIndexX = pCbSampleIndex->g_SampleIndexX;
		g_SampleIndexY = pCbSampleIndex->g_SampleIndexY;
		g_SampleGaussianSum = pCbSampleIndex->g_SampleGaussianSum;
	}
};

struct cbReduction
{
	unsigned int g_RDLength;
	unsigned int g_RDReadUAVIdx;//0:g_uAccumulation, 1:g_uResultBuffer
	float g_ScreenGaussianSum;
	float g_ScreenGaussianSigma;

	cbReduction(unsigned int length, unsigned int readUAVIdx, float screenGaussianSum, float screenGaussianSigma)
	{
		g_RDLength = length;
		g_RDReadUAVIdx = readUAVIdx;
		g_ScreenGaussianSum = screenGaussianSum;
		g_ScreenGaussianSigma = screenGaussianSigma;
	}

	void Update(ReductionData* pReductionData)
	{
		g_RDLength = pReductionData->GetLength();
		g_RDReadUAVIdx = pReductionData->GetReadUAVIdx();
		g_ScreenGaussianSum = pReductionData->GetScreenGaussianSum();
		g_ScreenGaussianSigma = pReductionData->GetScreenGaussianSigma();
	}
};