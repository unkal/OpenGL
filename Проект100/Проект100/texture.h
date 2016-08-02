#ifndef __TEXTURE_H
#define __TEXTURE_H

#include "main.h"
// ������ ���������� DevIL
#include "il\il.h"
#include "il\ilu.h"

// �������� ��������� TextureImage ��� �������� ������ ��������.
typedef struct TextureImage
{
	GLubyte *imageData;     // ������ ��������
	GLuint  bpp;            // ���� �� ������
	GLuint width, height;        // ������ � ������
	GLuint texID;           // ID ��������
}TextureImage;


// ����� �������. �������� � �������������.
class CTexture{
public:

	CTexture();
	~CTexture();

	// ����������, ����� ��� ������ ������� - � �� ������ 2 ������� - 
	// �������� � ������������ ������� =)
	void LoadTexture(ILenum FileType, char *filename, TextureImage *texture);
	void FreeTexture(TextureImage *texture);
};


#endif