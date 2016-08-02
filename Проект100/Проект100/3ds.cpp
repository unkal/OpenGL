#include "main.h"
#include "3ds.h"

int gBuffer[50000] = { 0 };   // ������������ ��� ������ ������������� ������
/////   ����������� �������������� ������ tChunk
/////
///////////////////////////////// CLOAD3DS \\\\\\\\\\\\\\\\*

CLoad3DS::CLoad3DS()
{
	m_FilePointer = NULL;
}

/////   ���������� �������� ��� ��������, ������ � ����� ������� .3ds
/////
///////////////////////////////// IMPORT 3DS \\\\\\\\\\\\\\\\*

bool CLoad3DS::Import3DS(t3DModel *pModel, char *strFileName)
{
	char strMessage[255] = { 0 };
	tChunk currentChunk = { 0 };

	// ��������� .3ds ����
	m_FilePointer = fopen(strFileName, "rb");
	
	// ��������, ��� ��������� �� ���� ����� (�� ������� ����)
	if (!m_FilePointer)
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// ������ ����, ����� ��������� ����� �����, ����� ��������, ��� ��� 3DS.
	// ���� ��� ������ ����, �� ������ ID chunk-� ����� PRIMARY

	// ������ ������ chunk �����, ����� ��������, ��� ��� 3DS
	ReadChunk(&currentChunk);

	// ��������, ��� ��� 3DS
	if (currentChunk.ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, strMessage, "Error", MB_OK);
		return false;
	}

	// ������ �������� ������ ������. ProcessNextChunk() - ����������� �������

	// �������� �������� �������� ������� ����������� �������
	ProcessNextChunk(pModel, &currentChunk);

	// ����� ��������� ����� ����� ��� ����� ���������� ������� ������
	ComputeNormals(pModel);

	// � ����� ��������� ��
	CleanUp();

	return true;
}

///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ��� ������� ������ � ��������� ����
/////
///////////////////////////////// CLEAN UP \\\\\\\\\\\\\\\\*

void CLoad3DS::CleanUp()
{
	if (m_FilePointer) {
		fclose(m_FilePointer);  // ��������� ����
		m_FilePointer = NULL;
	}
}


///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ������� ������ �����, ����� ���������� ��� ������
/////
///////////////////////////////// PROCESS NEXT CHUNK\\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	t3DObject newObject = { 0 };      // ������������ ��� ���������� ������� � ������
	tMaterialInfo newTexture = { 0 };     // ������������ ��� ���������� ���������

	tChunk currentChunk = { 0 };      // ������� chunk ��� ��������
	tChunk tempChunk = { 0 };         // ��������� chunk ��� �������� ������

	// ���� ��������� ID chunk-a ������ ��� ��� ������ ������. �����,
	// ���� ����� �������� ������ �� chunk-�, ������ ���. ���� �� ���� chunk ���
	// �� �����, ������ ������ chunk � "��������" ������.

	// ���������� ������ ���������, ���� �� ������ �� ����� ������ �����.
	// ����� ������ ���� ������, ����������� ����������� ����� � ����������
	// �� � ����� �������.
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� chunk
		ReadChunk(&currentChunk);

		// �������� chunk ID
		switch (currentChunk.ID)
		{
		case VERSION:           // ������ �����

			// ������ ������ ����� � ��������� ����������� ����� � ���������� bytesRead
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length -
				currentChunk.bytesRead, m_FilePointer);

			// ���� ������ ����� ������ 3, ������� ��������������, ��� �����
			// ���������� ��������.
			if ((currentChunk.length - currentChunk.bytesRead == 4) && (gBuffer[0] > 0x03)) {
				MessageBox(NULL, "This 3DS file is over version 3 so it may load incorrectly",
					"Warning", MB_OK);
			}
			break;

		case OBJECTINFO:        // �������� ������ ����
		{
			// ���� chunk �������� ������ ����. ����� ��� ��������� ��� chunk-�� MATERIAL
			// � OBJECT. ������ �� �������� ������ ���������� ���������� � ��������.

			// ������ ��������� chunk
			ReadChunk(&tempChunk);

			// �������� ������ ����
			tempChunk.bytesRead += fread(gBuffer, 1, tempChunk.length -
				tempChunk.bytesRead, m_FilePointer);

			// ����������� bytesRead �� ����� ����������� ����
			currentChunk.bytesRead += tempChunk.bytesRead;

			// ��������� � ���������� chunk-�, ��� ����� MATERIAL, ����� OBJECT
			ProcessNextChunk(pModel, &currentChunk);
			break;
		}
		case MATERIAL:          // �������� ���������� � ���������

			// ���� chunk - ����� ��� ���������� � ���������

			// ����������� ����� ����������
			pModel->numOfMaterials++;


			// ��������� ������ ��������� �������� � ��� ������ �������.
			pModel->pMaterials.push_back(newTexture);

			// �������� �������, �������������� ��������
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case OBJECT:            // ������ ��� ��������� �������

			// ���� chunk - ����� ��� chunk-��, �������� ���������� �������.
			// ����� �� ������ ��� �������.

			// ����������� ������� ��������
			pModel->numOfObjects++;

			// ��������� ����� ������� tObject � ������ ��������
			pModel->pObject.push_back(newObject);

			// �������������� ������ � ��� ��� ������
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			// �������� � ��������� ��� �������, ����� ����������� ������� ����������� ����
			currentChunk.bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);

			// ��������� � ������ ���������� ���������� �������
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), &currentChunk);
			break;

		case EDITKEYFRAME:

			// ��� ��� � ����� ������� ������� ����, ��������� ��� ��������, � �� �������
			// ���������� � ���������� ��������. ���� chunk - ����� ��� ���� ����������
			// �� ��������. � ������� ������ ���� ������ ����� �������� ������.

			//ProcessNextKeyFrameChunk(pModel, currentChunk);

			// ������ � "��������" ��������� �������� ������ � ����������� �������
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length -
				currentChunk.bytesRead, m_FilePointer);
			break;

		default:

			// ��������� chunk-�, ������� ��� �� �����, ����� ���������� �����. ���
			// �� ��� ����� ��������� � "��������" ���������� ����������� ��� ������������
			// chunk-� � ��������� ������� ����������� ����.
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length -
				currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// �������� ����������� ����� ���������� chunk-� � ��������
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}


