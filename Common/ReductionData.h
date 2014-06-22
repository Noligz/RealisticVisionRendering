#ifndef REDUCTION_DATA_H
#define REDUCTION_DATA_H

class ReductionData
{
private:
	unsigned int m_Length;
	unsigned int m_ReadUAVIdx;//0:g_uAccumulation, 1:g_uResultBuffer
	float m_ScreenGaussianSum;
	float m_ScreenGaussianSigma;
public:
	ReductionData(unsigned int length, unsigned int readUAVIdx, float screenGausianSum, float screenGaussianSigma);
	unsigned int GetLength() { return m_Length; }
	unsigned int GetReadUAVIdx() { return m_ReadUAVIdx; }
	float GetScreenGaussianSum() { return m_ScreenGaussianSum;}
	float GetScreenGaussianSigma() { return m_ScreenGaussianSigma; }
	void NextLength(unsigned int shrink);
	void NextReadUAVIdx();
	bool IsShrinkable();
	int GetDispatchDimension(int pixelsPerGroup);
};

#endif