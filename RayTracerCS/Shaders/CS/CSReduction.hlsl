[numthreads(RD_THREAD_COUNT, RD_THREAD_COUNT, 1)]
void CSReduction( uint3 DTid : SV_DispatchThreadID, uint3 GID : SV_GroupID, uint GI : SV_GroupIndex)
{
	unsigned int inputIdx = GID.x * RD_THREAD_COUNT * RD_THREAD_COUNT * RD_ADD_PIXEL_COUNT + GI * RD_ADD_PIXEL_COUNT;
	if(inputIdx >= g_RDLength)
		return;

	float4 sum = 0.0f;
	for(uint i = 0; i < RD_ADD_PIXEL_COUNT; i++)
	{
		uint idx = inputIdx + i;
		if(idx >= g_RDLength)
			break;
		if(g_RDReadUAVIdx)
			sum += g_uResultBuffer[idx];
		else
			sum += g_uAccumulation[idx];
	}

	unsigned int outputIdx = inputIdx / RD_ADD_PIXEL_COUNT;
	if(g_RDReadUAVIdx)
		g_uAccumulation[outputIdx] = sum;
	else
		g_uResultBuffer[outputIdx] = sum;
}