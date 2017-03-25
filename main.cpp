#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#define WIDTH 600
#define HEIGHT 600
#define SIZE WIDTH/8

class vec2{
public:
	int x, y;
	vec2(){};
	vec2(int x, int y){
		this->x = x;
		this->y = y;
	};
};

int main(int argc, char** argv) {
	sf::RenderWindow screen(sf::VideoMode(WIDTH, HEIGHT), "Chess - made by hunar - hbkurd.weebly.com");
	screen.setFramerateLimit(24);
	
	sf::Image dashs_image;
	dashs_image.loadFromFile("sprites.png");
	
	sf::Texture dashes[12];
	for (int y = 0; y < 2; y++)
		for (int x = 0; x < 6; x++)
			dashes[6 * y + x].loadFromImage(dashs_image, sf::IntRect(x * SIZE, y * SIZE, SIZE, SIZE));
	
	
	// background
	sf::Texture back_tx;
	back_tx.loadFromFile("board.png");
	back_tx.setRepeated(true);
	sf::Sprite background;
	background.setTexture(back_tx);
	background.setTextureRect({ 0, 0, WIDTH, HEIGHT });
	
	bool should_redraw = true;
	while (screen.isOpen()) {
		
		sf::Event events;
		while (screen.pollEvent(events)) {
			if (events.type == sf::Event::Closed) {
				screen.close();
			}
			if (events.type == sf::Event::MouseButtonReleased) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					should_redraw = true;
				}
			}
		}
		
		if (should_redraw){
			screen.draw(background);
			
			sf::Sprite jun;
			jun.setTexture(dashes[5]);
			screen.draw(jun);
			
			should_redraw = false;
		}

		screen.display();
	}

	return 0;
}