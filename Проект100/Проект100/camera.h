#ifndef _CAMERA_H
#define _CAMERA_H

class CCamera {
public:
	CCamera();

	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// ��� ������� ��� ������� ����� � ��������� ������
	CVector3 Position() { return m_vPosition; }
	CVector3 View()		{ return m_vView; }
	CVector3 UpVector() { return m_vUpVector; }
	CVector3 Strafe()	{ return m_vStrafe; }
	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	void PositionCamera(float positionX, float positionY, float positionZ,
		float viewX, float viewY, float viewZ,
		float upVectorX, float upVectorY, float upVectorZ);

	void RotateView(float angle, float X, float Y, float Z);
	void SetViewByMouse();

	// ����� ������� - ������ ������
	void StrafeCamera(float speed);

	void MoveCamera(float speed);

	void CheckForMovement();

	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *
	// ������� ��������� ������ ������ � ������ ������
	void Update();
	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// ������� ���������� gluLookAt(), ����� ������� OpenGL, ���� ��������
	void Look();

private:
	CVector3 m_vPosition;
	CVector3 m_vView;
	CVector3 m_vUpVector;
	CVector3 m_vStrafe;
};

#endif
