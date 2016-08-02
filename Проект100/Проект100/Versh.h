#include "3ds.h"
#include "main.h"
//комната
int stena = 80;
int room[] = { -stena, stena / 2, -stena, -stena, 0, -stena, stena, 0, -stena,
stena, stena / 2, -stena, -stena, stena / 2, stena, -stena, 0, stena,
stena, 0, stena, stena, stena / 2, stena, -stena, stena / 2, -stena,
-stena, 0, -stena, -stena, 0, stena, -stena, stena / 2, stena,
stena, stena / 2, -stena, stena, 0, -stena, stena, 0, stena, stena, stena / 2, stena };
int romtext[] = { 0.0f, 20.0f,
0.0f, 0.0f,
120.0f, 0.0f,
120.0f, 20.0f,
0.0f, 20.0f,
0.0f, 0.0f,
120.0f, 0.0f,
120.0f, 20.0f,
0.0f, 20.0f,
0.0f, 0.0f,
120.0f, 0.0f,
120.0f, 20.0f,
0.0f, 20.0f,
0.0f, 0.0f,
120.0f, 0.0f,
120.0f, 20.0f };
int roomflor[] = { -stena, 0, -stena, -stena, 0, stena, stena, 0, stena, stena, 0, -stena };
//elemtrot
float trotflor[] = {stena/3,0.01,-stena,stena,0.01,-stena,stena,0.01,stena,stena/3,0.01,stena};
float trotflor1[] = { -stena / 3, 0.01, -stena, -stena/3, 0.01, stena, -stena, 0.01, stena, -stena, 0.01, -stena };
float polos[] = { stena / 66, 0.01, -stena, stena / 66, 0.01, stena, -stena / 66, 0.01, stena, -stena / 66, 0.01, -stena };
int floortext[] = { 0.0f, 16.0f, 0.0f, 0.0f, 16.0f, 0.0f, 16.0f, 16.0f };
int cordsky[] = { 0, 1 };
int sky[] = {-stena,stena/2,-stena,-stena,stena/2,stena,stena,stena/2,stena,stena,stena/2,-stena};
//ящик
int cubx = 1;
int corcub[] = { 0, 0, 1, 0, 1, 1, 0, 1,
0, 0, 1, 0, 1, 1, 0, 1,
0, 0, 1, 0, 1, 1, 0, 1,
0, 0, 1, 0, 1, 1, 0, 1,
0, 0, 1, 0, 1, 1, 0, 1,
0, 0, 1, 0, 1, 1, 0, 1 };
int cubik[] = { cubx, -cubx, -cubx, cubx, cubx, -cubx, -cubx, cubx, -cubx, -cubx, -cubx, -cubx,
cubx, -cubx, cubx, cubx, cubx, cubx, -cubx, cubx, cubx, -cubx, -cubx, cubx,
cubx, -cubx, -cubx, cubx, cubx, -cubx, cubx, cubx, cubx, cubx, -cubx, cubx,
-cubx, -cubx, -cubx, -cubx, cubx, -cubx, -cubx, cubx, cubx, -cubx, -cubx, cubx,
cubx, cubx, -cubx, cubx, cubx, cubx, -cubx, cubx, cubx, -cubx, cubx, -cubx,
cubx, -cubx, -cubx, cubx, -cubx, cubx, -cubx, -cubx, cubx, -cubx, -cubx, -cubx };
//класс текстуры
CTexture *Texture;
TextureImage textures[100];
//класс модели
CLoad3DS g_Load3ds, skel;
t3DModel g_3DModel, skelmod;
int   g_ViewMode = GL_TRIANGLES;
char* FILE_NAME = "Model\\skelet.3ds";
void LoadModel1()
{
	// Сначала загружаем наш .3ds файл. Передаём указатель на структуру и имя файла.
	g_Load3ds.Import3DS(&g_3DModel, "Model\\model1.3ds");
	// Проходим через все материалы
	for (int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// Проверяем, есть ли в загруженном материале имя файла
		if (strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{
			// Используем имя файла для загрузки битмапа с текстурным ID (i).
			Texture->LoadTexture(IL_BMP, g_3DModel.pMaterials[i].strFile, &textures[i]);
		}
		// Устанавливаем ID текстуры для этого материала
		g_3DModel.pMaterials[i].texureId = i;
	}	
}
void Model1()
{
	// Так как мы знаем число обьектов в нашей модели, проходим через каждый из них
	for (int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// Убедимся, что передан верный обьект
		if (g_3DModel.pObject.size() <= 0) break;
		// Получим текущий обьект
		t3DObject *pObject = &g_3DModel.pObject[i];
		// Проверим, имеет ли обьект тексурную карту, если да - биндим на него текстуру
		if (pObject->bHasTexture) {
			// Включаем текстуры
			glEnable(GL_TEXTURE_2D);
			// Сбрасываем цвет
			glColor3ub(255, 255, 255);
			// Биндим текстурную карту на обьект по его materialID
			glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].texID);
		}
		else {
			// Иначе выключим текстуры
			glDisable(GL_TEXTURE_2D);
			// И сбросим цвет на нормальный
			glColor3ub(255, 255, 255);
		}
		// Начинаем отрисовку в выбранном режиме
		glBegin(g_ViewMode);    // Рисуем обьекты (треугольники или линии)

		// Проходим через все полигоны обьекта и рисуем их
		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			// Проходим через каждый угол треугольника и рисуем его
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				// Получаем индекс для каждой точки полигона
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// Передаём OpenGL нормаль этой вершины
				glNormal3f(pObject->pNormals[index].x,
					pObject->pNormals[index].y, pObject->pNormals[index].z);

				// Если обьект имеет текстуру, передаем текст. координаты
				if (pObject->bHasTexture) {

					// Убедимся, что UVW-мап применена на обьект, иначе
					// он не будет иметь текстурных координат
					if (pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);
					}
				}
				else {

					// Убедимся, что у нас есть верный материал/цвет, привязанный
					// к обьекту. Вообще практически всегда к обьекту привязан как 
					// минимум цвет, но просто на всякий случай проверим это.
					// Если размер материала минимум 1, и materialID != -1, 
					// материал верен.
					if (g_3DModel.pMaterials.size() && pObject->materialID >= 0)
					{
						// Получаем и устанавливаем цвет обьекта, если он
						// не имеет текстуры
						BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;

						// Применяем цвет к модели
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				// Передаём текущую вершину обьекта
				glVertex3f(pObject->pVerts[index].x, pObject->pVerts[index].y,
					pObject->pVerts[index].z);
			}
		}
		glEnd();
	}
}

