#include "classes.h"
#include <iostream> 
#include <SFML/Graphics.hpp>
#include "map.h"
#include "ourclasses.h"


using namespace sf;


Wall::Wall(Image &image, float X, float Y, int W, int H, std::string  Name) :Entity(image, X, Y, W, H, Name)
{
	x = X;
	y = Y;
	w = W;
	h = H;
	life = true;
	sprite.setTextureRect(IntRect(32, 0, w, h));
	sprite.setPosition(x, y);
};

slow::slow(Image &image, float X, float Y, int W, int H, std::string  Name) :Entity(image, X, Y, W, H, Name)
{
		x = X;
		y = Y;
		w = W;
		h = H;
		life = true;
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);

};