#pragma once
#ifndef human_imp
#define human_imp

#include <stdexcept>

struct color
{
	int r;
	int g;
	int b;
};

enum operatorType {
    add,
    minus,
    multiply,
    divide,
    power
};

#include <type_traits>

template<typename T>
class formula {
private:
    operatorType _op;
    T _scalar;
public:
    formula(operatorType op, T scalar) {
        static_assert(std::is_arithmetic<T>::value, "Not an arithmetic type");
        _op = op;
        _scalar = scalar;
    }
    T get(T in) {
        switch (_op) {
            case add:
                return in + _scalar;
                break;
            case minus:
                return in - _scalar;
                break;
            case divide:
                return in * _scalar;
                break;
            case multiply:
                return in / _scalar;
                break;
            case power:
                return pow(in, _scalar);
                break;
            default:
                throw std::runtime_error("ERROR: Formula operation wasn't in enum");
        }
    }
    void update(operatorType newOp, T newScalar) {
        _op=newOp;
        _scalar=newScalar;
    }
};

enum class infectInfo
{
	susceptible,
	infectious,
	infectious_no_symptoms,
	immune,
	dead,
};
static color possibleColors[5] = { color{ 255,255,255 },color{ 255,0,0 },color{ 255,255,0 },color{ 0,255,0 },color{ 64,64,64 }, };

struct human
{
	int x;
	int y;
	infectInfo infect_info;
	bool real = true;
	int time = 0;
	int peopleInfected = 0;
	//TODO: Add more entries as needed
};


struct HumanIND {
    human person;
    int i;
};
#endif