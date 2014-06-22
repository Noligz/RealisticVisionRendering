#include "ReductionData.h"

ReductionData::ReductionData(unsigned int length, unsigned int readUAVIdx, float screenGaussianSum, float screenGaussianSigma)
{
	m_Length = length;
	m_ReadUAVIdx = readUAVIdx;
	m_ScreenGaussianSum = screenGaussianSum;
	m_ScreenGaussianSigma = screenGaussianSigma;
}

void ReductionData::NextLength(unsigned int shrink)
{
	if(!IsShrinkable())
		return;
	unsigned int remaining = 0;
	if(m_Length % shrink > 0)
		remaining = 1;
	m_Length = m_Length / shrink + remaining;
}

void ReductionData::NextReadUAVIdx()
{
	m_ReadUAVIdx = 1 - m_ReadUAVIdx;
}

bool ReductionData::IsShrinkable()
{
	return (m_Length > 1);
}

int ReductionData::GetDispatchDimension(int pixelsPerGroup)
{
	int remaining = 0;
	if(m_Length % pixelsPerGroup > 0)
		remaining = 1;
	return (m_Length / pixelsPerGroup + remaining);
}