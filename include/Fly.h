#pragma once
#include "Vector2d.h"
#include "Player.h"
#include "game.h"

class Fly
{
public:
	Vector2d position;
	float size = 10.f;
	bool isAlive = true;
	bool isAttachedToTongue = false;

	Texture2D texture = { 0 };

	void Load(const char* filename);

	void Respawn(int inScreenWidth, int inScreenHeight);

	void Update(Player& player);

	void Draw();
};

