#include "MainMenuState.h"

MainMenuState::MainMenuState(RenderWindow* window, Font& font, stack<State*>* states) :
	State(window, font, states)
{
	//��ʼ�����ڵ���Ұ
	//init window's view
	View view = this->window->getView();
	view.setCenter(0, 0);
	this->window->setView(view);

	//��ʼ����ʼ��ť
	//init start button and text
	startText.setFont(font);
	startText.setScale(2.f, 2.f);
	startText.setString(L"��ʼ\n��Ϸ");
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
	updateMouse();
	if (startButton.getGlobalBounds().contains(mPos))
	{
		if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
		{
			states->push(new GameState(this->window, this->font, this->states));
		}
	}
}

void MainMenuState::draw(RenderStates states)
{
	window->draw(startButton);
	window->draw(startText);
}
