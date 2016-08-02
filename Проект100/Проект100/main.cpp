#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0501
#define _WIN32_IE 0x0600
#include "main.h"
#include <OGL\glext.h>
#include <glut.h>
#include "font.h"
#include "camera.h"
#include <wchar.h>
#include "texture.h"
#include "3ds.h"
#include "Versh.h"

   
float g_LightPosition[4] = { 1, 1, 1, 1 };         // Позиция источника света
float g_bLight = true;                           // Включен ли свет
bool  g_bLighting = true;        
float g_RotateX = 0.0f;       
float g_RotationSpeed = 0.8f;         

float aa=0, bb=0;
float s = 45, stop=0,stopp=0, angelxx=0;
float a1, b1, a2, b2, a3, b3, a4, b4, a5, b5, a6, b6, a7, b7;
float a11, a22, a33, a44, a55, a66, a77;
float ab1, ab2, ab3, ab4, ab5, ab6, ab7;
float speedrob = 0.25;
float speedUpDw = 0.01;
bool flag0=0,flag1=0, flag2=0, flag3=0,flag4=0,flag5=0,flag6=0,flag7=0,flag8=0,flag9=0,flag10=0;
//класс камеры
CCamera g_Camera;
//шрифт
CFont *Font;
float FPS;
int angelx = 0, angelz = 0,number,num=1;
//окно
HWND  g_hWnd;
RECT  g_rRect;
HDC   g_hDC;
HGLRC g_hRC;
HINSTANCE g_hInstance;
PFNGLMULTITEXCOORD2FARBPROC		glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC		glActiveTextureARB = NULL;

void vpered()
{
	if (flag0 == 0)
	{
		aa = aa + speedrob;
		if (stop >= 25)
		{
			flag0 = 1;
		}
	}
}
void cubUp()
{
	if (flag0 == 1)
	{
		switch (num)
		{
		case 1:
			if (a1 > 2)
			{	
				flag9 = 1;
			}
			else
			{
				a1 = a1 + speedUpDw;
			}
			break;
		case 2:
			if (a2 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a2 = a2 + speedUpDw;
			}
			break;
		case 3:
			if (a3 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a3 = a3 + speedUpDw;
			}
			break;
		case 4:
			if (a4 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a4 = a4 + speedUpDw;
			}
			break;
		case 5:
			if (a5 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a5 = a5 + speedUpDw;
			}
			break;
		case 6:
			if (a6 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a6 = a6 + speedUpDw;
			}
			break;
		case 7:
			if (a7 > 2)
			{
				flag9 = 1;
			}
			else
			{
				a7 = a7 + speedUpDw;
			}
			break;
		}

	}
}
void povorot()
{
	if ((flag0 == 1) & (flag1 == 0) & (flag9 == 1))
	{
		angelxx--; 
		switch (num)
		{
		case 1: ab1 = ab1 + 0.017;
			break;
		case 2: ab2 = ab2 + 0.017;
			break;
		case 3: ab3 = ab3 + 0.017;
			break;
		case 4: ab4 = ab4 + 0.017;
			break;
		case 5: ab5 = ab5 + 0.017;
			break;
		case 6: ab6 = ab6 + 0.017;
			break;
		case 7: ab7 = ab7 + 0.017;
			break;
		}
		
		if (angelxx ==-180)
		{
			flag1 = 1;
		}
	}
}
void nazad()
{
	if ((flag1 == 1) & (flag2 == 0))
	{
	aa = aa - speedrob;
	switch (num)
	{
	case 1:b1 = b1 - speedrob;
		break;
	case 2: b2 = b2 - speedrob;
		break;
	case 3: b3 = b3 - speedrob;
		break;
	case 4: b4 = b4 - speedrob;
		break;
	case 5: b5 = b5 - speedrob;
		break;
	case 6: b6 = b6 - speedrob;
		break;
	case 7: b7 = b7 - speedrob;
		break;
	}
	if (stop <= -25)
	{
		flag2 = 1;
	}
	}
	
}
void CubDown()
{
	if (flag2==1)
	{
		switch (num)
		{
		case 1:
			if (a11 >1.9)
			{
				flag10 = 1;
			}
			else
			{
				a11 = a11 + speedUpDw;
			}
			break;
		case 2:
			if (a22 > 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a22 = a22+ speedUpDw;
			}
			break;
		case 3:
			if (a33 > 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a33 = a33 + speedUpDw;
			}
			break;
		case 4:
			if (a44> 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a44 = a44 + speedUpDw;
			}
			break;
		case 5:
			if (a55 > 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a55 = a55 + speedUpDw;
			}
			break;
		case 6:
			if (a66 > 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a66 = a66 + speedUpDw;
			}
			break;
		case 7:
			if (a77 > 1.9)
			{
				flag10 = 1;
			}
			else
			{
				a77 = a77 + speedUpDw;
			}
			break;

		}
	}
}
void Right()
{
	if ((flag2==1) & (flag5 == 0) & (flag10==1))//разворачиваемся после опускания ящика
	{
		angelxx++;
		if (angelxx == 0)
		{
			flag5 = 1;
		}	
	}
	if ((flag2 == 1) & (flag5 == 1) & (flag3 == 0))
	{
		if (flag4 == 0)
		{
			aa = aa + speedrob;  //едем к центру
			if (stop >=0)   
			{
				flag4 = 1;
			}
		}
		if ((flag4 == 1) & (flag3 == 0))
		{
			angelxx++;					//поворачиваемся
			if (angelxx == 90)
			{
				flag3 = 1;
			}
		}
	}
		if ((flag6 == 0) & (flag3 == 1))
		{
			bb = bb - speedrob;			//едем по центру к след ящику
			if (s > -40)
			{
				if (stopp <= s)
				{
					flag6 = 1;
					s = s - 15;
				}
			}
		}
		if ((flag6 == 1) & (flag7 == 0))
		{
			angelxx--;					//поворот к ящику
			if (angelxx == 0)
			{
				flag8 = 0;
				flag7 = 1;

			}
		}
		if ((flag7 == 1) & (flag8 == 0))
		{
			aa = aa + speedrob;			//едем к ящику и заираем
			if (stop >= 25)
			{
					flag8 = 1;
					flag1 = 0;
					flag2 = 0;
					flag3 = 0;
					flag4 = 0;
					flag5 = 0;
					flag6 = 0;
					flag7 = 0;
					flag9 = 0;
					flag10 = 0;
					num++;
			}
		}
	if (stopp < -60) /// останавливаем мусорщика в конце улицы
	{
		flag0 = 1;
		flag1 = 1;
		flag2 = 1;
		flag3 = 1;
		flag4 = 1;
		flag5 = 1;
		flag6 = 1;
		flag7 = 1;
		flag8 = 1;
	}
}

