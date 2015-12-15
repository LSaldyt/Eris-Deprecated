#pragma once
#include "Turret.hpp"

class Railgun :
	public Turret
{
public:
	Railgun();
	~Railgun();

	Railgun(Ship * parent);

	void shoot(float rotation, sf::Vector2f pos, float velocity, std::string type, Window* board, bool enemy);
};

