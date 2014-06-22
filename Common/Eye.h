#ifndef EYE_H
#define EYE_H

#include <iostream>

#define PI 3.1415926

class Eye
{
public:
	Eye();
	~Eye();

private:
	float m_Pupil2Retina;
	float m_PupilRadius;
	float m_DiopterSphere;
	float m_DiopterCylinder;
	float m_AstigmatismAxisAngle;
	float m_FocalDistance;

	float m_DeltaDiopterSphere;
	float m_DeltaDiopterCylinder;
	float m_DeltaFocalDistance;
	float m_DeltaAstigmatismAxisAngle;

private:
	float CalFocalLength(float diopter);
	void CorrectAstigmatismAxisAngle();
public:
	void PrintEyeData();
	float GetFocalDistance() { return m_FocalDistance; }
	float GetLengthPupil2Retina() { return m_Pupil2Retina; }
	float GetPupilRadius() { return m_PupilRadius; }
	float GetLengthFocalLengthPrimary();
	float GetLengthFocalLengthSecondary();
	float GetAstigmatismAxisAngle();
	void Reset();
	void DiopterSphere_Plus();
	void DiopterSphere_Minus();
	void SetDiopterSphere(float diopter);
	void DiopterCylinder_Plus();
	void DiopterCylinder_Minus();
	void SetDiopterCylinder(float diopter);
	void AstigmatismAxisAngle_Plus();
	void AstigmatismAxisAngle_Minus();
	void SetAstigmatismAxisAngle(float angle);
	void FocalDistance_Plus();
	void FocalDistance_Minus();
	void SetFocalDistant(float distant);
};

#endif // !EYE_H
