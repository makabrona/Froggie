#pragma once
#include "Vector2d.h"
#include "Player.h"
#include "game.h"

class Butterfly
{
public:
	Vector2d position;
	float size = 15.f;
	bool isAlive = true;

	Texture2D texture = { 0 };

	void Load(const char* filename);

	void Respawn(int screenWidth, int screenHeight);

	bool Update(Player& player);

	void Draw();
};

