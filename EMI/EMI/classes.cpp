#include "classes.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"

using namespace sf;

Entity::Entity(Image &image, float X, float Y, int W, int H, std::string Name)
{
		x = X; y = Y; //���������� ��������� �������
		w = W; h = H; 
		name = Name; 
		moveTimer = 0;
		dx = 0; dy = 0; 
		speed = 0;

		CurrentFrame = 0;
		life = true; //���������������� ���������� ���������� �����, ����� ���
		texture.loadFromImage(image); //������� ���� ����������� � ��������
		sprite.setTexture(texture); //�������� ������ ���������
}

FloatRect Entity::getRect()
{//����� ��������� ��������������. ��� �����, ������� (���,�����).
	FloatRect FR(x, y, w, h); // ���������� FR ���� FloatRect
	return FR;
		//��� ������ (�����) "sf::FloatRect" ��������� ������� ������ ���������� ��������������
		//� ����� ���� ��� ���������� �������� ������������ ����� �� �����
		//����� ��� �������� ��������, ���� �� ��� �����-���� ���� �� ���� ����� 
		//��� �-��� ����� ��� �������� ����������� 
}

Player::Player(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	state = stay;
	if (name == "Player1")
	{
		//������ ������� ���� ������������� ���
		//������ ������ ������. IntRect � ��� ���������� �����
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
	for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'w'))//���� ������� ������ �����
			{
				if (Dy > 0) { y = i * 32 - h;  dy = 0; }//�� Y 
				if (Dy < 0) { y = i * 32 + 32; dy = 0; }//������������ � �������� ������ 
				if (Dx > 0) { x = j * 32 - w; dx = 0; }//� ������ ����� �����
				if (Dx < 0) { x = j * 32 + 32; dx = 0; }// � ����� ����� �����
			}

		}
}

void Player::update(float time) //����� "���������/����������" ������� ������.
	{
		if (life)
		{//���������, ��� �� �����
			control();//������� ���������� ����������
			switch (state)//�������� ��������� �������� � ����������� �� ���������
			{
				case right:
				{//��������� ���� ������
					dx = speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 64, 64, 64));
					break;
				}
				case left:
				{//��������� ���� �����
					dx = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 128, 64, 64));
					break;
				}
				case up:
				{//���� �����
					dy = -speed;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 0, 64, 64));
					break;
				}
				case down:
				{//���� ����
					dy = speed;
					dx = 0;
					CurrentFrame += 0.005*time;
					if (CurrentFrame > 2) CurrentFrame -= 2;
					sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 196, 64, 64));
					break;
				}
				case stay:
				{//�����
					dy = speed;
					dx = speed;
					break;
				}
			}

			x += dx*time; //�������� �� �X�
			checkCollisionWithMap(dx, 0);//������������ ������������ �� �
			y += dy*time; //�������� �� �Y�
			checkCollisionWithMap(0, dy);//������������ ������������ �� Y

			speed = 0;    //�������� ��������, ����� �������� �����������.
			//state = stay;

			sprite.setPosition(x, y); //������ � ������� (x, y). 
		}
	}

Enemy::Enemy(Image &image, float X, float Y, int W, int H, std::string Name) :Entity(image, X, Y, W, H, Name)
{
	if (name == "Enemy")
	{
		//������ ������� ���� ������������� ���
		//������ ������ ������. IntRect � ��� ���������� �����
		sprite.setTextureRect(IntRect(0, 0, w, h));
		direction = rand() % (3); //����������� �������� ����� ����� ��������� �������
		//����� ��������� ��������� �����
		speed = 0.2;//���� ��������.���� ������ ������ ���������
		dx = speed;
		dy = speed;
	}
}

void Enemy::checkCollisionWithMap(float Dx, float Dy)//�-��� �������� ������������ � ������
{
	for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
		for (int j = x / 32; j<(x + w) / 32; j++)
		{
			if ((TileMap[i][j] == '0') || (TileMap[i][j] == 'w'))//���� ������� - ������ �����
			{
				if (Dy > 0) 
				{
					y = i * 32 - h;  dy = -0.1; 
					direction = rand() % (4); //����������� �������� �����
				}//�� Y 
				if (Dy < 0)
				{
					y = i * 32 + 32; dy = 0.1; 
					direction = rand() % (4);//����������� �������� ����� 
				}//������������ � �������� ������ 
				if (Dx > 0)
				{
					x = j * 32 - w; dx = -0.1; 
					direction = rand() % (4);//����������� �������� ����� 
				}//� ������ ����� �����
				if (Dx < 0)
				{
					x = j * 32 + 32; dx = 0.1; 
					direction = rand() % (4); //����������� �������� �����
				}// � ����� ����� �����
			}
		}
	}

void Enemy::update(float time)
	{
		if (name == "Enemy")
		{//��� ��������� � ����� ������ ������ ����� �����
			if (life) 
			{//���������, ��� �� �����
				switch (direction)//�������� ��������� �������� � ����������� �� ���������
				{
					case 0:
					{//��������� ���� ������
						dx = speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 64, 64, 64));
						state = right;
						break;
					}
					case 1:
					{//��������� ���� �����
						dx = -speed;
						dy = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 128, 64, 64));
						state = left;
						break;
					}
					case 2:
					{//���� �����
						dy = -speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 0, 64, 64));
						state = up;
						break;
					}
					case 3:
					{//���� ����
						dy = speed;
						dx = 0;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 2) CurrentFrame -= 2;
						sprite.setTextureRect(IntRect(64 * int(CurrentFrame), 192, 64, 64));
						state = down;
					break;
					}
				}
				x += dx*time; //�������� �� �X�
				checkCollisionWithMap(dx, 0);//������������ ������������ �� �
				y += dy*time; //�������� �� �Y�
				checkCollisionWithMap(0, dy);//������������ ������������ �� Y

				sprite.setPosition(x, y); //������ � ������� (x, y).

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
		//���� ������������� � ������������
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
			x += dx*time;//���� �������� ���� �� �
			y += dy*time;//�� �

		if (x <= 0) x = 20;// �������� ���� � ����� �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������ 
		if (y <= 0) y = 20;

		if (x >= 1280) x = 1260;// �������� ���� � ������ �����, ����� ��� ���������� ������ ��� �������� �� �������� �� ������ ����� � �� ���� ������ 
		if (y >= 720) y = 700;


			for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
				for (int j = x / 32; j < (x + w) / 32; j++)
				{
					if (TileMap[i][j] == '0')//���� ������� ��� ������ �����, ��
						life = false;// �� ���� �������
				}
			sprite.setPosition(x + w / 2, y + h / 2);//�������� ������� ����
		}
	}