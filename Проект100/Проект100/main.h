#ifndef _MAIN_H
#define _MAIN_H

// ������, ����������� ��� ������ ���������
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "math.h"
#include <vector>
using namespace std;
// ������� ���������� ����������, ������, ������ � ������� ����� ������
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_DEPTH 16
extern float g_LightPosition[4];
// ���������� ��������� ����; ����� �������� �� ������ ������:
extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;

// ��������� 3D-�����
struct CVector31{
	float x, y, z;
};

// ��������� 2D-�����
struct CVector2{
	float x, y;
};

// ��� ��������� ��������. ��� ������������ ��� �������������� �������� ���������
// ������ � �������. ��� ���������� �������� ��� � ���, ����� ������ ������ � �������
// ������ �������� �����������. �� �� ����� �������� ���������� ���������.
struct tFace
{
	int vertIndex[3];           // indicies for the verts that make up this triangle
	int coordIndex[3];          // indicies for the tex coords to texture this face
};


// ��� ��������� ������ ���������� � ���������. ��� ����� ���� ���������� ����� �����.
struct tMaterialInfo
{
	char  strName[255];         // ��� ��������
	char  strFile[255];         // ��� ����� ��������
	BYTE  color[3];             // ���� ������� (R, G, B)
	int   texureId;             // ID ��������
	float uTile;                // u-tiling �������� (������ �� ������������)
	float vTile;                // v-tiling �������� (������ �� ������������)
	float uOffset;              // u-offset �������� (������ �� ������������)
	float vOffset;              // v-offset �������� (������ �� ������������)
};
// �������� ��� ���������� � ������/�����.
struct t3DObject
{
	int  numOfVerts;            // ����� ������ � ������
	int  numOfFaces;            // ����� ��������� � ������
	int  numTexVertex;          // ����� ���������� ���������
	int  materialID;            // ID �������� ��� �������������, ������ ������� �������
	bool bHasTexture;           // TRUE ���� ���� ���������� ����� ��� ����� �������
	char strName[255];          // ��� �������
	CVector31  *pVerts;          // ������ ������ �������
	CVector31  *pNormals;            // ������� �������
	CVector2  *pTexVerts;           // ���������� ����������
	tFace *pFaces;              // �������� �������
};



// �������� ���������� � ������. ���� ������� �� �������� � �����. �� ����� ������������
struct t3DModel
{
	int numOfObjects;           // ����� �������� � ������
	int numOfMaterials;         // ����� ���������� ������
	vector<tMaterialInfo> pMaterials;   // ����� �������� ���������� (�������� � �����)
	vector<t3DObject> pObject;      // ������ �������� � ������
};
//		����� �����, �������� ��� ����� � Frustrum.cpp:
//
///////////////////////////////////////////////////////////
class CFrustum {

public:

	// ���������� ������ ��� ��� �������� ������, ����� �������� ��������
	void CalculateFrustum();

	// ��������� 3D ����� � ���������� TRUE, ���� ��� ��������� ������ frustrum'a
	bool PointInFrustum(float x, float y, float z);

	// ��������� ����� � ������ � ���������� TRUE, ���� ��� ��������� ������ frustrum'a
	bool SphereInFrustum(float x, float y, float z, float radius);

	// ��������� ����� � �������� ������ ����
	bool CubeInFrustum(float x, float y, float z, float size);

private:

	// ������ A B C � D ���������� ��� ������ ������� ��������.
	float m_Frustum[6][4];
};
class CVector3 {

public:
	// ��������� �����������
	CVector3() {}

	// �����������, ���������������� ������
	CVector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}
	// ����������� �������� +, ����� ���������� ���� � ����� �������
	CVector3 operator+(CVector3 vVector)
	{
		// ���������� +����������� ������
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// ����������� �������� -
	CVector3 operator-(CVector3 vVector)
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	// ����������� �������� *
	CVector3 operator*(float num)
	{
		return CVector3(x*num, y*num, z*num);
	}

	// ����������� �������� /
	CVector3 operator/(float num)
	{
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;				// ������ float ��� X,Y,Z
};

// �������� ������� ������� ��������� - WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// �������� ������� ��������� ���������
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// ������� - ������� ���� ���������
WPARAM MainLoop();

// �������, ��������� ����
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// �������, ��������������� ������ �������
bool bSetupPixelFormat(HDC hdc);

// �������� �������, ��������������� ������� ���� OpenGL
void SizeOpenGLScreen(int width, int height);

// �������, ���������������� OpenGL
void InitializeOpenGL(int width, int height);

// ����� �������������
void Init(HWND hWnd);

// �������, ������� ���������� ������ �����
void RenderScene();

// ��-�������������
void DeInit();

#endif