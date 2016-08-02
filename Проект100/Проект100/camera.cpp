// Включаем наш хидер
#include "main.h"
#include "Camera.h"

// Увеличим скорость
#define kSpeed 15.0f

//интервал времени с последнего кадра
float g_FrameInterval = 0.00f;

///////////////////////////////// GET FRAME TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////						Вычисляет время с последнего кадра
/////
///////////////////////////////// GET FRAME TIME \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void GetFrameTime()
{
	static float frameTime = 0.00f;	// Время последнего кадра

	float currentTime = timeGetTime() * 0.001f;

	// Интервал времени, прошедшего с прошлого кадра
	g_FrameInterval = currentTime - frameTime;

	frameTime = currentTime;
}

/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
/////
/////	Возвращает перпендикулярный вектор от 2х переданных векторов.
/////   2 любых пересекающихся вектора образуют плоскость, от котороый мы
/////   и ищем перпендикуляр.
/////
/////////////////////////////////////// CROSS \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

CVector3 Cross(CVector3 vVector1, CVector3 vVector2)
{
	CVector3 vNormal;

	// Если у нас есть 2 вектора (вектор взгляда и вертикальный вектор), 
	// у нас есть плоскость, от которой мы можем вычислить угол в 90 градусов.
	// Рассчет cross'a прост, но его сложно запомнить с первого раза. 
	// Значение X для вектора - (V1.y * V2.z) - (V1.z * V2.y)
	vNormal.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));

	// Значение Y - (V1.z * V2.x) - (V1.x * V2.z)
	vNormal.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));

	// Значение Z: (V1.x * V2.y) - (V1.y * V2.x)
	vNormal.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	// вернем результат.
	return vNormal;
}

/////				Возвращает величину вектора

float Magnitude(CVector3 vNormal)
{
	// Это даст нам величину нашей нормали, 
	// т.е. длину вектора. Мы используем эту информацию для нормализации
	// вектора. Вот формула: magnitude = sqrt(V.x^2 + V.y^2 + V.z^2)   где V - вектор.

	return (float)sqrt((vNormal.x * vNormal.x) +
		(vNormal.y * vNormal.y) +
		(vNormal.z * vNormal.z));
}

/////	Возвращает нормализированный вектор, длинна которого==1, 
/////	это делает все рассчеты проще.
CVector3 Normalize(CVector3 vVector)
{
	// Вы спросите, для чего эта ф-я? Мы должны убедиться, что наш вектор нормализирован.
	// Вектор нормализирован - значит, его длинна равна 1. Например,
	// вектор (2,0,0) после нормализации будет (1,0,0).

	// Вычислим величину нормали
	float magnitude = Magnitude(vVector);

	// Теперь у нас есть величина, и мы можем разделить наш вектор на его величину.
	// Это сделает длинну вектора равной единице, так с ним будет легче работать.
	vVector = vVector / magnitude;

	return vVector;
}

/////	Добавим ф-ю управления взглядом с пом. мышки
void CCamera::SetViewByMouse()
{
	POINT mousePos;			// Это структура, хранящяя X и Y позиции мыши

	// Оператор " >> 1" - то же самое, что деление на 2, но о**енно быстрее ;)
	int middleX = SCREEN_WIDTH >> 1;	// Вычисляем половину ширины
	int middleY = SCREEN_HEIGHT >> 1;	// И половину высоты экрана
	float angleY = 0.0f;	// Направление взгляда вверх/вниз
	float angleZ = 0.0f;	// Значение, необходимое для вращения влево-вправо (по оси Y)
	static float currentRotX = 0.0f;

	// Получаем текущие коорд. мыши
	GetCursorPos(&mousePos);

	// Если курсор остался в том же положении, мы не вращаем камеру
	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	// Теперь, получив координаты курсора, возвращаем его обратно в середину.
	SetCursorPos(middleX, middleY);

	// Теперь нам нужно направление (или вектор), куда сдвинулся курсор.
	// Его рассчет - простое вычитание. Просто возьмите среднюю точку и вычтите из неё
	// новую позицию мыши: VECTOR = P1 - P2; где P1 - средняя точка (400,300 при 800х600).
	// После получения дельты X и Y (или направления), я делю значение 
	// на 1000, иначе камера будет жутко быстрой.
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;

	static float lastRotX = 0.0f;
	lastRotX = currentRotX;		// Сохраняем последний угол вращения 
	// и используем заново currentRotX

	// Если текущее вращение больше 1 градуса, обрежем его, чтобы не вращать слишком быстро
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		// врощаем на оставшийся угол
		if (lastRotX != 1.0f)
		{
			// Чтобы найти ось, вокруг которой вращаться вверх и вниз, нужно 
			// найти вектор, перпендикулярный вектору взгляда камеры и 
			// вертикальному вектору.
			// Это и будет наша ось. И прежде чем использовать эту ось, 
			// неплохо бы нормализовать её.
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);

			// Вращаем камеру вокруг нашей оси на заданный угол
			RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}

	// Если угол меньше -1.0f, убедимся, что вращение не продолжится
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;
		if (lastRotX != -1.0f)
		{
			// Опять же вычисляем ось
			CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
			vAxis = Normalize(vAxis);

			// Вращаем
			RotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Если укладываемся в пределы 1.0f -1.0f - просто вращаем
	else
	{
		CVector3 vAxis = Cross(m_vView - m_vPosition, m_vUpVector);
		vAxis = Normalize(vAxis);
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}

	// Всегда вращаем камеру вокруг Y-оси
	RotateView(angleY, 0, 1, 0);
}
/////	Конструктор класса CCamera
CCamera::CCamera()
{
	CVector3 vZero = CVector3(0.0, 0.0, 0.0);		//Инициализируем вектор нашей позиции в нулевые координаты
	CVector3 vView = CVector3(0.0, 1.0, 0.5);		//Иниц. вектор взгляда
	CVector3 vUp = CVector3(0.0, 0.0, 1.0);		//Вектор верт.

	m_vPosition = vZero;
	m_vView = vView;
	m_vUpVector = vUp;
}

