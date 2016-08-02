#ifndef _MAIN_H
#define _MAIN_H

// Хидеры, необходимые для работы программы
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "math.h"
#include <vector>
using namespace std;
// Обьявим глобальные переменные, ширину, высоту и глубину цвета экрана
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_DEPTH 16
extern float g_LightPosition[4];
// Глобальные параметры окна; будут доступны из других файлов:
extern HWND  g_hWnd;
extern RECT  g_rRect;
extern HDC   g_hDC;
extern HGLRC g_hRC;
extern HINSTANCE g_hInstance;

// Структура 3D-точки
struct CVector31{
	float x, y, z;
};

// Структура 2D-точки
struct CVector2{
	float x, y;
};

// Это структура полигона. Она используется для индексирования массивов координат
// вершин и текстур. Эта информация сообщает нам о том, какие номера вершин в массиве
// какому полигону принадлежат. То же самое касается текстурных координат.
struct tFace
{
	int vertIndex[3];           // indicies for the verts that make up this triangle
	int coordIndex[3];          // indicies for the tex coords to texture this face
};


// Эта структура хранит информацию о материале. Это может быть текстурная карта света.
struct tMaterialInfo
{
	char  strName[255];         // Имя текстуры
	char  strFile[255];         // Имя файла текстуры
	BYTE  color[3];             // Цвет обьекта (R, G, B)
	int   texureId;             // ID текстуры
	float uTile;                // u-tiling текстуры (Сейчас не используется)
	float vTile;                // v-tiling текстуры (Сейчас не используется)
	float uOffset;              // u-offset текстуры (Сейчас не используется)
	float vOffset;              // v-offset текстуры (Сейчас не используется)
};
// Содержит всю информацию о модели/сцене.
struct t3DObject
{
	int  numOfVerts;            // Число вершин в модели
	int  numOfFaces;            // Число полигонов в модели
	int  numTexVertex;          // Число текстурных координат
	int  materialID;            // ID текстуры для использования, индекс массива текстур
	bool bHasTexture;           // TRUE если есть текстурная карта для этого обьекта
	char strName[255];          // Имя обьекта
	CVector31  *pVerts;          // Массив вершин обьекта
	CVector31  *pNormals;            // Нормали обьекта
	CVector2  *pTexVerts;           // Текстурные координаты
	tFace *pFaces;              // Полигоны обьекта
};



// Содержит информацию о модели. Тоже неплохо бы обернуть в класс. Мы будем использовать
struct t3DModel
{
	int numOfObjects;           // Число обьектов в модели
	int numOfMaterials;         // Число материалов модели
	vector<tMaterialInfo> pMaterials;   // Число обьектов материалов (текстуры и цвета)
	vector<t3DObject> pObject;      // Список обьектов в модели
};
//		Новый класс, описание его будет в Frustrum.cpp:
//
///////////////////////////////////////////////////////////
class CFrustum {

public:

	// Вызывается каждый раз при движении камеры, чтобы обновить пирамиду
	void CalculateFrustum();

	// Принимает 3D точку и возвращает TRUE, если она находится внутри frustrum'a
	bool PointInFrustum(float x, float y, float z);

	// Принимает точку и радиус и возвращает TRUE, если она находится внутри frustrum'a
	bool SphereInFrustum(float x, float y, float z, float radius);

	// Принимает центр и половину длинны куба
	bool CubeInFrustum(float x, float y, float z, float size);

private:

	// Хранит A B C и D переменные для каждой стороны пирамиды.
	float m_Frustum[6][4];
};
class CVector3 {

public:
	// Дефолтный конструктор
	CVector3() {}

	// Конструктор, инициализирующий данные
	CVector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}
	// Перегружаем оператор +, чтобы прибавлять друг к другу векторы
	CVector3 operator+(CVector3 vVector)
	{
		// Возвращаем +добавленный вектор
		return CVector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	// перегружаем оператор -
	CVector3 operator-(CVector3 vVector)
	{
		return CVector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	// Перегружаем оператор *
	CVector3 operator*(float num)
	{
		return CVector3(x*num, y*num, z*num);
	}

	// Перегружаем оператор /
	CVector3 operator/(float num)
	{
		return CVector3(x / num, y / num, z / num);
	}

	float x, y, z;				// Просто float для X,Y,Z
};

// Прототип главной функции программы - WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);

// Прототип функции обработки сообщений
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Функция - главный цикл программы
WPARAM MainLoop();

// Функция, создающая окно
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// Функция, устанавливающая формат пиксела
bool bSetupPixelFormat(HDC hdc);

// Прототип функции, устанавливающей размеры окна OpenGL
void SizeOpenGLScreen(int width, int height);

// Функция, инициализирующая OpenGL
void InitializeOpenGL(int width, int height);

// Общая инициализация
void Init(HWND hWnd);

// Функция, которая собственно рисует сцену
void RenderScene();

// Де-инициализация
void DeInit();

#endif