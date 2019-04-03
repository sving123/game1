#include <iostream> 
#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h" //���������� ��� � ������
#include <list>
#include "classes.h"
#include "ourclasses.h"

using namespace sf;


int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::RenderWindow window(sf::VideoMode(1280, 720, desktop.bitsPerPixel), "EMI");

	Font font;//����� 
	font.loadFromFile("shrift.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 30);//������� ������ �����
	text.setColor(Color::Red);//��������� ����� � �������	
	text.setStyle(Text::Bold);//������ �����.

	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/map.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//�������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������

	Clock clock;
	Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ���� 
	int gameTime = 0;//�������� ������� �����, ����������������.
	int delay = 0;
	bool winCheck = false;
	bool slowCheck = false;
	int i = 0;


	Image heroImage;
	heroImage.loadFromFile("images/Player.png"); // ��������� ����������� ������

	Image EnemyImage;
	EnemyImage.loadFromFile("images/Enemy.png"); // ��������� ����������� �����

	Image BulletImage;//����������� ��� ����
	BulletImage.loadFromFile("images/bullet.png");//��������� �������� � ������ �����������

	Image slowImage;
	slowImage.loadFromFile("images/slow.png");

	Player p(heroImage, 1050, 360, 60, 60, "Player1");//������ ������ ������

	slow *s = new slow(slowImage, 592, 340, 21, 29, "slow");

	std::list<Entity*>  enemies; //������ ������
	std::list<Entity*>  Bullets; //������ ����
	std::list<Entity*>  Walls; //������ ����
	std::list<Entity*>::iterator it; //�������� ����� ��������� �� ��������� ������
	std::list<Entity*>::iterator it2;
	std::list<Entity*>::iterator bullet;
	std::list<Entity*>::iterator wall;

	const int ENEMY_COUNT = 7;	//������������ ���������� ������ � ����
	int enemiesCount = 0;	//������� ���������� ������ � ����

	//��������� ������ ��������� �������
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		float xr = 150 + rand() % 100;
		float yr = 480;
		 // ��������� ���������� ����� �� ���� ���� �� ��� �x�
	

		//������� ������ � �������� � ������
		enemies.push_back(new Enemy(EnemyImage, xr, yr, 60, 60, "Enemy"));
		enemiesCount += 1; //��������� ������� ������
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


	int createObjectForMapTimer = 0;//���������� ��� ����� ��� ������������� ������

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();

		if (p.life) gameTime = gameTimeClock.getElapsedTime().asMilliseconds();//������� ����� � 
		//�������� ��� ������, ���� ��� �����. ������������� ��� time ��� �� ����. 
		//��� �� ��������� ������ ����
		clock.restart();
		time = time / 800;

		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//�������� �� ������� ������� "Space"
			if ((event.type == sf::Event::KeyPressed) && (p.life == true))
			{
				if ((event.key.code == sf::Keyboard::Space))
				{
					Bullets.push_back(new Bullet(BulletImage, p.x, p.y, 16, 16, "Bullet", p.state, true));
					
				}
			}

		}

		p.update(time); //�������� ������ �p� ������ �Player� 

		//�������� ������
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
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
		//�������� ����
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			(*it)->update(time); //��������� ����� update()
		}
		//��������� ������ �� ������� "�������" ���� � ������� ��
		for (it = Bullets.begin(); it != Bullets.end(); )//������� ��� ���������� �� ������ �� �����
		{// ���� ���� ������ �����, �� ������� ���
			if ((*it)-> life == false)	
			{ 
				it = Bullets.erase(it); 
			} 
			else  it++;//� ���� �������� (����������) � ���� �������.		
		}

		for (bullet = Bullets.begin();  bullet!= Bullets.end(); bullet++)
		{
			for (it = enemies.begin(); it != enemies.end(); )
			{//����� �� ������ ������
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
/////////////////////////////������ �����/////////////////////
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
				text.setString("���������!");//������ ������ ������
				text.setPosition(520 ,0);//������ ������� ������
				window.draw(text);//������ ���� �����
				

		}
		if (enemies.empty())
		{
				winCheck = true;
				text.setString("������!");//������ ������ ������
				text.setPosition(560 ,0);//������ ������� ������
				window.draw(text);//������ ���� �����
				}

		window.draw(p.sprite);//������ ������ ������� �p� ������ �Player�
		if (slowCheck == false)
		{
			window.draw(s->sprite);
		}
		//������ ������
		for (it = enemies.begin(); it != enemies.end(); it++)
		{
			if ((*it)->life) //���� ����� ����
			window.draw((*it)->sprite); //������ 
		}

		//������ ����
		for (it = Bullets.begin(); it != Bullets.end(); it++)
		{
			if ((*it)->life) //���� ���� ����
				window.draw((*it)->sprite); //������ �������
		}

		for (it = Walls.begin(); it != Walls.end(); it++)
		{
			window.draw((*it)->sprite);
		}

		window.display();
	} 
	return 0;
}
