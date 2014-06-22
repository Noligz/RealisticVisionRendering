[numthreads(BLOCK_SIZE_X, BLOCK_SIZE_Y, 1)]
void CSReconstruction( uint3 DTid : SV_DispatchThreadID )
{
	if(DTid.x % SAMPLE_COUNT_X != 0 || DTid.y % SAMPLE_COUNT_Y != 0)
		return;
	float4 colSum = float4(0.f, 0.f, 0.f, 0.f);
	//unsigned int index = DTid.y * SCREEN_WIDTH * SAMPLE_COUNT_X + DTid.x;
	int sampleCount = SAMPLE_COUNT_X * SAMPLE_COUNT_Y;
	for(int j = 0; j < SAMPLE_COUNT_Y; j++)
	{
		unsigned int index = (DTid.y + j) * SCREEN_WIDTH * SAMPLE_COUNT_X + DTid.x;
		for(int i = 0; i < SAMPLE_COUNT_X; i++)
			colSum += g_uAccumulation[index + i];
	}
	float2 screenPos = float2(DTid.x / float(SAMPLE_COUNT_X), DTid.y / float(SAMPLE_COUNT_Y));
	g_uResultTexture[screenPos.xy] = colSum / float(sampleCount);
}