void Draw3DSGrid()
{
	// Просто рисуем по 100 зеленых линий вертикально и горизонтально по осям X и Z

	glColor3ub(0, 255, 0);

	// Рисуем сетку 1х1 вдоль осей
	for (float i = -50; i <= 50; i += 1)
	{
		glBegin(GL_LINES);
		// Ось Х
		glVertex3f(-50, 0, i);
		glVertex3f(50, 0, i);

		// Ось Z
		glVertex3f(i, 0, -50);
		glVertex3f(i, 0, 50);
		glEnd();
	}
}

void CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;	//наши фпс
	static float lastTime = 0.0f;			//Тут хранится время, прошедшее с последнего кадра
	static char strFrameRate[50] = { 0 };		//Строка для вывода

	float currenttTime = timeGetTime() * 0.001f;

	++framesPerSecond;

	if (currenttTime - lastTime > 1.0f)
	{
		lastTime = currenttTime;
		FPS = framesPerSecond;
		framesPerSecond = 0;
	}
}

void DrawRoom()
{
	//стены
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);
	glVertexPointer(3, GL_INT, 0, room);
	glTexCoordPointer(2, GL_INT, 0, romtext);
	glDrawArrays(GL_QUADS, 0, 16);
	//пол
	glBindTexture(GL_TEXTURE_2D, textures[1].texID);
	glVertexPointer(3, GL_INT, 0, roomflor);
	glTexCoordPointer(2, GL_INT, 0, floortext);
	glDrawArrays(GL_QUADS, 0, 4);
	//trot
	glBindTexture(GL_TEXTURE_2D, textures[4].texID);
	glVertexPointer(3, GL_FLOAT, 0, trotflor);
	glTexCoordPointer(2, GL_INT, 0, floortext);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, textures[4].texID);
	glVertexPointer(3, GL_FLOAT, 0, trotflor1);
	glTexCoordPointer(2, GL_INT, 0, floortext);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, textures[5].texID);
	glVertexPointer(3, GL_FLOAT, 0, polos);
	glTexCoordPointer(2, GL_INT, 0, floortext);
	glDrawArrays(GL_QUADS, 0, 4);
	//sky
	glBindTexture(GL_TEXTURE_2D, textures[3].texID);
	glVertexPointer(3, GL_INT, 0, sky);
	glTexCoordPointer(2, GL_INT, 0, cordsky);
	glDrawArrays(GL_QUADS, 0, 4);
}

