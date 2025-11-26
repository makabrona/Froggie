#include "Player.h"
#include "game.h"

void Player::Load(const char* inTexture, const char* inLives, const char* inTongue) {
		texture = LoadTexture(inTexture);
		livesTexture = LoadTexture(inLives);
		tongueTexture = LoadTexture(inTongue);
}

void Player::Controller() {

	Vector2d offsetPosition{ 0.f, 0.f };

	if (IsKeyDown(KEY_W)) {
		offsetPosition.y -= 1;
		tongueDirection = { 0, -1 };
	}
	else if (IsKeyDown(KEY_S)) {
		offsetPosition.y += 1;
		tongueDirection = { 0, 1 };
	}
	else if (IsKeyDown(KEY_A)) {
		offsetPosition.x -= 1;
		tongueDirection = { -1, 0 };
	}
	else if (IsKeyDown(KEY_D)) {
		offsetPosition.x += 1;
		tongueDirection = { 1, 0 };
	}

	offsetPosition = offsetPosition.NormalizeVector();

	previousPosition = position;

	Vector2d newPos = position;
	newPos = newPos.SetVectorOffset(offsetPosition.ScaleVector(speed * GetFrameTime()));

	position = newPos;
}

void Player::Tongue(float deltaTime) {

	if (IsKeyPressed(KEY_SPACE) && (!isTongueOut)) {
		isTongueOut = true;
		isTongueExtending = true;
		tongueCurrentLength = 0.f;
		tongueDirection = tongueDirection.NormalizeVector();
	}

	if (isTongueOut) {

		if (isTongueExtending) {

			tongueCurrentLength += tongueSpeed * deltaTime;

			if (tongueCurrentLength >= tongueMaxLength) {

				tongueCurrentLength = tongueMaxLength;
				isTongueExtending = false;
			}
		}
		else {

			tongueCurrentLength -= tongueSpeed * deltaTime;

			if (tongueCurrentLength <= 0.f) {
				tongueCurrentLength = 0.f;
				isTongueOut = false;
			}
		}

		tongueEnd = position.SetVectorOffset(tongueDirection.ScaleVector(tongueCurrentLength));
	}
}

void Player::ScreenLimits(int screenWidth, int screenHeight) {

	if (position.x > screenWidth - size) {
		position.x = screenWidth - size;
	}
	if (position.x < size) {
		position.x = size;
	}
	if (position.y > screenHeight - size) {
		position.y = screenHeight - size;
	}
	if (position.y < size) {
		position.y = size;
	}
}

void Player::Draw() {


	if (isTongueOut) {

		float angle = atan2(tongueDirection.y, tongueDirection.x) * (180.f / PI);

		Rectangle source = { 0, 0, (float)tongueTexture.width, (float)tongueTexture.height };

		Rectangle dest = { position.x, position.y, tongueCurrentLength, tongueWidth };

		Vector2 origin = { 0, tongueWidth / 2.f };

		DrawTexturePro(tongueTexture, source, dest, origin, angle, WHITE);

	}

	if (texture.id != 0) {

		Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
		Rectangle dest = { position.x, position.y, (float)texture.width, (float)texture.height };
		Vector2 origin = { texture.width / 2.f, texture.height / 2.0f };

		float angle = atan2(tongueDirection.y, tongueDirection.x) * (180.f / PI) + 90.f;

		DrawTexturePro(texture, source, dest, origin, angle, WHITE);
	}
	else {
		DrawCircle(position.x, position.y, size, RAYWHITE);
	}

}

Rectangle Player::GetBounds() const {

	float texWidth = (float)texture.width;
	float texHeight = (float)texture.height;

	return Rectangle{ position.x - texWidth / 2.f, position.y - texHeight / 2.f, texWidth, texHeight };
}