///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\*
/////
/////   ������� ��������� ��� ���������� �� �������
/////
///////////////////////////////// PROCESS NEXT OBJECT CHUNK \\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ������� chunk, � ������� ��������
	tChunk currentChunk = { 0 };

	// ���������� ������ ��� chunk-�, ���� �� ����� �� ����� ���� ������
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� ������
		ReadChunk(&currentChunk);

		// ���������, ��� ��� �� ������
		switch (currentChunk.ID)
		{
		case OBJECT_MESH:       // ��� ��� �����, ��� �� ������ ����� ������

			// ����� ����� ������, ��������� ��� ���������� ���������
			ProcessNextObjectChunk(pModel, pObject, &currentChunk);
			break;

		case OBJECT_VERTICES:       // ������� ������ �������
			ReadVertices(pObject, &currentChunk);
			break;

		case OBJECT_FACES:      // �������� �������
			ReadVertexIndices(pObject, &currentChunk);
			break;

		case OBJECT_MATERIAL:       // ��� ��������� �������

			// ��� ������ ������ ��� ���������, ���������� � ������� ��������. ��� ����� ����
			// ���� ��� ���������� �����. ��� ������ ����� �������� ��������, � �������
			// ��������� �������� (���� �������� �� ������� ��������� �������, ��� ������
			// �������� �������� ������ �� ����� �������). ������ � ��� ����� ������ ����
			// �������� �� ���� ������, ��� ��� ������� ������ ID ���������.

			// ������ �� ������ ��� ���������, ������������ � �������
			ReadObjectMaterial(pModel, pObject, &currentChunk);
			break;

		case OBJECT_UV:     // ������ ���������� ���������� �������

			// ��� ������ �������� ��� UV-���������� �������. ��������� ��.
			ReadUVCoordinates(pObject, &currentChunk);
			break;

		default:

			// Read past the ignored or unknown chunks
			// ������ ������������/����������� ������ � "��������" ������
			currentChunk.bytesRead += fread(gBuffer, 1, currentChunk.length
				- currentChunk.bytesRead, m_FilePointer);
			break;
		}

		// ���������� ����������� ������ � ��������
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}


///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\*
/////
/////   ��� ������� ������ ��� ���������� � ��������� (��������)
/////
///////////////////////////////// PROCESS NEXT MATERIAL CHUNK \\\\\\\\\\\\\\\\*

