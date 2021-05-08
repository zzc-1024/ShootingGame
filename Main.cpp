#include "SFML/Graphics.hpp"
//#include "SFML/Network.hpp"
//#include "SFML/Audio.hpp"
#ifndef _DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <stdlib.h>
#include <cmath>

#include <vld.h>

using namespace sf;
using namespace std;

constexpr int width = 800;
constexpr int height = 600;
const double pi = acos(-1);

RenderWindow* window;
Font font;
Text text;
Clock clk;

class object;
class bullet;

void menu();

void run();

int main()
{
	window = new RenderWindow(VideoMode(width, height), L"射击游戏", Style::Close | Style::Titlebar);
	window->setVerticalSyncEnabled(true);
	View view = window->getView();
	view.setCenter(0, 0);
	window->setView(view);
	font.loadFromFile("simsun.ttc");
	text.setFont(font);
	text.setString(L"开始\n游戏");
	text.setScale(2, 2);
	FloatRect lb = text.getLocalBounds();
	text.setOrigin(lb.left + lb.width / 2, lb.top + lb.height / 2);
	srand(time(0));
	while (window->isOpen())
	{
		menu();
		if (!window->isOpen())
			break;
		clk.restart();
		run();
	}
	delete window;

	return 0;
}


class object
{
public:
	int hp;
	int attack;
	float speed;
	float cd;
	CircleShape shape;

	object()
	{
		hp = 10;
		attack = 1;
		speed = 100;
		cd = 0.1;
		shape.setRadius(20.f);
		shape.setOrigin(Vector2f(20.f, 20.f));
		shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	}
};

class bullet
{
public:
	CircleShape shape;
	float speed;
	float life;
	float angle;

	bullet()
	{
		life = 1.f;
		speed = 300;
		shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
		shape.setRadius(5.f);
		shape.setOrigin(5.f, 5.f);
		angle = 0.f;
	}
};

void menu()
{
	View view = window->getView();
	view.setCenter(0, 0);
	window->setView(view);
	while (true)
	{
		Font font;
		RectangleShape rect;
		rect.setFillColor(Color::Black);
		rect.setSize(Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
		rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
		rect.setPosition(text.getPosition());
		font.loadFromFile("simsun.ttc");
		wstringstream sstring;
		Vector2f pos = window->mapPixelToCoords(Mouse::getPosition(*window));
		wstring s;
		sstring >> s;
		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window->close();
				return;
			}
			if (event.type == Event::MouseButtonReleased)
				if (event.mouseButton.button == Mouse::Left)
					if (rect.getGlobalBounds().contains(pos))
						return;
		}
		window->clear(Color(Color::White));
		text.setPosition(0, 0);
		text.setFillColor(Color::Blue);
		window->draw(rect);
		window->draw(text);
		window->display();
	}
}

void run()
{
	object player;
	player.shape.setPosition(400.f, 300.f);

	vector<object> v;
	list<bullet> b;

	int score = 0;
	float dt;

	View view = window->getView();

	const wstring showScore = L"当前得分:", showFPS = L"FPS:";

	while (player.hp > 0)
	{
		dt = clk.getElapsedTime().asSeconds();
		clk.restart();

		Event event;
		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window->close();
				return;
			}
		}

		//player响应移动指令
		if (Keyboard::isKeyPressed(Keyboard::A))
			player.shape.move(Vector2f(-player.speed * dt, 0));
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.shape.move(Vector2f(player.speed * dt, 0));
		if (Keyboard::isKeyPressed(Keyboard::W))
			player.shape.move(Vector2f(0, -player.speed * dt));
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.shape.move(Vector2f(0, player.speed * dt));
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window->close();
			return;
		}

		Vector2f ppos = player.shape.getPosition();

		if (ppos.x < 0)
			ppos.x = 0;
		else if (ppos.x > width)
			ppos.x = width;
		if (ppos.y < 0)
			ppos.y = 0;
		else if (ppos.y > height)
			ppos.y = height;

		player.shape.setPosition(ppos);
		view.setCenter(ppos);
		window->setView(view);

		for (auto& it : v)
		{
			Vector2f pos;
			pos = it.shape.getPosition();
			float dx = ppos.x - pos.x;
			float dy = ppos.y - pos.y;
			float angle;
			if (dx == 0)
				angle = pi / 2;
			else
				angle = abs(atan(dy / dx));

			it.shape.move
			(dx / abs(dx) * it.speed * cos(angle) * dt, dy / abs(dy) * it.speed * sin(angle) * dt);
		}

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			Vector2f posView;
			posView = window->mapPixelToCoords(Mouse::getPosition(*window));
			float dx = posView.x - ppos.x;
			float dy = posView.y - ppos.y;
			float angle;
			if (dx == 0)
				if (dy > 0)
					angle = pi / 2;
				else
					angle = -pi / 2;
			else
			{
				angle = atan(dy / dx);
				if (dx < 0)
					angle = angle + pi;
			}
			bullet tmp;
			tmp.angle = angle;
			tmp.shape.setPosition(ppos);
			b.push_back(tmp);
		}

		while (!b.empty() && b.front().life <= 0)
			b.pop_front();

		//更新子弹
		for (auto it = b.begin(); it != b.end();)
		{
			it->shape.move(it->speed * cos(it->angle) * dt, it->speed * sin(it->angle) * dt);
			it->life -= dt;
			bool flag = true;
			//检测子弹是否碰到敌人
			for (auto tmp = v.begin(); tmp != v.end();)
			{
				if (it->shape.getGlobalBounds().intersects(tmp->shape.getGlobalBounds()))
				{
					v.erase(tmp++);
					b.erase(it++);
					score++;
					flag = false;
					break;
				}
				else
				{
					tmp++;
				}
			}
			if (flag)
				it++;
		}

		//检测敌人是否碰到玩家
		for (auto it = v.begin(); it != v.end();)
		{
			if (it->shape.getGlobalBounds().intersects(player.shape.getGlobalBounds()))
			{
				//window->close();
				return;
			}
			it++;
		}

		object ai;

		ai.shape.setPosition(0.f, rand() % 600);

		if (rand() % 1000 < 100)
			v.push_back(ai);

		//更新显示的信息
		wstringstream wss;
		wstring showInfo;
		wss << setprecision(0) << fixed << L"当前得分:" << score << L"\n" << showFPS << 1 / dt;
		wchar_t c;
		while ((c = wss.get()) != WEOF)
			showInfo += c;

		Text info;
		info.setString(showInfo);
		info.setFont(font);
		info.setFillColor(Color::Black);
		info.setPosition(ppos.x - width / 2, ppos.y - height / 2);
		info.setOrigin(info.getLocalBounds().left, info.getLocalBounds().top);

		window->clear(Color::White);

		RectangleShape rect(Vector2f(width, height));
		rect.setFillColor(Color::White);
		rect.setOutlineThickness(1);
		rect.setOutlineColor(Color::Black);
		window->draw(rect);

		window->draw(player.shape);
		for (auto& it : v)
			window->draw(it.shape);
		for (auto& it : b)
			window->draw(it.shape);
		window->draw(info);

		window->display();
	}
}
