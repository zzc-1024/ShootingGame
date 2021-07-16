#include "State.h"

void State::updateMouse()
{
	this->mPos = this->window->mapPixelToCoords(Mouse::getPosition(*window));
}

State::State(RenderWindow* window, Font& font, stack<State*>* states)
{
	this->window = window;
	this->font = font;
	this->states = states;
}

void State::setFont(Font font)
{
	this->font = font;
}

Font State::getFont() const
{
	return this->font;
}

void State::setWindow(RenderWindow* window)
{
	this->window = window;
}

RenderWindow* State::getWindow() const
{
	return window;
}

void State::setEvent(Event event)
{
	this->event = event;
}

Event State::getEvent()
{
	return this->event;
}