void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	// ������� chunk ��� ������
	tChunk currentChunk = { 0 };

	// ���������� ������ ��� chunk-�, ���� �� ����� �� ����� ���������
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// ������ ��������� ������
		ReadChunk(&currentChunk);

		// ���������, ��� ������ �� ���������
		switch (currentChunk.ID)
		{
		case MATNAME:       // ��� ������ ������ ��� ���������

			// ������ ��� ���������
			currentChunk.bytesRead +=
				fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName,
				1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:        // ������ RGB-���� �������
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), &currentChunk);
			break;

		case MATMAP:            // ��� ����� ���������� � ��������

			// ������ ���������� ���������� � ���������
			ProcessNextMaterialChunk(pModel, &currentChunk);
			break;

		case MATMAPFILE:        // ������ ��� ����� ���������

			// ������ ��� ����� ���������
			currentChunk.bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile,
				1, currentChunk.length - currentChunk.bytesRead, m_FilePointer);
			break;

		default:

			// ������ ��������� ������ � "�����"
			currentChunk.bytesRead += fread(gBuffer, 1,
				currentChunk.length - currentChunk.bytesRead,
				m_FilePointer);
			break;
		}

		// ���������� ����������� ������ � ��������
		pPreviousChunk->bytesRead += currentChunk.bytesRead;
	}
}

///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ID chunk-� � ��� ������ � ������
/////
///////////////////////////////// READ CHUNK \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	// ������� ������ ID ������ (2 �����).
	// ID chunk-� - ���, ��������, OBJECT/MATERIAL. ��� ������� ���,
	// ����� ������ ����� ���� ��������� � ���� ������.
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// ����� ������ ������ ������ (4 �����). ������ �� �����,
	// ������� ������ ��� ����� ����� ���������.
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\*
/////
/////   ������ ������ � ������ char-��
/////
///////////////////////////////// GET STRING \\\\\\\\\\\\\\\\*

int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	// ������ 1 ���� ������, ������ ����� ������
	fread(pBuffer, 1, 1, m_FilePointer);

	// ����, ���� �� �������� NULL
	while (*(pBuffer + index++) != 0) {

		// ������ ������� �� �����, ���� �� ������� NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// ����� ������ ������, �.�. ������� ������ �� ��������� (������� NULL)
	return strlen(pBuffer) + 1;
}


///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\*
/////
/////   ������ ������ RGB-�����
/////
///////////////////////////////// READ COLOR \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	tChunk tempChunk = { 0 };

	// ������ ���������� � �����
	ReadChunk(&tempChunk);

	// ������ RGB-���� (3 ����� - �� 0 �� 255)
	tempChunk.bytesRead += fread(pMaterial->color, 1, tempChunk.length - tempChunk.bytesRead, m_FilePointer);

	// ����������� �������
	pChunk->bytesRead += tempChunk.bytesRead;
}