/////	Устанавливает позицию, взгляд, верт. вектор камеры
GLvoid CCamera::PositionCamera(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ,
	float upVectorX, float upVectorY, float upVectorZ)
{
	CVector3 vPosition = CVector3(positionX, positionY, positionZ);
	CVector3 vView = CVector3(viewX, viewY, viewZ);
	CVector3 vUpVector = CVector3(upVectorX, upVectorY, upVectorZ);

	//Обширный код просто делает легче назначение переменных.
	//Можно просто сразу присвоить переменным класса переданные функции значения.

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}
/////	                  Вращение камеры вокруг оси

void CCamera::RotateView(float angle, float x, float y, float z)
{
	CVector3 vNewView;
	CVector3 vView;

	// Получим наш вектор взгляда (направление, куда мы смотрим)
	vView.x = m_vView.x - m_vPosition.x;	//направление по X
	vView.y = m_vView.y - m_vPosition.y;	//направление по Y
	vView.z = m_vView.z - m_vPosition.z;	//направление по Z


	// Рассчитаем 1 раз синус и косинус переданного угла
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Найдем новую позицию X для вращаемой точки
	vNewView.x = (cosTheta + (1 - cosTheta) * x * x)	* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Найдем позицию Y
	vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)	* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// И позицию Z
	vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)	* vView.z;

	// Теперь просто добавим новый вектор вращения к нашей позиции, чтобы
	// установить новый взгляд камеры.
	m_vView.x = m_vPosition.x + vNewView.x;
	m_vView.y = m_vPosition.y + vNewView.y;
	m_vView.z = m_vPosition.z + vNewView.z;
}

/////			   стрейфит камеру в стороны:
void CCamera::StrafeCamera(float speed)
{
	// добавим вектор стрейфа к позиции
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Добавим теперь к взгляду
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

/////	Движение камеры вперед-назад с заданной скоростью
void CCamera::MoveCamera(float speed)
{
	CVector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;
	m_vPosition.z += vVector.z * speed;
	m_vView.x += vVector.x * speed;
	m_vView.z += vVector.z * speed;

}

/////			обрабатываем ввод
void CCamera::CheckForMovement()
{
	// Теперь, имея интервал времени, прошедший с последнего кадра, 
	// мы получаем необходимую скорость умножением скорости на интервал времени:
	float speed = kSpeed * g_FrameInterval;

	// И дальше во все функции передаём полученное значение:

	// нажаты ли W
	if (GetKeyState('W') & 0x80) {
		MoveCamera(speed);
	}

	//  или S
	if (GetKeyState('S') & 0x80) {
		MoveCamera(-speed);
	}

	// или A
	if ( GetKeyState('A') & 0x80) {
		StrafeCamera(-speed);
	}

	//  или D
	if (GetKeyState('D') & 0x80) {
		// стрейфим вправо
		StrafeCamera(speed);
	}
}

/////			Обновляет вектор взгляда и верт. вектор
void CCamera::Update()
{
	// Ниже мы рассчитываем вектор стрейфа каждый раз, когда мы апдейтим
	// камеру. Это необходимо, т.к. значения используются во многих местах.

	// Иниц. переменную для результата cross 
	CVector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	//Нормализуем вектор стрейфа
	m_vStrafe = Normalize(vCross);

	// Посмотрим, двигалась ли мыша
	SetViewByMouse();

	// Проверим нажатия
	CheckForMovement();

	GetFrameTime();
}
/////			Обновляет взгляд камеры
void CCamera::Look()
{
	// Дадим OpenGL позицию, взгляд и верт. вектор камеры
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_vView.x, m_vView.y, m_vView.z,
		m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}


