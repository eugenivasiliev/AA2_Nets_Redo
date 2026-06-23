#include <iostream>
#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "NetworkManager.h"

int main() {

	if (!NT->Init()) return -1;

	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ WINDOW_WIDTH, WINDOW_HEIGHT }), WINDOW_NAME);

	SceneManager sm = SceneManager();
	while (sm.update(*window));

	delete window;
}