#include <iostream>
#include <raylib.h>
#include <deque> 
#include <raymath.h>

Color green = { 173, 204, 96, 255 };
Color darkGreen = { 43, 51, 24, 255 };

float cellSize = 30;
float cellCount = 25;

double lastUpdateTime = 0;

int gIteration = 0;

bool ElementInDeque(Vector2 element, std::deque<Vector2> deque)
{
	for (unsigned int i = 0; i < deque.size(); i++)
	{
		if (Vector2Equals(deque[i], element))
		{
			return true;
		}
	}
	return false;
}

bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}

class Snake
{
public:
	std::deque<Vector2> body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
	Vector2 direction = { 1 , 0 };
	bool addSegment = false;

	void Draw()
	{
		for (unsigned int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{ x * cellSize, y * cellSize, cellSize, cellSize };
			DrawRectangleRounded(segment, 0.5, 6, darkGreen);
		}
	}

	void Update()
	{
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{
			body.pop_back();
		}

	}

	void Reset()
	{
		body = { Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9} };
		direction = { 1, 0 };

	}
};

class Food 
{
public:
	Vector2 position;
	Texture2D texture;


	Food(std::deque<Vector2> snakeBody)
	{
		Image image = LoadImage("Graphics/food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		position = GenerateRandomPos(snakeBody);
	}

	~Food()
	{
		UnloadTexture(texture);
	}

	void Draw()
	{
		DrawTexture(texture, position.x * cellSize, position.y * cellSize, WHITE);
	}

	Vector2 GenerateRandomCell()
	{
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{ x, y };
	}

	Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody)
	{
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}
		return position;
	}
};

class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	bool isRunning = true;

	void GameStart()
	{
		DrawText("SNAKE", 170, 300, 120, darkGreen);
		DrawText("Developed by: Rotem Tater", 170, 400, 20, darkGreen);
	}
	void Draw()
	{
		food.Draw();
		snake.Draw();
	}

	void Update()
	{
		if (isRunning)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithBody();
			CheckCollisionWithEdges();

		}
	}

	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
		}
	}

	void CheckCollisionWithBody()
	{
		for (int b = 1; b < snake.body.size(); b++)
		{
			if (Vector2Equals(snake.body[0], snake.body[b]))
			{
				GameOver();
			}
		}
	}

	void CheckCollisionWithEdges()
	{
		if (snake.body[0].x == -1 || snake.body[0].x == cellCount)
		{
			GameOver();
		}
		if (snake.body[0].y == -1 || snake.body[0].y == cellCount)
		{
			GameOver();
		}
	}

	void GameOver()
	{
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		isRunning = false;
	}
};

int main()
{
	std::cout << "Starting the game..." << std::endl;
	InitWindow(cellSize * cellCount, cellSize * cellCount, "Snake by Rotem Tater");
	SetTargetFPS(60);

	Game game = Game();



	while (WindowShouldClose() == false)
	{
		BeginDrawing();

		DrawFPS(3, 2);
		DrawText("SNAKE", 300, 2, 20, darkGreen);

		// Setting window's icon
		Image WindowIcon = LoadImage("Graphics/+oln+r");
		SetWindowIcon(WindowIcon);

		if (eventTriggered(0.2))
		{
			game.Update();
			gIteration++;
			std::cout << gIteration << std::endl;
		}

		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = { 0, -1 };
			game.isRunning = true;
		}

		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = { 0, 1 };
			game.isRunning = true;
		}

		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = { -1, 0 };
			game.isRunning = true;
		}

		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = { 1, 0 };
			game.isRunning = true;
		}


		// Drawing
		ClearBackground(green);
		game.Draw();
		game.Draw();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}