#ifndef __FONT_H
#define __FONT_H

#include "main.h"
#include <wchar.h>      // ����� ������������ � ��������� ������� ����, �� �����
// ������������ ������, ������� �������� � ���������� ���� wchar_t
// ������ FTGL:
#include <ftgl\FTFont.h>
#include <ftgl\FTGLBitmapFont.h>
#include <ftgl\FTGLOutlineFont.h>

class CFont{
public:
	// ����������� ������ - ������� ��� ��� .ttf-�����, ������ � ������� ������.
	CFont(char *ttf, int FSize, int FDepth);
	// ���������� ������
	~CFont();

	// ����������� ��� ����� ��������� �����
	FTFont *Font;

	// ������� ������� ������ ������
	void Print(float x, float y, const wchar_t *text);
};

#endif

