#pragma once
#include "../human.h"
#include <vector>

class renderer
{
public:
	virtual bool init() = 0;
	virtual bool drawScreen(std::vector<human> toDraw) = 0;
};
