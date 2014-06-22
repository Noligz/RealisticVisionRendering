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

//-------------------------------------------------------------------------------------------
// PRIMARY RAYS STAGE
//-------------------------------------------------------------------------------------------
// Primary rays.

#define HALFPI 1.5707963
#define MATH_PI 3.1415926

float GetPupilRadius()
{
	float pupilRadius = g_uAccumulation[SCREEN_HEIGHT * SCREEN_WIDTH].y/2;
	float minR = 0.00001;
	if(pupilRadius < minR)
		pupilRadius = minR;
	return pupilRadius;
}

[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void CSGeneratePrimaryRays(uint3 DTid : SV_DispatchThreadID, uint GIndex : SV_GroupIndex )
{
#ifdef GLOBAL_ILLUM
	//float2 indexasfloat = float2(DTid.x,DTid.y)/float(SCREEN_WIDTH);
	//float2 jitterOffset = indexasfloat+float2(g_fRandX,g_fRandY);
	//float2 tx_Random = g_sRandomTx.SampleLevel(g_ssSampler, jitterOffset, 0).xy;
	//float2 jitter = 2.0f*tx_Random;
	//float inverse = 1.0f/(float(SCREEN_WIDTH));
	//float y = -float(2.f * DTid.y + jitter.x + 1.f - SCREEN_WIDTH) * inverse;
	//float x = float(2.f * DTid.x + jitter.y + 1.f - SCREEN_WIDTH) * inverse;	
	//float z = 2.0f;
#else
	//float inverse = 1.0f/(float(SCREEN_WIDTH));
	float2 screenPos = float2(DTid.x , DTid.y );
	float y = -float(2.f * screenPos.y + 1.f - SCREEN_HEIGHT) / float(SCREEN_HEIGHT);
	float x = float(2.f * screenPos.x + 1.f - SCREEN_WIDTH) / float(SCREEN_HEIGHT);	
	float z = 2.0f;
#endif

	float pupilRadius = GetPupilRadius();

	float tmpX = 0.0f;
	float tmpY = 0.0f;
	float3 samplePntAtLen;
	float angle = HALFPI;

	bool isGridSample = false;
	if(isGridSample)	//Grid Sample
	{
		if(SAMPLE_COUNT_X > 1)
			tmpX = g_SampleIndexX / float(SAMPLE_COUNT_X - 1) * 2.f - 1.f;
		if(SAMPLE_COUNT_Y > 1)
			tmpY = g_SampleIndexY / float(SAMPLE_COUNT_Y - 1) * 2.f - 1.f;
		samplePntAtLen = float3(tmpX * pupilRadius, tmpY * pupilRadius, 0.0f);
		if(abs(tmpX) > 0.00001)
			angle = atan(tmpY / tmpX);
	}
	else    //Circle Sample
	{
		if(SAMPLE_COUNT_X > 1)
			tmpX = (g_SampleIndexX + 1) / float(SAMPLE_COUNT_X - 0);
		if(SAMPLE_COUNT_Y > 1)
			tmpY = (g_SampleIndexY + 1) / float(SAMPLE_COUNT_Y - 0);
		float r = pupilRadius * tmpX;
		angle = tmpY * 2 * MATH_PI + tmpX * 2 * MATH_PI / SAMPLE_COUNT_Y;
		samplePntAtLen = float3(r * cos(angle), r * sin(angle), 0.0f);
	}
	
	if(angle > MATH_PI)
		angle -= MATH_PI;
	float includedAngle = abs(angle - g_AstigmatismAxisAngle);
	if(includedAngle > HALFPI)
		includedAngle = MATH_PI - includedAngle;

	float focalDistanceInEye = lerp(g_FocalLengthPrimary, g_FocalLengthSecondary, includedAngle / HALFPI);
		//focalDistanceInEye= g_FocalLengthPrimary;

	float3 pntAtRetina_Pos = float3(-x,-y,-z) / z * g_Pupil2Retina;
	float3 dirSamplePntAtLen2PntAtRetina = pntAtRetina_Pos - samplePntAtLen;
	float k = -focalDistanceInEye / dirSamplePntAtLen2PntAtRetina.z;
	float3 focusPntInEye = samplePntAtLen + k * dirSamplePntAtLen2PntAtRetina;
	float3 pntAtFocalPane = -focusPntInEye * g_FocalDistance / focalDistanceInEye;

	float3 pntAtFocalPane_World = mul(float4(pntAtFocalPane.x, pntAtFocalPane.y, pntAtFocalPane.z, 1.0f), 
		g_mfWorld).xyz;
	float3 samplePntAtLen_World = mul(float4(samplePntAtLen.x, samplePntAtLen.y, samplePntAtLen.z, 1.0f),
		g_mfWorld).xyz;

	Ray ray;
	ray.vfOrigin = samplePntAtLen_World;
	ray.vfDirection = normalize(pntAtFocalPane_World - ray.vfOrigin);

	//// Create new ray from the camera position to the pixel position
	//Ray ray;
	//float4 aux = (mul(float4(0,0,0,1.f),g_mfWorld));
	//ray.vfOrigin = aux.xyz/aux.w;
	//float3 vfPixelPosition = mul(float4(x,y,z,1.f),
	//	g_mfWorld).xyz;
	//ray.vfDirection = normalize(vfPixelPosition-ray.vfOrigin);

	ray.fMaxT = 10000000000000000.f;
	ray.fMinT = 0;
	ray.vfReflectiveFactor = float3(1.f,1.f,1.f);
	ray.iTriangleId = -1;

	unsigned int index = DTid.y * SCREEN_WIDTH + DTid.x;
	// Copy ray to global UAV
	g_uRays[index] = ray;
//	// Initialize accumulation buffer and result buffer
//#ifdef GLOBAL_ILLUM
//	g_uAccumulation[index] *= g_fFactorAnt;
//#else
	g_uAccumulation[index] = 0.0f;
//#endif
//
//	g_uResultTexture[screenPos.xy] = 0.f;
}