void cub(int numb)
{
	switch (numb)
	{
	case 1:	
		glTranslated(28 - 2.5 * (1 - cos(ab1)), 0 + a1-a11, 65 + 2.5 * (sin(ab1)));
		glTranslated(b1, 0,0);
		glRotatef(-ab1*59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24); 
		break;
	case 2:
		glTranslated(28 - 2.5 * (1 - cos(ab2)), 0 + a2-a22, 45 + 2.5 * (sin(ab2)));
		glTranslated(b2, 0, 0);
		glRotatef(-ab2 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24);
		break;
	case 3:
		glTranslated(28 - 2.5 * (1 - cos(ab3)), 0 + a3 - a33, 30 + 2.5 * (sin(ab3)));
		glTranslated(b3, 0, 0);
		glRotatef(-ab3 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24); 
		break;
	case 4:
		glTranslated(28 - 2.5 * (1 - cos(ab4)), 0 + a4 - a44, 15 + 2.5 * (sin(ab4)));
		glTranslated(b4, 0, 0);
		glRotatef(-ab4 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24); 
		break;
	case 5:
		glTranslated(28 - 2.5 * (1 - cos(ab5)), 0 + a5 - a55, 0 + 2.5 * (sin(ab5)));
		glTranslated(b5, 0, 0);
		glRotatef(-ab5 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24); 
		break;
	case 6:
		glTranslated(28 - 2.5 * (1 - cos(ab6)), 0 + a6 - a66, -15 + 2.5 * (sin(ab6)));
		glTranslated(b6, 0, 0);
		glRotatef(-ab6 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24);
		break;
	case 7:
		glTranslated(28 - 2.5 * (1 - cos(ab7)), 0 + a7 - a77, -30 + 2.5 * (sin(ab7)));
		glTranslated(b7, 0, 0);
		glRotatef(-ab7 * 59, 0, 1, 0);
		glBindTexture(GL_TEXTURE_2D, textures[2].texID);
		glVertexPointer(3, GL_INT, 0, cubik);
		glTexCoordPointer(2, GL_INT, 0, corcub);
		glDrawArrays(GL_QUADS, 0, 24);
		break;
	}
}
/// RENDER SCENE 
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLightfv(GL_LIGHT0, GL_POSITION, g_LightPosition);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glLoadIdentity();
	g_Camera.Look();
	glTranslatef(0, -1, -5);

	glPushMatrix();
	glTranslated(0, -1, 0);
	DrawRoom();
	glPopMatrix();
	glPushMatrix();

	vpered();
	cubUp();
	povorot();
	nazad();
	CubDown();
	Right();


	glPushMatrix();
	cub(1);
	glPopMatrix();
		glPushMatrix();
		cub(2);
		glPopMatrix();
			glPushMatrix();
			cub(3);
			glPopMatrix();
				glPushMatrix();
				cub(4);
				glPopMatrix();
					glPushMatrix();
					cub(5);
					glPopMatrix();
						glPushMatrix();
						cub(6);
						glPopMatrix();
							glPushMatrix();
							cub(7);
							glPopMatrix();

	glTranslatef(stop=-20+aa, -1, stopp=65+bb);
	glRotated(90+angelxx,0,1,0);
	glScalef(1.9,1.9,1.9);
	Model2();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	CalculateFrameRate();
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);// Устанавливаем цвет в белый. Это обязательно, т.к. FTGL необходим инициализированный цвет.
	wchar_t buf[256];
	swprintf(buf, sizeof(buf), L"Текущие FPS: %f", FPS);
	Font->Print(10, 20, buf);  // Выводим текст'
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisableClientState(GL_VERTEX_ARRAY);
	SwapBuffers(g_hDC);
}

