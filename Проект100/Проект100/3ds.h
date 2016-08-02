#ifndef _3DS_H
#define _3DS_H

//>------ ������� Chunk, � ������ ������� 3ds-�����
#define PRIMARY       0x4D4D

//>------ ������� Chunk-�
#define OBJECTINFO    0x3D3D            // ��� ������������� ������ ���� ����� ����������� �� �������
#define VERSION       0x0002            // ������������� ������ .3ds �����
#define EDITKEYFRAME  0xB000            // ����� ��� ���� ���������� � ������

//>------ ���-������� OBJECTINFO
#define MATERIAL      0xAFFF        // ���������� � ���������
#define OBJECT        0x4000        // ��������, �������, � �.�...

//>------ ���-������� ��� MATERIAL
#define MATNAME       0xA000            // �������� ���������
#define MATDIFFUSE    0xA020            // ������ ���� �������/���������
#define MATMAP        0xA200            // ����� ��� ������ ���������
#define MATMAPFILE    0xA300            // ������ ��� ����� ��������

#define OBJECT_MESH   0x4100            // ��� ��� �����, ��� �������� ��������� ����� ������

//>------ ���-������� ��� OBJECT_MESH
#define OBJECT_VERTICES     0x4110      // ������� �������
#define OBJECT_FACES        0x4120      // �������� �������
#define OBJECT_MATERIAL     0x4130      // ������ ���������, ���� ������ ����� ��������, ����� ����/��������
#define OBJECT_UV       0x4140      // UV ���������� ����������


// ��������� ��� �������� 3DS (��� ��� .3ds ������ 4 unsigned short)
struct tIndices {
	unsigned short a, b, c, bVisible;   // ��� ������ ������� ��� ����� 1,2,3 �������
	// ������, ���� ���� ���������
};

// ������ ���������� � chunk-�
struct tChunk
{
	unsigned short int ID;          // ID chunk-�
	unsigned int length;            // ������ chunk-�
	unsigned int bytesRead;         // ����� �������� ���� ��� ����� chunk-�
};

// ����� �������� ���� ��� ��������
class CLoad3DS
{
public:
	CLoad3DS();     // ��� ����� ����������������� ��� ������

	// ��� �-� � ����� ��������� ��� �������� 3DS
	bool Import3DS(t3DModel *pModel, char *strFileName);

private:
	// ������ ������ � ��������� � � ���������� ������ char-��
	int GetString(char *);

	// ������ ��������� chunk
	void ReadChunk(tChunk *);

	// ������ ��������� ������� chunk
	void ProcessNextChunk(t3DModel *pModel, tChunk *);

	// ������ chunk-� �������
	void ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *);

	// ������ chunk-� ���������
	void ProcessNextMaterialChunk(t3DModel *pModel, tChunk *);

	// ������ RGB-�������� ����� �������
	void ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk);

	// ������ ������� �������
	void ReadVertices(t3DObject *pObject, tChunk *);

	// ������ ���������� ��������� �������
	void ReadVertexIndices(t3DObject *pObject, tChunk *);

	// ������ ���������� ���������� �������
	void ReadUVCoordinates(t3DObject *pObject, tChunk *);

	// ������ ��� ���������, ������������ �������, � ������������� materialID
	void ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk);

	// ������������ ������� ������ �������
	void ComputeNormals(t3DModel *pModel);

	// This frees memory and closes the file
	// ����������� ������ � ��������� ����
	void CleanUp();

	// ��������� �� ����
	FILE *m_FilePointer;
};


#endif