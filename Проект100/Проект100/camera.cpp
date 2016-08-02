// �������� ��� �����
#include "main.h"
#include "Camera.h"

// �������� ��������
#define kSpeed 15.0f

//�������� ������� � ���������� �����
float g_FrameInterval = 0.00f;

///////////////////////////////// GET FRAME TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////						��������� ����� � ���������� �����
/////
///////////////////////////////// GET FRAME TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void GetFrameTime()
{
	static float frameTime = 0.00f;	// ����� ���������� �����

	float currentTime = timeGetTime() * 0.001f;

	// �������� �������, ���������� � �������� �����
	g_FrameInterval = currentTime - frameTime;

	frameTime = currentTime;
}

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	���������� ���������������� ������ �� 2� ���������� ��������.
/////   2 ����� �������������� ������� �������� ���������, �� �������� ��
/////   � ���� �������������.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;

	// ���� � ��� ���� 2 ������� (������ ������� � ������������ ������), 
	// � ��� ���� ���������, �� ������� �� ����� ��������� ���� � 90 ��������.
	// ������� cross'a �����, �� ��� ������ ��������� � ������� ����. 
	// �������� X ��� ������� - (V1.y * V2.z) - (V1.z * V2.y)
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	// �������� Y - (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	// �������� Z: (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// ������ ���������.
	return vNormal;
}

/////				���������� �������� �������

float Magnitude(CVector3 vNormal)
{
	// ��� ���� ��� �������� ����� �������, 
	// �.�. ����� �������. �� ���������� ��� ���������� ��� ������������
	// �������. ��� �������: magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)   ��� V - ������.

	return (float)sqrt((vNormal.x * vNormal.x) +
		(vNormal.y * vNormal.y) +
		(vNormal.z * vNormal.z));
}

/////	���������� ����������������� ������, ������ ��������==1, 
/////	��� ������ ��� �������� �����.
CVector3 Normalize(CVector3 vVector)
{
	// �� ��������, ��� ���� ��� �-�? �� ������ ���������, ��� ��� ������ ��������������.
	// ������ �������������� - ������, ��� ������ ����� 1. ��������,
	// ������ (2,0,0) ����� ������������ ����� (1,0,0).

	// �������� �������� �������
	float magnitude = Magnitude(vVector);

	// ������ � ��� ���� ��������, � �� ����� ��������� ��� ������ �� ��� ��������.
	// ��� ������� ������ ������� ������ �������, ��� � ��� ����� ����� ��������.
	vVector = vVector / magnitude;

	return vVector;
}

