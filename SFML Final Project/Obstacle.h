#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

class obstacle {
public:
	sf::Texture obstacleTexture;
	sf::Sprite obstacleSprite;
	float yPos;
	float speed;

	obstacle();
	obstacle(sf::Vector2f position, sf::Texture* obstacleTexture);

	void update();
	void reposition();
	int randomizeYPos();
};

