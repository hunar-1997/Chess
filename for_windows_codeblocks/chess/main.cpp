#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

// Enums
enum Type_of_pice {
	pasha, wazir, fil, asp, qala, sarbaz, batal
}; // In English {king, queen, bishop, knight, rook, pawn, empty}

enum Color_of_pice {spi, rash}; // In English {white, black}

enum Cell_states {normal, this_piece, available, under_attack};

enum Game_states {stopped, paused, started, selected, won};
// ----------------------------

// Global variables
const int WIDTH = 600;
const int HEIGHT = 600;
const int SIZE = WIDTH/8;

sf::Texture pieces[12];
int game_state = stopped;
int turn = spi;
// ----------------------------

// Classes
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
	int state = normal;

	piece() {};

	piece(int type, int color) {
		this->type = type;
		this->color = color;
	}
};
// ----------------------------

// Global Methods
piece board[8][8];
// ----------------------------

// functions
sf::Texture *get_texture(int type, int color) {
	return &pieces[type + color * 6];
}

int get_type_at(vec2 pos, int x_off, int y_off){
	if (pos.x-x_off<0 || pos.x+x_off>7 || pos.y-y_off<0 || pos.y+y_off>7){
		return -1;
	}
	return board[pos.y-y_off][pos.x+x_off].type;
}

std::vector<vec2> available_moves(vec2 pos){	// I left the code here .. its very unstable
	std::vector<vec2> to_return;
	switch (board[pos.y][pos.x].type){
		case sarbaz:
			if (get_type_at(pos, 0, -1) != -1) to_return.push_back(vec2(pos.x+0, pos.y-1));
			if (get_type_at(pos, 0, -2) != -1) to_return.push_back(vec2(pos.x+0, pos.y-2));
			break;
	}
	return to_return;
}

vec2 current_press = vec2(0,0);
void move(vec2 pos){
	board[current_press.y][current_press.x].state = normal;
	current_press = pos;

	piece &cp = board[pos.y][pos.x];
	if (cp.type != batal){
		cp.state = this_piece;
		std::vector<vec2> av = available_moves(pos);
		for (int i=0; i<av.size(); i++)
			board[av[i].y][av[i].x].state = available;
	}
}

void toggle_turn(){
	turn = (turn==spi)?rash:spi;
}
// ----------------------------

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

	// Some handmade textures
	sf::Image av_img;	// available cell texture
	av_img.create(SIZE, SIZE);
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			av_img.setPixel(i, j, sf::Color(0, 243, 255, 150));
	sf::Image at_img;	// under attack cell texture
	at_img.create(SIZE, SIZE);
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			at_img.setPixel(i, j, sf::Color(255, 0, 0, 150));
	sf::Image th_img;	// selected cell texture
	th_img.create(SIZE, SIZE);
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			th_img.setPixel(i, j, sf::Color(0, 255, 0, 150));
	// ----------------------------

	// Creating the starting board
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++) {
			int color;
			if (i > 3) color = spi;
			else color = rash;

			if (i == 1) board[i][j] = piece(sarbaz, color);
			else if (i == 6) board[i][j] = piece(sarbaz, color);
			else if (i == 7 || i == 0) {
				int tp[] = {qala, asp, fil, wazir, pasha, fil, asp, qala};
				board[i][j] = piece(tp[j], color);
			} else board[i][j] = piece(batal, rash);
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
		// taking care about the mouse clicks
		sf::Event events;
		while (screen.pollEvent(events)) {
			if (events.type == sf::Event::Closed){
				screen.close();
			}
			if (events.type == sf::Event::MouseButtonReleased) {
				if (events.mouseButton.button == sf::Mouse::Left) {
					move( vec2(events.mouseButton.x/SIZE, events.mouseButton.y/SIZE) );

					should_redraw = true;
				}
			}
		}
		// ----------------------------

		// Do thins when we need to update the display
		if (should_redraw) {
			screen.draw(background);

			// Drawing the board
			sf::Sprite s;
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					piece _this = board[i][j];
					if (_this.state != normal) {
						sf::Texture tx;
						if (_this.state==available) tx.loadFromImage(av_img);
						if (_this.state==under_attack) tx.loadFromImage(at_img);
						if (_this.state==this_piece) tx.loadFromImage(th_img);
						s.setTexture(tx);
						s.setPosition(j*SIZE, i * SIZE);
						screen.draw(s);
					}
					if (_this.type != batal) {
						s.setTexture(*get_texture(_this.type, _this.color));
						s.setPosition(j*SIZE, i * SIZE);
						screen.draw(s);
					}
				}
			}
			// ---------------------------

			should_redraw = false;
			screen.display();
		}
		// ----------------------------
	}

	return 0;
}
