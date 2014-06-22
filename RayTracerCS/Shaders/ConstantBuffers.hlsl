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

// ------------------------------------------
// Constant Buffers	(Multiple of 32 bytes)
// ------------------------------------------
cbuffer cbCamera : register( b0 )	// 64 bytes
{
	float4x4 g_mfWorld;		// 64 bytes
};

cbuffer cbEyeData : register( b1 )
{
	float	g_Pupil2Retina;
	float	g_PupilRadius;
	float	g_FocalDistance;
	float	g_FocalLengthPrimary;
	float	g_AstigmatismAxisAngle;
	float	g_FocalLengthSecondary;
	float	g_AstigmatismAngle_Secondary;
	int		g_bIsAstigmatismOn;
};

cbuffer cbInputOutput : register( b2 )	//32  bytes
{
	int	g_bIsShadowOn;				// 4 byte
	int	g_bIsPhongShadingOn;		// 4 byte
	int	g_bIsNormalMapspingOn;		// 4 byte
	int	g_bIsGlossMappingOn;		// 4 bytes
	int	g_iAccelerationStructure;	// 4 byte
	int	g_iEnvMappingFlag;			// 4 bytes
	uint g_iNumBounces;				// 4 bytes
	int g_viPadding;				// 4 bytes
};

cbuffer cbLight : register( b3 )	// 32 bytes
{
	float3 g_vfLightPosition;		// 12 bytes
	//int g_vfPaddingLight[5];		// 20 bytes
	float g_Brightness;
};

cbuffer cbGlobalIllumination : register( b4 ) //	32 bytes
{
	float g_fRandX,g_fRandY;	// 8 bytes
	float g_fFactorAnt;			// 4 bytes
	float g_fFactorAct;			// 4 bytes
	uint g_uiNumMuestras;		// 4 bytes
	int3 g_vfPadding;			// 12 bytes
};

cbuffer cbSampleIndex : register( b5 )
{
	int	g_SampleIndexX;
	int	g_SampleIndexY;
	float	g_SampleGaussianSum;
	int 	g_Padding;
};

cbuffer cbReduction : register( b6 )
{
	uint g_RDLength;
	uint g_RDReadUAVIdx;//0:g_uAccumulation, 1:g_uResultBuffer
	float g_ScreenGaussianSum;
	float g_ScreenGaussianSigma;
};