///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ������� ��� ������� ������
/////
///////////////////////////////// READ VERTEX INDECES \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;       // ������������ ��� ������ ������� �������� ��������

	// ����� ��������� ������� ������ ��� �������, ����� ������� ��������� ��
	// �����, ����� ��� �� �����. ���������, ��� ����� ��������� ������ 3 ��
	// 4 �������� ��� ������� ��������. ��� ��������� �������� - ���� ���������
	// ��� 3DS Max, ������� ������ ��� ��� �� ������.

	// ������ ����� ��������� ����� �������
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// �������� ���������� ������ ��� ��������� � �������������� ���������
	pObject->pFaces = new tFace[pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	// �������� ����� ��� �������� ����� �������
	for (int i = 0; i < pObject->numOfFaces; i++)
	{
		// ����� ������ A-B-C ������� ��� ��������, �� ���������� 4-� ��������.
		for (int j = 0; j < 4; j++)
		{
			// ������ ������ ������ ������� ��� �������� ��������
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if (j < 3)
			{
				// ��������� ������ � ��������� ���������
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ UV-���������� �������
/////
///////////////////////////////// READ UV COORDINATES \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ����� ��������� ������� UV-��������� ��� �������, ������� �����
	// ��������� �� ������ ����������, ����� ��� �� �����.

	// ������ ����� UV-���������
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// �������� ������ ��� �������� UV-���������
	pObject->pTexVerts = new CVector2[pObject->numTexVertex];

	// ������ ���������� ���������� (������ �� 2� float)
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1,
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}


///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ������� �������
/////
///////////////////////////////// READ VERTICES \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// ��� � � ����������� chunk-��, ������ ��� ������ ���� �������,
	// ����� ����� �� ����������.

	// ������ ����� ������
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// �������� ������ ��� ������ � �������������� ���������
	pObject->pVerts = new CVector31[pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector31) * pObject->numOfVerts);

	// ������ � ������ ������ (������ �� 3 float)
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1,
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	// ������ ��� ������� ���������. ��� ��� � ������� 3DS Max ������ ����������
	// ���, ����� �������� Y-�������� � Z-�������� ����� ������.

	// �������� ����� ��� ������� � ������ y<->z
	for (int i = 0; i < pObject->numOfVerts; i++)
	{
		// ��������� ������ ����-� Y
		float fTempY = pObject->pVerts[i].y;

		// ������������� �������� Y � Z
		pObject->pVerts[i].y = pObject->pVerts[i].z;

		// ������������� �������� Z � Y
		// � ������ ��� �������������, �.�. � 3ds max Z-��� �����������
		pObject->pVerts[i].z = -fTempY;
	}
}


///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\*
/////
/////   ������� ������ ��� ���������, ����������� �� ������, � ������������� materialID
/////
///////////////////////////////// READ OBJECT MATERIAL \\\\\\\\\\\\\\\\*

void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = { 0 };        // ������ ��� ���������

	// *��� ����� ��������?* - �������� - ��� ���� + ���������� ����� �������.
	// ����� �� ������������ ������ ���������� ���� �������, "�����������" � �.�.
	// ������ ��� ����� ������ ���� ��� ��� ���������� �����.

	// ������ ��� ���������, ������������ � �������� �������.
	// strMaterial ������ ������ ��������� ������ � ������ ���������, ���� "Material #2" � �.�...
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// ������, ���� ��� ���������, ����� ������ ����� ��� ��������� � ��������� ��
	// ����� �� ���������� � �����. ����� ������ �������� � ������ ��� ����������� 
	// ������, ����������� materialID ������� � ������� ����� ���������. 

	// �������� ����� ��� ���������
	for (int i = 0; i < pModel->numOfMaterials; i++)
	{
		// ���� ������ ��� ����������� �������� ��������� � ������ �������
		if (strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			// ���������, ���� �� ���������� �����. ���� strFile ��������
			// ������ � ������� >=1, �������� ����.
			if (strlen(pModel->pMaterials[i].strFile) > 0)
			{
				// ������������� ID ��������� � ������� ������ 'i' � ����������� ��������
				pObject->materialID = i;

				// ������������� ���� ��������������� � true
				pObject->bHasTexture = true;
			}
			break;
		}
		else
		{
			// ��������� ����, ����� �������, ���� �� ��� �������� �� ���� �������
			if (pObject->bHasTexture != true)
			{
				// ������������� ID ��������� � -1, ����� �������, ��� ��������� ��� ������� ���
				pObject->materialID = -1;
			}
		}
	}

	// ��������� ������ � "�����"
	pPreviousChunk->bytesRead += fread(gBuffer, 1,
		pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

// *Note*
//
// ���� ���� ��������� �������������� �������, ����������� ������� ������. ��� ���
// �����, ����� ������ ��������� ������������� �����. � ������� ������ �� ��� ������ 
// ��� �������, ��� ��� ��� ������� ����� ������ ���������� ����� 3dmath.h/.cpp

//////////////////////////////  Math Functions  ////////////////////////////////*

// ������������ �������� ������� (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// ������������ ������� ����� 2 ������� � ���������� ���������
CVector31 Vector(CVector31 vPoint1, CVector31 vPoint2)
{
	CVector31 vVector;           // ������ �������������� ������

	vVector.x = vPoint1.x - vPoint2.x;
	vVector.y = vPoint1.y - vPoint2.y;
	vVector.z = vPoint1.z - vPoint2.z;

	return vVector;             // ����� �������������� ������
}

// This adds 2 vectors together and returns the result
// ���������� 2 ������� � ���������� ���������
CVector31 AddVector(CVector31 vVector1, CVector31 vVector2)
{
	CVector31 vResult;               // ������ �������������� ������

	vResult.x = vVector2.x + vVector1.x;
	vResult.y = vVector2.y + vVector1.y;
	vResult.z = vVector2.z + vVector1.z;

	return vResult;                 // ����� ���������
}

// ����� ������ �� ���������� ����� � ���������� ���������
CVector31 DivideVectorByScaler(CVector31 vVector1, float Scaler)
{
	CVector31 vResult;

	vResult.x = vVector1.x / Scaler;
	vResult.y = vVector1.y / Scaler;
	vResult.z = vVector1.z / Scaler;

	return vResult;
}

// ���������� ��������� ������������ (dot product) ���� ��������
CVector31 Cross(CVector31 vVector1, CVector31 vVector2)
{
	CVector31 vCross;

	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;
}

// ���������� ������� �������
CVector31 Normalize(CVector31 vNormal)
{
	double Magnitude;

	Magnitude = Mag(vNormal);

	vNormal.x /= (float)Magnitude;
	vNormal.y /= (float)Magnitude;
	vNormal.z /= (float)Magnitude;

	return vNormal;
}

///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\*
/////
/////   ������� ������������ ������� ��� ������� � ��� ������
/////
///////////////////////////////// COMPUTER NORMALS \\\\\\\\\\\\\\\\*

void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector31 vVector1, vVector2, vNormal, vPoly[3];

	// ���� �������� ���, ���������� ���� ���
	if (pModel->numOfObjects <= 0)
		return;

	// ��� ����� ������� ������? ��� ��� ���������� �� ��������� ��������? ���� ��
	// ����� ������� ������������, ��� "������� ��������". ���� �� �������� OpenGL
	// ������� �������� ��� ���������, ��� ������ ����� ��������� ������� � ������.
	// ���� �� �� ������� ������� ��� ������ �������, ���������� ������ �����
	// ��������� ����������, �.�. ����� ������������.

	// �������� ����� ��� ������� ��� ���������� �� ������
	for (int index = 0; index < pModel->numOfObjects; index++)
	{
		// ������� ������� ������
		t3DObject *pObject = &(pModel->pObject[index]);

		// �������� ������ ��� ������ ����������
		CVector31 *pNormals = new CVector31[pObject->numOfFaces];
		CVector31 *pTempNormals = new CVector31[pObject->numOfFaces];
		pObject->pNormals = new CVector31[pObject->numOfVerts];

		// �������� ����� ��� �������� �������
		for (int i = 0; i < pObject->numOfFaces; i++)
		{
			// ��������� 3 ����� ����� ��������, ����� �������� �������� ����
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// ������ �������� ������� ���������

			vVector1 = Vector(vPoly[0], vPoly[2]);  // ������ �������� (�� 2� ��� ������)
			vVector2 = Vector(vPoly[2], vPoly[1]);  // ������ ������ ��������

			vNormal = Cross(vVector1, vVector2);   // �������� cross product ��������
			pTempNormals[i] = vNormal;      // �������� ��������� ��-����������������� �������
			// ��� ������
			vNormal = Normalize(vNormal);      // ������������� cross product ��� �������� ��������

			pNormals[i] = vNormal;          // ��������� ������� � ������
		}

		//////////////// ������ �������� ��������� ������� /////////////////

		CVector31 vSum = { 0.0, 0.0, 0.0 };
		CVector31 vZero = vSum;
		int shared = 0;

		for (int i = 0; i < pObject->numOfVerts; i++)   // �������� ����� ��� �������
		{
			for (int j = 0; j < pObject->numOfFaces; j++)   // �������� ����� ��� ������������
			{               // ���������, ������������ �� ������� ������ ���������
				if (pObject->pFaces[j].vertIndex[0] == i ||
					pObject->pFaces[j].vertIndex[1] == i ||
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);    // ���������� ��-
					// ����������������� ������� ������� ��������
					shared++;       // ����������� ����� ��������� � ������ ���������
				}
			}

			// �������� ������� �������� �� ����� ����� ���������. ������ � �������.
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// ����������� ������� ��� �������
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);

			vSum = vZero;           // ���������� �����
			shared = 0;         // � ����� ��������
		}

		// ����������� ������ ��������� ����������
		delete[] pTempNormals;
		delete[] pNormals;
	}
}