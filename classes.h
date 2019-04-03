#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"

#ifndef __CCLASSES_H__
#define __CCLASSES_H__


using namespace sf;

class Entity {
public:
	enum { left, right, up, down, stay} state;// тип перечисления - состояние объекта 
	float dx, dy, x, y, speed, CurrentFrame, moveTimer;//добавили таймер
	int w, h, direction; //переменная h, хранит жизни игрока
	bool life, check; //переменная life-логическая жизнь
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт 
	std::string name;
	
	Entity(Image &image, float X, float Y, int W, int H, std::string Name);

	FloatRect getRect();
	
	virtual void update(float time) = 0;

};

class Player :public Entity 
{
	public:
		Player(Image &image, float X, float Y, int W, int H, std::string Name);
		void control();
		void checkCollisionWithMap(float Dx, float Dy);//Метод проверки столкновений с элементами карты
		void update(float time);
};

class Enemy :public Entity{
public:
	int direction;//направление движения врага
	Enemy(Image &image, float X, float Y, int W, int H, std::string Name);
	void checkCollisionWithMap(float Dx, float Dy);//ф-ция проверки столкновений с картой
	void update(float time);
};

class Bullet :public Entity{
public:
	int direction;//направление пули
	Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2);
	void update(float time);
};

#endif //__CCLASSES_H__