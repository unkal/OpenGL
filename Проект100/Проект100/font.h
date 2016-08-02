#ifndef __FONT_H
#define __FONT_H

#include "main.h"
#include <wchar.h>      // Чтобы использовать в программе русский язык, мы будем
// использовать юникод, который хранится в переменных типа wchar_t
// Хидеры FTGL:
#include <ftgl\FTFont.h>
#include <ftgl\FTGLBitmapFont.h>
#include <ftgl\FTGLOutlineFont.h>

class CFont{
public:
	// Конструктор класса - передаём ему имя .ttf-файла, размер и глубину шрифта.
	CFont(char *ttf, int FSize, int FDepth);
	// Деструктор класса
	~CFont();

	// Создаваемый для нашей программы шрифт
	FTFont *Font;

	// Простая функция печати текста
	void Print(float x, float y, const wchar_t *text);
};

#endif

