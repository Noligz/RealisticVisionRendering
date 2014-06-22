#include "Eye.h"

Eye::Eye()
{
	Reset();
}

void Eye::Reset()
{
	m_AstigmatismAxisAngle = 180;
	m_DiopterCylinder = 0.0f;
	m_DiopterSphere = 0.0f;
	m_FocalDistance = 10.0f;
	m_Pupil2Retina = 0.024f;
	m_PupilRadius = 0.005f;

	m_DeltaAstigmatismAxisAngle = 5;
	m_DeltaDiopterCylinder = 0.25f;
	m_DeltaDiopterSphere = 0.25f;
	m_DeltaFocalDistance = 0.5f;
}

void Eye::PrintEyeData()
{
	printf("FD: %.1f,\tDS: %.2f,\tDC: %.2f,\tA: %.0f\n", m_FocalDistance, m_DiopterSphere, m_DiopterCylinder, m_AstigmatismAxisAngle);
}

float Eye::GetAstigmatismAxisAngle()
{
	float angle = m_AstigmatismAxisAngle;
	if(angle > 90)
		angle -= 180;
	return angle / 180.0f * PI;
}

float Eye::CalFocalLength(float diopter)
{
	return m_Pupil2Retina / (1.0f - m_Pupil2Retina * diopter);
}

float Eye::GetLengthFocalLengthPrimary()
{
	return CalFocalLength(m_DiopterSphere);
}

float Eye::GetLengthFocalLengthSecondary()
{
	return CalFocalLength(m_DiopterSphere + m_DiopterCylinder);
}

void Eye::CorrectAstigmatismAxisAngle()
{
	if(m_AstigmatismAxisAngle > 180)
		m_AstigmatismAxisAngle -= 180;
	if(m_AstigmatismAxisAngle <= 0)
		m_AstigmatismAxisAngle += 180;
}

void Eye::AstigmatismAxisAngle_Plus()
{
	m_AstigmatismAxisAngle += m_DeltaAstigmatismAxisAngle;
	CorrectAstigmatismAxisAngle();
}

void Eye::AstigmatismAxisAngle_Minus()
{
	m_AstigmatismAxisAngle -= m_DeltaAstigmatismAxisAngle;
	CorrectAstigmatismAxisAngle();
}

void Eye::SetAstigmatismAxisAngle(float angle)
{
	m_AstigmatismAxisAngle = angle;
	CorrectAstigmatismAxisAngle();
}

void Eye::FocalDistance_Plus()
{
	if(m_FocalDistance > 1)
		m_FocalDistance += m_DeltaFocalDistance;
	else
		m_FocalDistance += 0.1;
}

void Eye::FocalDistance_Minus()
{
	if(m_FocalDistance > 1)
		m_FocalDistance -= m_DeltaFocalDistance;
	else
		m_FocalDistance -= 0.1;
	if(m_FocalDistance <= 0)
		m_FocalDistance = m_DeltaFocalDistance;
}

void Eye::SetFocalDistant(float distant)
{
	m_FocalDistance = distant;
}

void Eye::DiopterSphere_Plus()
{
	m_DiopterSphere += m_DeltaDiopterSphere;
}

void Eye::DiopterSphere_Minus()
{
	m_DiopterSphere -= m_DeltaDiopterSphere;
}

void Eye::SetDiopterSphere(float diopter)
{
	m_DiopterSphere = diopter;
}

void Eye::DiopterCylinder_Plus()
{
	m_DiopterCylinder += m_DeltaDiopterCylinder;
}

void Eye::DiopterCylinder_Minus()
{
	m_DiopterCylinder -= m_DeltaDiopterCylinder;
}

void Eye::SetDiopterCylinder(float diopter)
{
	m_DiopterCylinder = diopter;
}



