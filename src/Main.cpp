#ifndef _DEBUG
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif
constexpr int FAIL_TO_FIND_RES		= 1 << 0;
constexpr int UNKNOWN_ERROR			= -1;

//#include "SFML/Graphics.hpp"
//#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "jsonxx/json.hpp"
#include "chr/Player.h"
#include "chr/AI.h"
#include "bullet.h"
#include "state/MainMenuState.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <list>
#include <stdlib.h>

#include <vld.h>

using namespace sf;
using namespace std;
using namespace jsonxx;

int width	= 800;
int height	= 600;

RenderWindow* window;
Texture texture, fireBullet;
Font font;
Clock clk;

Sound shoot;

void menu(int score);

int run();

int main()
{
	json j;
	try
	{
		ifstream ifm("res/data.json");
		ifm >> j;
		Player::globalHP = j["player"]["hp"].as_integer();
		Player::globalAttack = j["player"]["attack"].as_integer();
		Player::globalSpeed = j["player"]["speed"].as_float();
		Player::globalCD = j["player"]["cd"].as_float();
		AI::globalHP = j["AI"]["hp"].as_integer();
		AI::globalAttack = j["AI"]["attack"].as_integer();
		AI::globalSpeed = j["AI"]["speed"].as_float();
#ifdef _DEBUG
		clog << "Read resource successfully!" << endl;
#endif
	}
	catch (json_deserialization_error e)
	{
#ifdef _DEBUG
		clog << "Failed to read resource" << endl;
#endif
		ofstream ofm("res/data.json");
		j["player"]["hp"] = Player::globalHP;
		j["player"]["attack"] = Player::globalAttack;
		j["player"]["speed"] = Player::globalSpeed;
		j["player"]["cd"] = Player::globalCD;
		j["AI"]["hp"] = AI::globalHP;
		j["AI"]["attack"] = AI::globalAttack;
		j["AI"]["speed"] = AI::globalSpeed;
		try
		{
			ofm << setw(4) << j;
		}
		catch (json_deserialization_error e)
		{
			system("mkdir res");
			ofm << setw(4) << j;
			return FAIL_TO_FIND_RES;
		}
		catch (...)
		{
			return UNKNOWN_ERROR;
		}
	}
	catch (...)
	{
		return UNKNOWN_ERROR;
	}

	if (!texture.loadFromFile("res/Tileset.png"))
	{
#ifdef _DEBUG
		cerr << "Failed to open tile";
#endif
		return FAIL_TO_FIND_RES;
	}

	if (!fireBullet.loadFromFile("res/fireball.png"))
	{
#ifdef _DEBUG
		cerr << "Failed to open fire ball tile";
#endif
		return FAIL_TO_FIND_RES;
	}

	Music music;
	if (!music.openFromFile("res/game_music.wav"))
	{
#ifdef _DEBUG
		cerr << "Failed to open back ground music";
#endif
		return FAIL_TO_FIND_RES;
	}
	music.setLoop(true);
	music.play();

	SoundBuffer shootBuffer;
	if (!shootBuffer.loadFromFile("res/shoot.wav"))
	{
#ifdef _DEBUG
		cerr << "Failed to open back ground music";
#endif
		return FAIL_TO_FIND_RES;
	}
	shoot.setBuffer(shootBuffer);

	window = new RenderWindow(VideoMode(width, height), L"射击游戏",/*Style::Fullscreen*/ Style::Close | Style::Titlebar);
	window->setVerticalSyncEnabled(true);
	View view = window->getView();
	view.setCenter(0, 0);
	window->setView(view);
	font.loadFromFile("res/simsun.ttc");
	srand(time(0));
	int score = -1;
	
	stack<State*> states;
	states.push(new MainMenuState(window, font, &states));
	Clock clk;
	float dt;
	clk.restart();
	Event event;
	while (window->isOpen())
	{
		window->clear(Color::White);

		dt = clk.getElapsedTime().asSeconds();
		clk.restart();
		
		states.top()->update(dt);
		states.top()->draw();
		window->display();

		while (window->pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window->close();
				while (!states.empty())
				{
					delete states.top();
					states.pop();
				}
				break;
			}
			states.top()->setEvent(event);
		}
	}


	while (window->isOpen())
	{
		menu(score);
		if (!window->isOpen())
			break;
		clk.restart();
		score = run();

	}
	delete window;

	return 0;
}

