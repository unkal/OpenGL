#include "3ds.h"
#include "main.h"
//�������
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
//����
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
//����� ��������
CTexture *Texture;
TextureImage textures[100];
//����� ������
CLoad3DS g_Load3ds, skel;
t3DModel g_3DModel, skelmod;
int   g_ViewMode = GL_TRIANGLES;
char* FILE_NAME = "Model\\skelet.3ds";
void LoadModel1()
{
	// ������� ��������� ��� .3ds ����. ������� ��������� �� ��������� � ��� �����.
	g_Load3ds.Import3DS(&g_3DModel, "Model\\model1.3ds");
	// �������� ����� ��� ���������
	for (int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// ���������, ���� �� � ����������� ��������� ��� �����
		if (strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{
			// ���������� ��� ����� ��� �������� ������� � ���������� ID (i).
			Texture->LoadTexture(IL_BMP, g_3DModel.pMaterials[i].strFile, &textures[i]);
		}
		// ������������� ID �������� ��� ����� ���������
		g_3DModel.pMaterials[i].texureId = i;
	}	
}
void Model1()
{
	// ��� ��� �� ����� ����� �������� � ����� ������, �������� ����� ������ �� ���
	for (int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// ��������, ��� ������� ������ ������
		if (g_3DModel.pObject.size() <= 0) break;
		// ������� ������� ������
		t3DObject *pObject = &g_3DModel.pObject[i];
		// ��������, ����� �� ������ ��������� �����, ���� �� - ������ �� ���� ��������
		if (pObject->bHasTexture) {
			// �������� ��������
			glEnable(GL_TEXTURE_2D);
			// ���������� ����
			glColor3ub(255, 255, 255);
			// ������ ���������� ����� �� ������ �� ��� materialID
			glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].texID);
		}
		else {
			// ����� �������� ��������
			glDisable(GL_TEXTURE_2D);
			// � ������� ���� �� ����������
			glColor3ub(255, 255, 255);
		}
		// �������� ��������� � ��������� ������
		glBegin(g_ViewMode);    // ������ ������� (������������ ��� �����)

		// �������� ����� ��� �������� ������� � ������ ��
		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			// �������� ����� ������ ���� ������������ � ������ ���
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				// �������� ������ ��� ������ ����� ��������
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// ������� OpenGL ������� ���� �������
				glNormal3f(pObject->pNormals[index].x,
					pObject->pNormals[index].y, pObject->pNormals[index].z);

				// ���� ������ ����� ��������, �������� �����. ����������
				if (pObject->bHasTexture) {

					// ��������, ��� UVW-��� ��������� �� ������, �����
					// �� �� ����� ����� ���������� ���������
					if (pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);
					}
				}
				else {

					// ��������, ��� � ��� ���� ������ ��������/����, �����������
					// � �������. ������ ����������� ������ � ������� �������� ��� 
					// ������� ����, �� ������ �� ������ ������ �������� ���.
					// ���� ������ ��������� ������� 1, � materialID != -1, 
					// �������� �����.
					if (g_3DModel.pMaterials.size() && pObject->materialID >= 0)
					{
						// �������� � ������������� ���� �������, ���� ��
						// �� ����� ��������
						BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;

						// ��������� ���� � ������
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				// ������� ������� ������� �������
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
	// ������� ��������� ��� .3ds ����. ������� ��������� �� ��������� � ��� �����.
	g_Load3ds.Import3DS(&skelmod, "Model\\skelet.3ds");
	// �������� ����� ��� ���������
	for (int i = 0; i < skelmod.numOfMaterials; i++)
	{
		// ���������, ���� �� � ����������� ��������� ��� �����
		if (strlen(skelmod.pMaterials[i].strFile) > 0)
		{
			// ���������� ��� ����� ��� �������� ������� � ���������� ID (i).
			Texture->LoadTexture(IL_BMP, skelmod.pMaterials[i].strFile, &textures[i]);
		}
		// ������������� ID �������� ��� ����� ���������
		skelmod.pMaterials[i].texureId = i;
	}
}
void Model2()
{
	// ��� ��� �� ����� ����� �������� � ����� ������, �������� ����� ������ �� ���
	for (int i = 0; i < skelmod.numOfObjects; i++)
	{
		// ��������, ��� ������� ������ ������
		if (skelmod.pObject.size() <= 0) break;
		// ������� ������� ������
		t3DObject *pObject = &skelmod.pObject[i];
		// ��������, ����� �� ������ ��������� �����, ���� �� - ������ �� ���� ��������
		if (pObject->bHasTexture) {
			// �������� ��������
			glEnable(GL_TEXTURE_2D);
			// ���������� ����
			glColor3ub(255, 255, 255);
			// ������ ���������� ����� �� ������ �� ��� materialID
			glBindTexture(GL_TEXTURE_2D, textures[pObject->materialID].texID);
		}
		else {
			// ����� �������� ��������
			glDisable(GL_TEXTURE_2D);
			// � ������� ���� �� ����������
			glColor3ub(255, 255, 255);
		}
		// �������� ��������� � ��������� ������
		glBegin(g_ViewMode);    // ������ ������� (������������ ��� �����)

		// �������� ����� ��� �������� ������� � ������ ��
		for (int j = 0; j < pObject->numOfFaces; j++)
		{
			// �������� ����� ������ ���� ������������ � ������ ���
			for (int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// Get the index for each point of the face
				// �������� ������ ��� ������ ����� ��������
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// ������� OpenGL ������� ���� �������
				glNormal3f(pObject->pNormals[index].x,
					pObject->pNormals[index].y, pObject->pNormals[index].z);

				// ���� ������ ����� ��������, �������� �����. ����������
				if (pObject->bHasTexture) {

					// ��������, ��� UVW-��� ��������� �� ������, �����
					// �� �� ����� ����� ���������� ���������
					if (pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[index].x, pObject->pTexVerts[index].y);
					}
				}
				else {

					// ��������, ��� � ��� ���� ������ ��������/����, �����������
					// � �������. ������ ����������� ������ � ������� �������� ��� 
					// ������� ����, �� ������ �� ������ ������ �������� ���.
					// ���� ������ ��������� ������� 1, � materialID != -1, 
					// �������� �����.
					if (skelmod.pMaterials.size() && pObject->materialID >= 0)
					{
						// �������� � ������������� ���� �������, ���� ��
						// �� ����� ��������
						BYTE *pColor = skelmod.pMaterials[pObject->materialID].color;

						// ��������� ���� � ������
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				// ������� ������� ������� �������
				glVertex3f(pObject->pVerts[index].x, pObject->pVerts[index].y,
					pObject->pVerts[index].z);
			}
		}
		glEnd();
	}
}