/////	������� �-� ���������� �������� � ���. �����
void CCamera::SetViewByMouse()
{
	POINT mousePos;			// ��� ���������, �������� X � Y ������� ����

	// �������� " >> 1" - �� �� �����, ��� ������� �� 2, �� �**���� ������� ;)
	int middleX = SCREEN_WIDTH >> 1;	// ��������� �������� ������
	int middleY = SCREEN_HEIGHT >> 1;	// � �������� ������ ������
	float angleY = 0.0f;	// ����������� ������� �����/����
	float angleZ = 0.0f;	// ��������, ����������� ��� �������� �����-������ (�� ��� Y)
	static float currentRotX = 0.0f;

	// �������� ������� �����. ����
	GetCursorPos(&mousePos);

	// ���� ������ ������� � ��� �� ���������, �� �� ������� ������
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	// ������, ������� ���������� �������, ���������� ��� ������� � ��������.
	SetCursorPos(middleX, middleY);

	// ������ ��� ����� ����������� (��� ������), ���� ��������� ������.
	// ��� ������� - ������� ���������. ������ �������� ������� ����� � ������� �� ��
	// ����� ������� ����: VECTOR = P1 - P2; ��� P1 - ������� ����� (400,300 ��� 800�600).
	// ����� ��������� ������ X � Y (��� �����������), � ���� �������� 
	// �� 1000, ����� ������ ����� ����� �������.
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;		// ��������� ��������� ���� �������� 
	// � ���������� ������ currentRotX

	// ���� ������� �������� ������ 1 �������, ������� ���, ����� �� ������� ������� ������
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		// ������� �� ���������� ����
		if (lastRotX != 1.0f)
		{
			// ����� ����� ���, ������ ������� ��������� ����� � ����, ����� 
			// ����� ������, ���������������� ������� ������� ������ � 
			// ������������� �������.
			// ��� � ����� ���� ���. � ������ ��� ������������ ��� ���, 
			// ������� �� ������������� �.
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);

			// ������� ������ ������ ����� ��� �� �������� ����
			RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	// ���� ���� ������ -1.0f, ��������, ��� �������� �� �����������
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if (lastRotX != -1.0f)
		{
			// ����� �� ��������� ���
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);

			// �������
			RotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// ���� ������������ � ������� 1.0f -1.0f - ������ �������
	else
	{
		CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	// ������ ������� ������ ������ Y-���
	RotateView(angleY, 0, 1, 0);
}
/////	����������� ������ CCamera
CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		//�������������� ������ ����� ������� � ������� ����������
	CVector3 vView = CVector3(0.0, 1.0, 0.5);		//����. ������ �������
	CVector3 vUp = CVector3(0.0, 0.0, 1.0);		//������ ����.

	m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

/////	������������� �������, ������, ����. ������ ������
GLvoid CCamera::PositionCamera(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ,
	float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vPosition = CVector3(positionX, positionY, positionZ);
	CVector3 vView = CVector3(viewX, viewY, viewZ);
	CVector3 vUpVector = CVector3(upVectorX, upVectorY, upVectorZ);

	//�������� ��� ������ ������ ����� ���������� ����������.
	//����� ������ ����� ��������� ���������� ������ ���������� ������� ��������.

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}
/////	                  �������� ������ ������ ���

void CCamera::RotateView(float angle, float x, float y, float z)
{
	CVector3 vNewView;
	CVector3 vView;

	// ������� ��� ������ ������� (�����������, ���� �� �������)
	vView.x = m_vView.x - m_vPosition.x;	//����������� �� X
	vView.y = m_vView.y - m_vPosition.y;	//����������� �� Y
	vView.z = m_vView.z - m_vPosition.z;	//����������� �� Z


	// ���������� 1 ��� ����� � ������� ����������� ����
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// ������ ����� ������� X ��� ��������� �����
	vNewView.x = (cosTheta + (1 - cosTheta) * x * x)	* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// ������ ������� Y
	vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)	* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// � ������� Z
	vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)	* vView.z;

	// ������ ������ ������� ����� ������ �������� � ����� �������, �����
	// ���������� ����� ������ ������.
	m_vView.x = m_vPosition.x + vNewView.x;
	m_vView.y = m_vPosition.y + vNewView.y;
	m_vView.z = m_vPosition.z + vNewView.z;
}

/////			   �������� ������ � �������:
void CCamera::StrafeCamera(float speed)
{
	// ������� ������ ������� � �������
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// ������� ������ � �������
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

/////	�������� ������ ������-����� � �������� ���������
void CCamera::MoveCamera(float speed)
{
	CVector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.z += vVector.z * speed;

}

/////			������������ ����
void CCamera::CheckForMovement()
{
	// ������, ���� �������� �������, ��������� � ���������� �����, 
	// �� �������� ����������� �������� ���������� �������� �� �������� �������:
	float speed = kSpeed * g_FrameInterval;

	// � ������ �� ��� ������� ������� ���������� ��������:

	// ������ �� W
	if (GetKeyState('W') & 0x80) {
		MoveCamera(speed);
	}

	//  ��� S
	if (GetKeyState('S') & 0x80) {
		MoveCamera(-speed);
	}

	// ��� A
	if ( GetKeyState('A') & 0x80) {
		StrafeCamera(-speed);
	}

	//  ��� D
	if (GetKeyState('D') & 0x80) {
		// �������� ������
		StrafeCamera(speed);
	}
}

/////			��������� ������ ������� � ����. ������
void CCamera::Update()
{
	// ���� �� ������������ ������ ������� ������ ���, ����� �� ��������
	// ������. ��� ����������, �.�. �������� ������������ �� ������ ������.

	// ����. ���������� ��� ���������� cross 
	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	//����������� ������ �������
	m_vStrafe = Normalize(vCross);

	// ���������, ��������� �� ����
	SetViewByMouse();

	// �������� �������
	CheckForMovement();

	GetFrameTime();
}
/////			��������� ������ ������
void CCamera::Look()
{
	// ����� OpenGL �������, ������ � ����. ������ ������
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_vView.x, m_vView.y, m_vView.z,
		m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


