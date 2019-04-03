#include <sstream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "classes.h"


#ifndef __OURCCLASSES_H__
#define __OURCCLASSES_H__


class Wall :public Entity//класс стена
{
	public:
		Wall(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};

class slow :public Entity //класс замедление
{
	public:
		slow(Image &image, float X, float Y, int W, int H, std::string  Name);
		void update(float time) {};
};
#endif //__OURCCLASSES_H__