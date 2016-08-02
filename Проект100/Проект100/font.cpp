#include "font.h"

// Конструктор класса.
// Передаваемые параметры: имя файла шрифта, размер и глубина шрифта.
CFont::CFont(char *ttf, int FSize, int FDepth)
{
	// Создаём новый битмапный шрифт
	this->Font = new FTGLBitmapFont(ttf);
	// Устанавливаем размер шрифта или выходим с ошибкой
	if (!Font->FaceSize(FSize)){
		MessageBox(NULL, "Can't set font FaceSize", "Error", MB_OK);
		exit(1);
	}
	// Устанавливаем глубину
	Font->Depth(FDepth);
	// И кодировку - юникод
	Font->CharMap(ft_encoding_unicode);
}

// Функция печати текста.
// Первые 2 параметра - экранные координаты (те, что мы установили в 800 и 600),
// третий - текст для печати

void CFont::Print(float x, float y, const wchar_t *text)
{
	// Входим в новую матрицу и обнуляем её
	glPushMatrix();
	glLoadIdentity();
	// Выключаем текстуры
	glDisable(GL_TEXTURE_2D);
	// Перемещаемся на указанные координаты
	glTranslatef(x, y, -1);
	glRasterPos2f(-1, 0.5);
	Font->Render(text); // И рендерим текст
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}
