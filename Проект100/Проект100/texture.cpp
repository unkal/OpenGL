#include "texture.h"

// �����������
CTexture::CTexture()
{
	// � ������������ �������������� ���������� IL � ILU.
	// ���� ����� �� �������, ��������� ����� ����������� � �������.
	ilInit();
	iluInit();
}

// ����������
CTexture::~CTexture()
{
}

// �������, ��������������� ����������� �������� � ���������� ���������.
// � ������� ���������� ��� �����, ��� ����� � ��������� �� ��������.
void CTexture::LoadTexture(ILenum FileType, char *filename, TextureImage *texture)
{
	// ������ DevIL ��������� ����
	ilLoad(FileType, filename);

	// ���������, ��� �� ������ ��� �������� �����
	int err = ilGetError();
	if (err != IL_NO_ERROR)
	{
		const char* strError = iluErrorString(err);
		MessageBox(NULL, strError, "������ ��� ��������!", MB_OK);
		exit(1);
	}

	// �������� ������ ��������
	texture->width = ilGetInteger(IL_IMAGE_WIDTH);          // ������
	texture->height = ilGetInteger(IL_IMAGE_HEIGHT);        // ������
	texture->bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);      // ���� �� �������

	// ��������� ������ � ���� ��������
	texture->imageData = ilGetData();

	ilEnable(IL_CONV_PAL);

	// ��� ������ �����������
	unsigned int type = ilGetInteger(IL_IMAGE_FORMAT);

	// ���������� ��������
	glGenTextures(1, &texture->texID);

	// ����������� ������ �������� � ID
	glBindTexture(GL_TEXTURE_2D, texture->texID);

	// ������ ���-����
	gluBuild2DMipmaps(GL_TEXTURE_2D, texture->bpp, texture->width,
		texture->height, type, GL_UNSIGNED_BYTE, texture->imageData);

	// ������������� �������� �������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void CTexture::FreeTexture(TextureImage *texture)
{

}