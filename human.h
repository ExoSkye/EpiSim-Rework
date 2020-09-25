#pragma once
struct color
{
	int r;
	int g;
	int b;
};

enum class infectInfo
{
	susceptible,
	infectious,
	infectious_no_symptoms,
	immune,
	dead,
};
color possibleColors[5] = { color{ 255,255,255 },color{ 255,0,0 },color{ 255,255,0 },color{ 0,255,0 },color{ 64,64,64 }, };

struct human
{
	int x;
	int y;
	infectInfo infect_info;
	//TODO: Add more entries as needed
};