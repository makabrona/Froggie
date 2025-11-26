#include <iostream>
#include <raylib.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Vector2d.h"
#include "Game.h"
#include "Player.h"
#include "Fly.h"
#include "Bee.h"
#include "Butterfly.h"

using namespace std;

int main() {

	int screenWidth = 800;
	int screenHeight = 800;

	float halfScreenWidth = (float)(screenWidth / 2);
	float halfScreenHeight = (float)(screenHeight / 2);

	int previousLevel = 0;
	int currentLevel = 1;
	int nextLevel = 1;
	float levelScreenTimer = 0.f;
	int fliesToWin;



	// Player Setup
	Player player;
	player.position = { 400, 700 };
	

	// Collectable Setup
	int amountOfFlies = 3;
	vector<Fly>  flyContainer(amountOfFlies);

	for (Fly& fly : flyContainer) {
		fly.Respawn(screenWidth, screenHeight);
	}

	// Enemy Setup
	Vector2d beeStartPosition = { halfScreenWidth, halfScreenHeight };
	// Level 1
	Bee bee1;
	bee1.type = 1;
	bee1.startPosition = beeStartPosition;
	bee1.spawnDelay = 1.f;
	bee1.Respawn();

	// Level 2
	Bee bee2;
	bee2.type = 2;
	bee2.startPosition = beeStartPosition;
	bee2.spawnDelay = 2.f;
	bee2.Respawn();

	// PowerUp Setup ()
	vector<Butterfly> butterflyContainer;

	// WALLS TO COLLIDE WITH
	vector <Rectangle> walls;
	walls.push_back({ 76, 84, 220, 159 });
	walls.push_back({ 69, 235, 153, 72 });
	walls.push_back({ 320, 92, 42, 40 });
	walls.push_back({ 492, 565, 232, 150 });
	walls.push_back({ 532, 476, 43, 39 });
	walls.push_back({ 589, 503, 137, 60 });
	walls.push_back({ 222, 610, 47, 42 });
	walls.push_back({ 276, 575, 81, 51 });
	walls.push_back({ 313, 597, 86, 67 });
	walls.push_back({ 145, 386, 33, 158 });
	walls.push_back({ 181, 410, 48, 115 });
	walls.push_back({ 656, 430, 32, 37 });
	walls.push_back({ 571, 290, 55, 92 });
	walls.push_back({ 608, 254, 44, 193 });
	walls.push_back({ 383, 191, 84, 35 });
	walls.push_back({ 403, 140, 95, 57 });
	walls.push_back({ 460, 126, 28, 29 });
	walls.push_back({ 484, 166, 47, 56 });
	walls.push_back({ 534, 149, 40, 40 });

	Vector2 circleCenter = { halfScreenWidth, halfScreenHeight };
	float circleRadius = 60.f;

	InitWindow(screenWidth, screenHeight, "Froggi");
	InitAudioDevice();
	SetTargetFPS(60);

	// Load Textures, Music and Sound
	LoadTextures();
	player.Load("assets/frog.png", "assets/lives.png","assets/tongue.png");
	bee1.Load("assets/beeAlive.png", "assets/beeDead.png");
	bee2.Load("assets/beeAlive.png", "assets/beeDead.png");
	Texture2D butterflyTexture = LoadTexture("assets/butterfly.png");
	Texture2D flyTexture = LoadTexture("assets/fly.png");
	Music music = LoadMusicStream("assets/music_background.mp3");
	Sound pacman = LoadSound("assets/music_gameOver.mp3");
	Sound beeKill = LoadSound("assets/music_beeKill.mp3");
	Sound beeDie = LoadSound("assets/music_beeDie.mp3");
	PlayMusicStream(music);
	music.looping = true;

	while (!WindowShouldClose()) {

		UpdateMusicStream(music);

		float deltaTime = GetFrameTime();
		bee1.searchTimer = GetFrameTime();
		bee2.searchTimer = GetFrameTime();

		BeginDrawing();
		ClearBackground(BLACK);

		if (currentLevel != previousLevel) {
			switch (currentLevel) {
			case 1:
				fliesToWin = 6;
				bee2.Respawn(2.f);
				break;

			case 2:
				fliesToWin = 10;
				bee1.Respawn(2.f);
				bee2.Respawn(0.8f);

				break;

			case 3:
				fliesToWin = 20;
				bee1.Respawn(2.f);
				bee2.Respawn(0.5);
				break;
			}
			previousLevel = currentLevel;
		}

		switch (actualScreen) {
		case Main:

			DrawTexture(backgroundMainMenu, 0, 0, WHITE);

			if (IsKeyDown(KEY_SPACE)) {
				actualScreen = Level;
				nextLevel = currentLevel;
				levelScreenTimer = 2.f;
			}

			break;
		case Play:

			DrawTexturePro(backgroundPlay, { 0, 0, (float)backgroundPlay.width, (float)backgroundPlay.height }, { 0, 0, (float)screenWidth, (float)screenHeight }, { 0, 0 }, 0.f, WHITE);

			// Player Update
			player.previousPosition;

			player.Controller();
			player.Tongue(deltaTime);

			Rectangle playerRect = player.GetBounds();
			for (const auto& wall : walls) {
				if (CheckCollisionRecs(playerRect, wall)) {
					player.position = player.previousPosition;
					break;
				}
			}

			if (CheckCollisionCircles({ player.position.x, player.position.y }, player.size, circleCenter, circleRadius)) {
				player.position = player.previousPosition;
			}
			player.ScreenLimits(screenWidth, screenHeight);
			player.Draw();

			// Flies Update
			for (Fly& fly : flyContainer) {
				fly.texture = flyTexture;
				fly.Update(player);
				fly.Draw();

				if (!fly.isAlive) {
					fly.Respawn(screenWidth, screenHeight);
				}
			}

			//// Butterfly Update

			for (Butterfly& butterfly : butterflyContainer) {
				if (butterfly.Update(player)) {

					bee1.actualState = Bee::Butterfly;
					bee1.butterflyTimer = 5.f;

					bee2.actualState = Bee::Butterfly;
					bee2.butterflyTimer = 5.f;

				}
				
				butterfly.Draw();
			}

			// Bees Update
			if (currentLevel >= 1) {
				bee2.Update(player, deltaTime);
				bee2.Draw();
			}
			if (currentLevel >= 2) {
				bee1.Update(player, deltaTime);
				bee1.Draw();
			}

			// Level Update

			if (player.fliesEaten >= fliesToWin && actualScreen != Level) {

				if (currentLevel < 3) {
					nextLevel = currentLevel + 1;
					levelScreenTimer = 2.f; // show level screen for 3 seconds
					player.fliesEaten = 0;
					actualScreen = Level;
				}
				else {
					actualScreen = GameOver;
				}
			}

			// Show points gained when eat something

			if (drawPointsGained) {
				pointsGainedTimer -= deltaTime;
				if (pointsGainedTimer <= 0.f) {
					drawPointsGained = false;
				}
			}

			if (drawPointsGained) {
				DrawText(TextFormat("+%i", pointsGained), (int)pointsPosition.x, (int)pointsPosition.y, 20, YELLOW);
			}

			// Show Level
			DrawText(TextFormat("LEVEL %i", currentLevel), halfScreenWidth - 30, 10, 20, WHITE);

			// Show Flies Eaten
			DrawText(TextFormat("Flies eaten: %i", player.fliesEaten), 10, 10, 20, YELLOW);

			// Show Highscore
			DrawText(TextFormat("%i", player.score), 700, 10, 20, YELLOW);

			// Show Lives
			for (int i = 0; i < player.lives; i++) {
				DrawTexture(player.livesTexture, 30 + i * 40 - player.livesTexture.width /2, 50 - player.livesTexture.height / 2, WHITE);
			}

			break;

		case Level:

			// Show this screen when level up
			DrawTexture(backgroundLevelScreen, 0, 0, WHITE);


			switch (nextLevel) {
			case 1:
				fliesToWin = 6;
				break;
			case 2:
				fliesToWin = 10;
				break;
			case 3:
				fliesToWin = 20;
				break;
			}

			DrawText(TextFormat("LEVEL %i", nextLevel), 280, 210, 50, WHITE);
			DrawText(TextFormat("EAT %i FLIES", fliesToWin), 275, 380, 50, WHITE);
			DrawText(TextFormat("Lives left %i", player.lives), 330, 500, 20, WHITE);

			// show level screen for 2 seconds
			levelScreenTimer -= deltaTime;
			if (levelScreenTimer <= 0.f) {
				currentLevel = nextLevel;
				player.fliesEaten = 0;
				actualScreen = Play;

				butterflyContainer.clear();
				int numButterflies = 0;

				switch (currentLevel) {
				case 1:
					numButterflies = 0;
					break;
				case 2:
					numButterflies = 2;
					break;
				case 3:
					numButterflies = 3;
					break;

				}

				butterflyContainer.resize(numButterflies);
				for (Butterfly& butterfly : butterflyContainer) {
	
					butterfly.texture = butterflyTexture;
					butterfly.Respawn(screenWidth, screenHeight);
				}
			}

			break;

		case GameOver:

			DrawTexture(backgroundGameOver, 0, 0, WHITE);

			DrawText(TextFormat("SCORE %i", (int)player.score), 350, 360, 20, WHITE);

			if (IsKeyPressed(KEY_SPACE)) {
				// reset all variables
				actualScreen = Play;
				currentLevel = 1;
				nextLevel = 1;
				previousLevel = 0;
				player.position = { 400, 700 };
				player.fliesEaten = 0;
				player.score = 0;
				player.lives = 3;
				// reset bees:
				bee1.Respawn();
				bee2.Respawn();
				//reset flies and butterflies
				for (Fly& fly : flyContainer) {
				
					fly.Respawn(screenWidth, screenHeight);
				}

				butterflyContainer.clear();
			}

			break;
		}


		EndDrawing();
	}

	// Unload and close resources

	UnloadTextures();
	UnloadMusicStream(music);

	UnloadSound(pacman);
	UnloadSound(beeDie);
	UnloadSound(beeKill);

	CloseAudioDevice();
	CloseWindow();
	return 0;
}