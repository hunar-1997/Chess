#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define WIDTH 600
#define HEIGHT 600
#define SIZE WIDTH/8

// These are Kurdish names

enum Type_of_pice {
	pasha, wazir, fil, asp, qala, sarbaz, batal
}; // In English {king, queen, bishop, knight, rook, pawn, empty}

enum Color_of_pice {
	spi, rash
}; // In English {white, black}

class vec2 {
public:
	int x, y;

	vec2() {
	};

	vec2(int x, int y) {
		this->x = x;
		this->y = y;
	};
};

class piece {
public:
	int type;
	int color;

	piece() {
	};

	piece(int type, int color) {
		this->type = type;
		this->color = color;
	}
};

sf::Texture pieces[12];
piece board[8][8];

sf::Texture *get_texture(int type, int color) {
	return &pieces[type + color * 6];
}

int main(int argc, char** argv) {
	// Display stuff
	sf::RenderWindow screen(sf::VideoMode(WIDTH, HEIGHT), "Chess - made by hunar - hbkurd.weebly.com");
	screen.setFramerateLimit(24);
	// ----------------------------

	// Textures of the pices
	sf::Image pieces_image;
	pieces_image.loadFromFile("sprites.png");

	for (int y = 0; y < 2; y++)
		for (int x = 0; x < 6; x++)
			pieces[6 * y + x].loadFromImage(pieces_image, sf::IntRect(x * SIZE, y * SIZE, SIZE, SIZE));
	// ----------------------------

	// Creating the starting board
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			int color;
			if (i > 3) color = spi;
			else color = rash;

			if (i == 1) board[i][j] = piece(sarbaz, color);
			else if (i == 6) board[i][j] = piece(sarbaz, color);
			else if ( i==7 || i==0 ){
				int tp[] = {qala, asp, fil, wazir, pasha, fil, asp, qala};
				board[i][j] = piece(tp[j], color);
			}
			

			else board[i][j] = piece(batal, rash);
		}
	}
	// ----------------------------


	// background
	sf::Texture back_tx;
	back_tx.loadFromFile("board.png");
	back_tx.setRepeated(true);
	sf::Sprite background;
	background.setTexture(back_tx);
	background.setTextureRect({0, 0, WIDTH, HEIGHT});
	// ----------------------------

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

		if (should_redraw) {
			screen.draw(background);

			// Drawing the board
			sf::Sprite s;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					piece _this = board[i][j];
					if (_this.type != batal) {
						s.setTexture(*get_texture(_this.type, _this.color));
						s.setPosition(j*SIZE, i * SIZE);
						screen.draw(s);
					}
				}
			}
			// ---------------------------

			should_redraw = false;
		}

		screen.display();
	}

	return 0;
}