///skel
void LoadModel2()
{
	// Сначала загружаем наш .3ds файл. Передаём указатель на структуру и имя файла.
	g_Load3ds.Import3DS(&skelmod, "Model\\skelet.3ds");
	// Проходим через все материалы
	for (int i = 0; i < skelmod.numOfMaterials; i++)
	{
		// Проверяем, есть ли в загруженном материале имя файла
		if (strlen(skelmod.pMaterials[i].strFile) > 0)
		{
			// Используем имя файла для загрузки битмапа с текстурным ID (i).
			Texture->LoadTexture(IL_BMP, skelmod.pMaterials[i].strFile, &textures[i]);
		}
		// Устанавливаем ID текстуры для этого материала
		skelmod.pMaterials[i].texureId = i;
	}
}
void Model2()
{
	// Так как мы знаем число обьектов в нашей модели, проходим через каждый из них
	for (int i = 0; i < skelmod.numOfObjects; i++)
	{
		// Убедимся, что передан верный обьект
		if (skelmod.pObject.size() <= 0) break;
		// Получим текущий обьект
		t3DObject *pObject = &skelmod.pObject[i];
		// Проверим, имеет ли обьект тексурную карту, если да - биндим на него текстуру
		if (pObject->bHasTexture) {
			// Включаем текстуры
			glEnable(GL_TEXTURE_2D);
			// Сбрасываем цвет
			glColor3ub(255, 255, 255);
			// Биндим текстурную карту на обьект по его materialID
			glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].texID);
		}
		else {
			// Иначе выключим текстуры
			glDisable(GL_TEXTURE_2D);
			// И сбросим цвет на нормальный
			glColor3ub(255, 255, 255);
		}
		// Начинаем отрисовку в выбранном режиме
		glBegin(g_ViewMode);    // Рисуем обьекты (треугольники или линии)

		// Проходим через все полигоны обьекта и рисуем их
		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			// Проходим через каждый угол треугольника и рисуем его
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				// Получаем индекс для каждой точки полигона
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// Передаём OpenGL нормаль этой вершины
				glNormal3f(pObject->pNormals[index].x,
					pObject->pNormals[index].y, pObject->pNormals[index].z);

				// Если обьект имеет текстуру, передаем текст. координаты
				if (pObject->bHasTexture) {

					// Убедимся, что UVW-мап применена на обьект, иначе
					// он не будет иметь текстурных координат
					if (pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);
					}
				}
				else {

					// Убедимся, что у нас есть верный материал/цвет, привязанный
					// к обьекту. Вообще практически всегда к обьекту привязан как 
					// минимум цвет, но просто на всякий случай проверим это.
					// Если размер материала минимум 1, и materialID != -1, 
					// материал верен.
					if (skelmod.pMaterials.size() && pObject->materialID >= 0)
					{
						// Получаем и устанавливаем цвет обьекта, если он
						// не имеет текстуры
						BYTE *pColor = skelmod.pMaterials[pObject->materialID].color;

						// Применяем цвет к модели
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				// Передаём текущую вершину обьекта
				glVertex3f(pObject->pVerts[index].x, pObject->pVerts[index].y,
					pObject->pVerts[index].z);
			}
		}
		glEnd();
	}
}