void menu(int score) 
{
	Text text;
	text.setFont(font);
	text.setString(L"开始\n游戏");
	text.setScale(2, 2);
	Util::setCenter(text);

	View view = window->getView();
	view.setCenter(0, 0);
	window->setView(view);
	wstringstream wss;
	wss << L"最终得分为:" << score;
	wstring info;
	wss >> info;
	Text show;
	show.setString(info);
	show.setFont(font);
	Util::setCenter(show);
	show.setPosition(0, -height / 4);
	show.setFillColor(Color::Black);
	while (true)
	{
		RectangleShape rect;
		rect.setFillColor(Color::Black);
		rect.setSize(Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
		rect.setOrigin(rect.getSize().x / 2, rect.getSize().y / 2);
		rect.setPosition(text.getPosition());
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
		if (score >= 0)
			window->draw(show);
		window->display();
	}
}

int run()
{
	Player player;
	player.shape.setPosition(width / 2, height / 2);
	player.loadTexture(texture, 192, 106, 16, 22, 4);

	list<AI> v;
	list<Bullet> b;
	AI ai;
	ai.loadTexture(texture, 432, 80, 16, 16, 4);

	ai.shape.setPosition(-width / 2, rand() % 600);
	v.push_back(ai);

	ai.shape.setPosition(width * 1.5, rand() % 600);
	v.push_back(ai);

	ai.shape.setPosition(rand() % 800, -height / 2);
	v.push_back(ai);

	ai.shape.setPosition(rand() % 800, height * 1.5);
	v.push_back(ai);

	int score = 0;
	float dt;
	float flushAI = 0.f;

	View view = window->getView();

	const wstring showScore = L"当前得分:", showFPS = L"FPS:" ,showHP = L"生命值";

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
				return score;
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
			//window->close();
			return score;
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

		//AI进行移动
		for (auto& it : v)
		{
			Vector2f pos;
			pos = it.shape.getPosition();
			float dx = ppos.x - pos.x;
			if (dx >= 0)
				it.isReverse = false;
			else
				it.isReverse = true;
			float dy = ppos.y - pos.y;
			float angle;
			if (dx == 0)
				angle = pi / 2;
			else
				angle = abs(atan(dy / dx));

			it.shape.move(
				dx / abs(dx) * it.speed * cos(angle) * dt, 
				dy / abs(dy) * it.speed * sin(angle) * dt
			);
		}

		//发射子弹
		player.cdRemain -= dt;
		Vector2f posView;
		posView = window->mapPixelToCoords(Mouse::getPosition(*window));
		float dx = posView.x - ppos.x;
		float dy = posView.y - ppos.y;
		if (dx >= 0)
			player.isReverse = false;
		else
			player.isReverse = true;
		if (player.cdRemain < 0.f)
			player.cdRemain = 0.f;
		if (player.cdRemain == 0.f && Mouse::isButtonPressed(Mouse::Left))
		{
			shoot.play();
			player.cdRemain = player.cd;
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
			Bullet tmp;
			tmp.angle = angle;
			tmp.loadTexture(fireBullet, 0, 0, 26, 10, 10);
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
					b.erase(it++);
					flag = false;
					tmp->hp -= player.attack;
					if (tmp->hp <= 0)
					{
						v.erase(tmp++);
						score++;
					}
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
				player.hp -= it->attack;
				v.erase(it++);
				if (player.hp <= 0)
					return score;
			}
			else
				it++;
		}

		//ai.resetColor();

		flushAI += dt;
		while (flushAI >= 0.1f)
		{
			flushAI -= 0.1f;
			ai.shape.setPosition(-width / 2, rand() % 600);
			if (rand() % 1000 < 10 + score)
				v.push_back(ai);
			ai.shape.setPosition(width * 1.5, rand() % 600);
			if (rand() % 1000 < 10 + score)
				v.push_back(ai);
			ai.shape.setPosition(rand() % 800, -height / 2);
			if (rand() % 1000 < 10 + score)
				v.push_back(ai);
			ai.shape.setPosition(rand() % 800, height * 1.5);
			if (rand() % 1000 < 10 + score)
				v.push_back(ai);
		}

		//更新显示的信息
		wstringstream wss;
		wstring showInfo;
		wss << setprecision(0) << fixed 
			<< L"当前得分:" << score << L"\n" 
			<< showFPS << 1 / dt << L'\n' 
			<< showHP << player.hp;
		wchar_t c;
		while ((c = wss.get()) != WEOF)
			showInfo += c;

		Text info;
		info.setString(showInfo);
		info.setFont(font);
		info.setFillColor(Color::Black);
		info.setPosition(ppos.x - width / 2, ppos.y - height / 2);
		info.setOrigin(info.getLocalBounds().left, info.getLocalBounds().top);

		window->clear(Color::Magenta);

		RectangleShape rect(Vector2f(width, height));
		rect.setFillColor(Color::White);
		rect.setOutlineThickness(2);
		rect.setOutlineColor(Color::Black);
		window->draw(rect);

		//window->draw(player.shape);
		player.update(dt);
		window->draw(player);
		//for (auto& it : v)
		//	window->draw(it.shape);
		for (auto& it : v)
		{
			it.update(dt);
			window->draw(it);
		}
		for (auto& it : b)
			window->draw(it.shape);
		for (auto& it : b)
			window->draw(it.updateState(dt));
		window->draw(info);

		window->display();
	}
}
