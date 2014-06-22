[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void CSClearBuffer( uint3 DTid : SV_DispatchThreadID )
{
	unsigned int index = DTid.y * SCREEN_WIDTH + DTid.x;
	g_uResultBuffer[index] = 0.0f;
}

[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void CSCopy2Display( uint3 DTid : SV_DispatchThreadID )
{
	unsigned int index = DTid.y * SCREEN_WIDTH + DTid.x;
	g_uResultTexture[DTid.xy] = g_uResultBuffer[index];
}

[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void CSApplyGaussian( uint3 DTid : SV_DispatchThreadID )
{
	float	tmpX = DTid.x / float(SCREEN_WIDTH - 1) * 2.f - 1.f;
	float	tmpY = DTid.y / float(SCREEN_HEIGHT - 1) * 2.f - 1.f;
	float weight = exp(-(tmpX*tmpX + tmpY*tmpY)/2/g_ScreenGaussianSigma/g_ScreenGaussianSigma);

	unsigned int index = DTid.y * SCREEN_WIDTH + DTid.x;
	g_uResultBuffer[index] *= weight;
}


[numthreads(1, 1, 1)]
void CSCalculatePupilSize( uint3 DTid : SV_DispatchThreadID, uint3 GID : SV_GroupID, uint GI : SV_GroupIndex)
{
	unsigned int pixelCount = SCREEN_HEIGHT * SCREEN_WIDTH;

	float4 colorSum;
	if(g_RDReadUAVIdx)
		colorSum = g_uAccumulation[0];
	else
		colorSum = g_uResultBuffer[0];
	//colorSum = colorSum / pixelCount;
	colorSum /= g_ScreenGaussianSum;
	float brightness = 0.299 * colorSum.r + 0.587 * colorSum.g + 0.114 * colorSum.b;
	float newPupilSize = 0.004157 + 0.0045864 * (1 - brightness);
	float oldPupilSize = g_uAccumulation[pixelCount].y;
	g_uAccumulation[pixelCount].x = brightness;
	g_uAccumulation[pixelCount].y = lerp(oldPupilSize, newPupilSize, 0.3f);
}