//Инициализирует окно

void Init(HWND hWnd)
{
	g_hWnd = hWnd;
	GetClientRect(g_hWnd, &g_rRect);
	InitializeOpenGL(g_rRect.right, g_rRect.bottom);
	glEnable(GL_COLOR_MATERIAL);
	Font = new CFont("arial.ttf", 16, 16);
	FPS = 0;
	g_Camera.PositionCamera(60, 4, 60, -10, -0.5f, 20, 0, 1, 0);//60, 4, 60
	srand(GetTickCount());
	// Инициализируем класс
	Texture = new CTexture();
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

	// давайте убедимся, что на машине присутствует нужная версия OpenGL.
	// Если функция расширения не найдена, указатели функций будут равны NULL
	if (!glActiveTextureARB || !glMultiTexCoord2fARB)
	{
		// Print a error message and quit.
		MessageBox(NULL, "Your version of OpenGL does not support multitexturing", "Error", MB_OK);
		PostQuitMessage(0);
	}

	LoadModel1();
	LoadModel2();
	// текстуры. Используем класс CTexture для их загрузки.
	Texture->LoadTexture(IL_BMP, "Wall.bmp", &textures[0]);
	Texture->LoadTexture(IL_BMP, "floor2.bmp", &textures[1]);
	Texture->LoadTexture(IL_BMP, "cub.bmp", &textures[2]);
	Texture->LoadTexture(IL_BMP, "sky.bmp", &textures[3]);
	Texture->LoadTexture(IL_BMP, "trot.bmp", &textures[4]);
	Texture->LoadTexture(IL_BMP, "polos.bmp", &textures[5]);
}

WPARAM MainLoop()
{
	MSG msg;

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//Проверяем мышь и клавиши каждый кадр
			g_Camera.SetViewByMouse();	//передвинуть мышью
			g_Camera.Update();
			RenderScene();
		}
	}
	for (int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// Очищаем структуры
		delete[] g_3DModel.pObject[i].pFaces;
		delete[] g_3DModel.pObject[i].pNormals;
		delete[] g_3DModel.pObject[i].pVerts;
		delete[] g_3DModel.pObject[i].pTexVerts;
	}
	for (int i = 0; i < skelmod.numOfObjects; i++)
	{
		delete[] skelmod.pObject[i].pFaces;
		delete[] skelmod.pObject[i].pNormals;
		delete[] skelmod.pObject[i].pVerts;
		delete[] skelmod.pObject[i].pTexVerts;

	}
	DeInit();

	return(msg.wParam);
}
LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG    lRet = 0;
	PAINTSTRUCT    ps;

	switch (uMsg)
	{
	case WM_SIZE:		// Если изменён размер окна

		SizeOpenGLScreen(LOWORD(lParam), HIWORD(lParam));// LoWord=Width, HiWord=Height
		GetClientRect(hWnd, &g_rRect);					// получаем window rectangle
		break;

	case WM_PAINT:			// Если нужно перерисовать сцену
		BeginPaint(hWnd, &ps);	// Иниц. paint struct
		EndPaint(hWnd, &ps);	// EndPaint, подчищаем
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);						// Выходим
			break;
		case VK_UP:
			angelx++; break;
		case VK_DOWN:
			angelx--; break;
		case VK_LEFT:
			angelz++; break;
		case VK_RIGHT: 
			angelz--; break;
		case VK_NUMPAD1:   
			g_LightPosition[1] += 0.1f;  // Увеличиваем значение Y
			// Убедимся, что не превысили 5
			if (g_LightPosition[1] > 5) g_LightPosition[1] = 5;
			break;

		case VK_NUMPAD2:
			g_LightPosition[1] -= 0.1f;// Уменьшим значение Y
			// Убедиммся, что оно не меньше -5
			if (g_LightPosition[1] < -5) g_LightPosition[1] = -5;
			break;
		}
		break;

	case WM_CLOSE:		// Если окно было закрыто
		PostQuitMessage(0);	// Выходим
		break;
	default:		// Return по умолчанию
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}
