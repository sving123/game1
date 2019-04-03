#include "classes.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"

using namespace sf;

Entity::Entity(Image &image, float X, float Y, int W, int H, std::string Name)
{
		x = X; y = Y; //координата появления спрайта
		w = W; h = H; 
		name = Name; 
		moveTimer = 0;
		dx = 0; dy = 0; 
		speed = 0;

		CurrentFrame = 0;
		life = true; //инициализировали логическую переменную жизни, герой жив
		texture.loadFromImage(image); //заносим наше изображение в текстуру
		sprite.setTexture(texture); //заливаем спрайт текстурой
}

FloatRect Entity::getRect()
{//метод получения прямоугольника. его коорд, размеры (шир,высот).
	FloatRect FR(x, y, w, h); // переменная FR типа FloatRect
	return FR;
		//Тип данных (класс) "sf::FloatRect" позволяет хранить четыре координаты прямоугольника
		//в нашей игре это координаты текущего расположения тайла на карте
		//далее это позволит спросить, есть ли ещё какой-либо тайл на этом месте 
		//эта ф-ция нужна для проверки пересечений 
}

Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	state = stay;
	if (name == "Player1")
	{
		//Задаем спрайту один прямоугольник для
		//вывода одного игрока. IntRect – для приведения типов
		sprite.setTextureRect(IntRect(0, 0, 60, 60));
	}
}

void Player::control()
{
	if (Keyboard::isKeyPressed(Keyboard::Left))
	{
			state = left;
			speed = 0.1;
	}
	if (Keyboard::isKeyPressed(Keyboard::Right)) 
	{
			state = right;
			speed = 0.1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Up))
	{
			state = up;
			speed = 0.1;
	}

	if (Keyboard::isKeyPressed(Keyboard::Down))
	{
			state = down;
			speed = 0.1;
	}
}

void Player::checkCollisionWithMap(float Dx, float Dy)
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'w'))//если элемент тайлик земли
			{
				if (Dy > 0) { y = i * 32 - h;  dy = 0; }//по Y 
				if (Dy < 0) { y = i * 32 + 32; dy = 0; }//столкновение с верхними краями 
				if (Dx > 0) { x = j * 32 - w; dx = 0; }//с правым краем карты
				if (Dx < 0) { x = j * 32 + 32; dx = 0; }// с левым краем карты
			}

		}
}

void Player::update(float time) //метод "оживления/обновления" объекта класса.
	{
		if (life)
		{//проверяем, жив ли герой
			control();//функция управления персонажем
			switch (state)//делаются различные действия в зависимости от состояния
			{
				case right:
				{//состояние идти вправо
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 64, 64, 64));
					break;
				}
				case left:
				{//состояние идти влево
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 128, 64, 64));
					break;
				}
				case up:
				{//идти вверх
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 0, 64, 64));
					break;
				}
				case down:
				{//идти вниз
					dy = speed;
					dx = 0;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 196, 64, 64));
					break;
				}
				case stay:
				{//стоим
					dy = speed;
					dx = speed;
					break;
				}
			}

			x += dx*time; //движение по “X”
			checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
			y += dy*time; //движение по “Y”
			checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

			speed = 0;    //обнуляем скорость, чтобы персонаж остановился.
			//state = stay;

			sprite.setPosition(x, y); //спрайт в позиции (x, y). 
		}
	}

Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	if (name == "Enemy")
	{
		//Задаем спрайту один прямоугольник для
		//вывода одного игрока. IntRect – для приведения типов
		sprite.setTextureRect(IntRect(0, 0, w, h));
		direction = rand() % (3); //Направление движения врага задаём случайным образом
		//через генератор случайных чисел
		speed = 0.2;//даем скорость.этот объект всегда двигается
		dx = speed;
		dy = speed;
	}
}

void Enemy::checkCollisionWithMap(float Dx, float Dy)//ф-ция проверки столкновений с картой
{
	for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'w'))//если элемент - тайлик земли
			{
				if (Dy > 0) 
				{
					y = i * 32 - h;  dy = -0.1; 
					direction = rand() % (4); //Направление движения врага
				}//по Y 
				if (Dy < 0)
				{
					y = i * 32 + 32; dy = 0.1; 
					direction = rand() % (4);//Направление движения врага 
				}//столкновение с верхними краями 
				if (Dx > 0)
				{
					x = j * 32 - w; dx = -0.1; 
					direction = rand() % (4);//Направление движения врага 
				}//с правым краем карты
				if (Dx < 0)
				{
					x = j * 32 + 32; dx = 0.1; 
					direction = rand() % (4); //Направление движения врага
				}// с левым краем карты
			}
		}
	}

void Enemy::update(float time)
	{
		if (name == "Enemy")
		{//для персонажа с таким именем логика будет такой
			if (life) 
			{//проверяем, жив ли герой
				switch (direction)//делаются различные действия в зависимости от состояния
				{
					case 0:
					{//состояние идти вправо
						dx = speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 64, 64, 64));
						state = right;
						break;
					}
					case 1:
					{//состояние идти влево
						dx = -speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 128, 64, 64));
						state = left;
						break;
					}
					case 2:
					{//идти вверх
						dy = -speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 0, 64, 64));
						state = up;
						break;
					}
					case 3:
					{//идти вниз
						dy = speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 192, 64, 64));
						state = down;
					break;
					}
				}
				x += dx*time; //движение по “X”
				checkCollisionWithMap(dx, 0);//обрабатываем столкновение по Х
				y += dy*time; //движение по “Y”
				checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y

				sprite.setPosition(x, y); //спрайт в позиции (x, y).

			}
		}
	}

Bullet::Bullet(Image &image, float X, float Y, int W, int H, std::string  Name, int dir, bool check2) :Entity(image, X, Y, W, H, Name)
{
	x = X+20;
	y = Y+5;
	direction = dir;
	speed = 0.8;
	w = h = 16;
	life = true;
	check = check2;
		//выше инициализация в конструкторе
};

void Bullet::update(float time)
	{
		switch (direction)
		{
			case 0: dx = -speed; dy = 0;   break;// state = left
			case 1: dx = speed; dy = 0;   break;// state = right
			case 2: dx = 0; dy = -speed;   break;// state = up
			case 3: dx = 0; dy = speed;   break;// state = down
		}

		if (life)
		{
			x += dx*time;//само движение пули по х
			y += dy*time;//по у

		if (x <= 0) x = 20;// задержка пули в левой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки 
		if (y <= 0) y = 20;

		if (x >= 1280) x = 1260;// задержка пули в правой стене, чтобы при проседании кадров она случайно не вылетела за предел карты и не было ошибки 
		if (y >= 720) y = 700;


			for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//если элемент наш тайлик земли, то
						life = false;// то пуля умирает
				}
			sprite.setPosition(x + w / 2, y + h / 2);//задается позицию пули
		}
	}