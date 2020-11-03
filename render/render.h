#pragma once
#include "../human.h"
#include <vector>
#ifdef _DEBUG
#include <cstdio>
#endif

class renderer
{
public:
	virtual bool init(int x, int y, bool render) = 0;
	virtual bool drawScreen(const std::vector<human>& toDraw) = 0;
	virtual void end() = 0;
	bool pause = false;
};
