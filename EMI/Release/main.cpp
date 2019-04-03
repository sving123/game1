#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //подключили код с картой
#include <list>
#include "classes.h"
#include "ourclasses.h"

using namespace sf;


int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1280, 720, desktop.bitsPerPixel), "EMI");

	Font font;//шрифт 
	font.loadFromFile("shrift.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 30);//создаем объект текст
	text.setColor(Color::Red);//покрасили текст в красный	
	text.setStyle(Text::Bold);//жирный текст.

	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/map.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//заряжаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом

	Clock clock;
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры 
	int gameTime = 0;//объявили игровое время, инициализировали.
	int delay = 0;
	bool winCheck = false;
	bool slowCheck = false;
	int i = 0;


	Image heroImage;
	heroImage.loadFromFile("images/Player.png"); // загружаем изображение игрока

	Image EnemyImage;
	EnemyImage.loadFromFile("images/Enemy.png"); // загружаем изображение врага

	Image BulletImage;//изображение для пули
	BulletImage.loadFromFile("images/bullet.png");//загрузили картинку в объект изображения

	Image slowImage;
	slowImage.loadFromFile("images/slow.png");

	Player p(heroImage, 1050, 360, 60, 60, "Player1");//объект класса игрока

	slow *s = new slow(slowImage, 592, 340, 21, 29, "slow");

	std::list<Entity*>  enemies; //список врагов
	std::list<Entity*>  Bullets; //список пуль
	std::list<Entity*>  Walls; //список стен
	std::list<Entity*>::iterator it; //итератор чтобы проходить по элементам списка
	std::list<Entity*>::iterator it2;
	std::list<Entity*>::iterator bullet;
	std::list<Entity*>::iterator wall;

	const int ENEMY_COUNT = 7;	//максимальное количество врагов в игре
	int enemiesCount = 0;	//текущее количество врагов в игре

	//Заполняем список объектами врагами
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 100;
		float yr = 480;
		 // случайная координата врага на поле игры по оси “x”
	

		//создаем врагов и помещаем в список
		enemies.push_back(new Enemy(EnemyImage, xr, yr, 60, 60, "Enemy"));
		enemiesCount += 1; //увеличили счётчик врагов
	}

	Walls.push_back(new Wall(map_image, 640, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 576, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 608, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 544, 288, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 544, 320, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 544, 352, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 544, 384, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 544, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 576, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 608, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 640, 416, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 640, 320, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 640, 352, 32, 32, "Wall"));
	Walls.push_back(new Wall(map_image, 640, 384, 32, 32, "Wall"));


	int createObjectForMapTimer = 0;//Переменная под время для генерирования камней

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asMilliseconds();//игровое время в 
		//секундах идёт вперед, пока жив игрок. Перезагружать как time его не надо. 
		//оно не обновляет логику игры
		clock.restart();
		time = time / 800;

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//стреляем по нажатию клавиши "Space"
			if ((event.type == sf::Event::KeyPressed) && (p.life == true))
			{
				if ((event.key.code == sf::Keyboard::Space))
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state, true));
					
				}
			}

		}

		p.update(time); //оживляем объект “p” класса “Player” 

		//оживляем врагов
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time); //запускаем метод update()
		}
		
		delay = delay + 1;

		if (delay == 500)
		{
			for (it = enemies.begin(); it != enemies.end(); it++)
			{
				if (rand() % 2 == 0)
				{
					Bullets.push_back(new Bullet(BulletImage, (*it)->x, (*it)->y, 16, 16, "Bullet", (*it)->state, false));
				}
			}
			delay = 0;
		}
		//оживляем пули
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //запускаем метод update()
		}
		//Проверяем список на наличие "мертвых" пуль и удаляем их
		for (it = Bullets.begin(); it != Bullets.end(); )//говорим что проходимся от начала до конца
		{// если этот объект мертв, то удаляем его
			if ((*it)-> life == false)	
			{ 
				it = Bullets.erase(it); 
			} 
			else  it++;//и идем курсором (итератором) к след объекту.		
		}

		for (bullet = Bullets.begin();  bullet!= Bullets.end(); bullet++)
		{
			for (it = enemies.begin(); it != enemies.end(); )
			{//бежим по списку врагов
				if (((*bullet)->getRect().intersects((*it)->getRect())) && ((*it)->name == "Enemy") && ((*bullet)->check == true))
				{
					it = enemies.erase(it);
					(*bullet)->life = false;
				}
				else it++;
			}

			if (p.getRect().intersects((*bullet)->getRect()) && ((*bullet)->check == false))
			{
				p.life = false;
			}


			for (it = Walls.begin(); it != Walls.end(); )
			{
				if ((*bullet)->getRect().intersects((*it)->getRect()))
				{
					it = Walls.erase(it);
					(*bullet)->life = false;
				}
				else it++;
			}
		}


		for (it = enemies.begin(); it != enemies.end(); it++) 
		{
			if ((*it)->getRect().intersects(p.getRect()))
			{
				p.life = false;
			}
		}

		

		if (p.getRect().intersects(s->getRect()) && (slowCheck == false))
		{
			for (it = enemies.begin(); it != enemies.end(); it++) 
			{
				(*it)->speed = 0.05;
			}
			delete s;
			slowCheck = true;
		}

	window.clear();
/////////////////////////////Рисуем карту/////////////////////
	for (int i = 0; i < HEIGHT_MAP; i++)
		for (int j = 0; j < WIDTH_MAP; j++)
		{
			if ((TileMap[i][j] == '0')) s_map.setTextureRect(IntRect(64, 0, 32, 32));
			if ((TileMap[i][j] == ' ')) s_map.setTextureRect(IntRect(0, 0, 32, 32));
			if ((TileMap[i][j] == 'w')) s_map.setTextureRect(IntRect(96, 0, 32, 32));
			if ((TileMap[i][j] == 'b')) s_map.setTextureRect(IntRect(129, 0, 32, 32));
			s_map.setPosition(j * 32, i * 32);
			window.draw(s_map);
		}
		if ((p.life == false) && (winCheck == false))
		{
				text.setString("Поражение!");//задаем строку тексту
				text.setPosition(520 ,0);//задаем позицию текста
				window.draw(text);//рисуем этот текст
				

		}
		if (enemies.empty())
		{
				winCheck = true;
				text.setString("Победа!");//задаем строку тексту
				text.setPosition(560 ,0);//задаем позицию текста
				window.draw(text);//рисуем этот текст
				}

		window.draw(p.sprite);//рисуем спрайт объекта “p” класса “Player”
		if (slowCheck == false)
		{
			window.draw(s->sprite);
		}
		//рисуем врагов
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			if ((*it)->life) //если враги живы
			window.draw((*it)->sprite); //рисуем 
		}

		//рисуем пули
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			if ((*it)->life) //если пули живы
				window.draw((*it)->sprite); //рисуем объекты
		}

		for (it = Walls.begin(); it != Walls.end(); it++)
		{
			window.draw((*it)->sprite);
		}

		window.display();
	} 
	return 0;
}
