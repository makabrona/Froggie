#pragma once
#include "Vector2d.h"
#include <raylib.h>
#include <iostream>
#include <vector>

class Player
{
public:

	// frog attributes
	float size = 20.f;
	float speed = 250.f;
	Vector2d position;
	Vector2d previousPosition;   //save it for collision
	int fliesEaten = 0;
	int lives = 3;
	int score = 0;

	// frog's tongue attributes
	float tongueSpeed = 200.f;
	float tongueMaxLength = 100.f;
	float tongueCurrentLength = 0.f;
	Vector2d tongueDirection = { 1, 0 };
	Vector2d tongueEnd = { 0,0 };            // extreme of the tongue
	Vector2d tongueRetract;
	bool isTongueOut = false;               // if tongue is active or not
	bool isTongueExtending = false;         // if tongue is extending or retracting
	float rotationAngle = 0.f;
	float tongueWidth = 12.f;

	//texture
	Texture2D livesTexture = { 0 };
	Texture2D texture = {0};
	Texture2D tongueTexture = {0};

	void Load(const char* inTexture, const char* inLives, const char* inTongue);
	void Controller();;
	void Tongue(float deltaTime);
	void ScreenLimits(int screenWidth, int screenHeight);
	void Draw();

	//rectangle collision with player
	Rectangle GetBounds() const;

};