#include "MainMenuState.h"

MainMenuState::MainMenuState(RenderWindow* window, Font& font) :
	State(window, font)
{
	//初始化窗口的视野
	//init window's view
	View view = this->window->getView();
	view.setCenter(0, 0);
	this->window->setView(view);

	//初始化开始按钮
	//init start button and text
	startText.setFont(font);
	startText.setScale(2.f, 2.f);
	startText.setString(L"开始\n游戏");
	startText.setPosition(0.f, 0.f);
	startText.setFillColor(Color::Blue);
	Util::setCenter(startText);

	startButton.setSize(Vector2f(startText.getGlobalBounds().width, startText.getGlobalBounds().height));
	startButton.setPosition(startText.getPosition());
	startButton.setFillColor(Color::Black);
	Util::setCenter(startButton);
}

void MainMenuState::update(float dt)
{
	//window->clear();
	updateMouse();

}

void MainMenuState::draw(RenderStates states) const
{
	window->draw(startButton);
	window->draw(startText);
	window->display();
}
