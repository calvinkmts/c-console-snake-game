#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include <vector>
#include <utility>

using namespace std;

void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}

class TimerMsec
{
	unsigned resetTime; //in ticks

public:
	TimerMsec() {}

	void Reset()
	{
		resetTime = clock();
	}

	unsigned Get()
	{
		return unsigned((clock() - resetTime)*1000.0 / CLOCKS_PER_SEC);
	}
};

class Snake {
public:
	Snake() {
		x = 50 / 2;
		y = 23 / 2;
		length = 1;
		snakeBodyPos.push_back(make_pair(x, y));
		direction = 0;
	}

	int getHeadX() { return x; }
	int getHeadY() { return y; }
	int getDirection() { return direction; }

	void move() {
		switch (direction)
		{
		case 0:
			break;
		case 1:
			y = y - 1;
			break;
		case 2:
			x = x + 1;
			break;
		case 3:
			y = y + 1;
			break;
		case 4:
			x = x - 1;
			break;
		default:
			break;
		}

		movement();
	}

	void movement() {
		for (int i = snakeBodyPos.size() - 1; i >= 1; i--) {
			snakeBodyPos[i].first = snakeBodyPos[i - 1].first;
			snakeBodyPos[i].second = snakeBodyPos[i - 1].second;
		}
		snakeBodyPos[0].first = x;
		snakeBodyPos[0].second = y;
	}

	bool eatSelf() {
		if (snakeBodyPos.size() < 5) return false;

		for (int i = 1; i < snakeBodyPos.size(); i++) {
			if (x == snakeBodyPos[i].first && y == snakeBodyPos[i].second) {
				return true;
			}
		}
		return false;
	}

	void setDirection(int dir) {
		direction = dir;
	}

	void display() {
		gotoxy(x, y);
		cout << "*";
		for (int i = 0; i < snakeBodyPos.size(); i++) {
			gotoxy(snakeBodyPos[i].first, snakeBodyPos[i].second);
			cout << "*";
		}
	}

	void grow() {
		snakeBodyPos.push_back(make_pair(snakeBodyPos[snakeBodyPos.size() - 1].first, snakeBodyPos[snakeBodyPos.size() - 1].second));
		for (int i = snakeBodyPos.size() - 1; i >= 1; i--) {
			snakeBodyPos[i].first = snakeBodyPos[i - 1].first;
			snakeBodyPos[i].second = snakeBodyPos[i - 1].second;
		}
		snakeBodyPos[0].first = x;
		snakeBodyPos[0].second = y;
		length++;
	}

private:
	int x, y;
	int length;
	int direction;
	vector< pair<int, int> > snakeBodyPos;
};

class Fruit {
public:
	Fruit() {
		x = rand() % 48 + 1;
		y = rand() % 21 + 1;
	}

	int getFruitX() { return x; }
	int getFruitY() { return y; }

	void display() {
		gotoxy(x, y);
		cout << "$";
	}

	void getNewFruitPos() {
		x = rand() % 48 + 1;
		y = rand() % 21 + 1;
	}

private:
	int x, y;
};

class Game {
public:

	Game(int width, int height) {
		score = 0;
		this->width = width;
		this->height = height;
	}

	void run() {

		TimerMsec timer;
		timer.Reset();

		while (true) {

			system("cls");

			if (_kbhit()) {
				char ch = _getch();

				switch (ch)
				{
				case 'a':
					//cout << "a" << endl;
					if (snake.getDirection() != 2)
						snake.setDirection(4);
					break;
				case 'w':
					//cout << "w" << endl;
					if (snake.getDirection() != 3)
						snake.setDirection(1);
					break;
				case 's':
					//cout << "s" << endl;
					if (snake.getDirection() != 1)
						snake.setDirection(3);
					break;
				case 'd':
					//cout << "d" << endl;
					if (snake.getDirection() != 4)
						snake.setDirection(2);
					break;
				default:
					break;
				}
			}

			update();

			draw();

			if (timer.Get() > 16) {
				Sleep(160);
			}
			timer.Reset();
		}

	}

	void update() {
		snake.move();

		if (snake.eatSelf()) {
			cout << "Game Over" << endl;
			Sleep(1000);
			snake.setDirection(0);
		}

		if (snake.getHeadX() == fruit.getFruitX() && snake.getHeadY() == fruit.getFruitY()) {
			score++;
			snake.grow();
			fruit.getNewFruitPos();
		}

		if (snake.getHeadX() >= width || snake.getHeadX() <= 0) {
			cout << "OUT OF BOUND 1" << endl;
			Sleep(1000);
			snake.setDirection(0);
		}

		if (snake.getHeadY() >= height || snake.getHeadY() <= 0) {
			cout << "OUT OF BOUND 2" << endl;
			Sleep(1000);
			snake.setDirection(0);
		}

	}

	void draw() {
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (i == 0) cout << "#";
				else if (j == 0) cout << "#";
				else if (i == height - 1) cout << "#";
				else if (j == width - 1) cout << "#";
				else cout << " ";
			}
			cout << endl;
		}

		fruit.display();
		snake.display();

		gotoxy(0, 25);
		cout << "Score : " << score << endl;
		cout << snake.getHeadX() << " " << snake.getHeadY() << endl;
	}

private:
	int height, width;
	int score;
	Snake snake;
	Fruit fruit;
};

int main() {
	srand(time(NULL));
	Game game(50, 23);
	game.run();
	return 0;
}