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

int rtg(int x, int y){	// relative to global position
	return x - 8*y;
}

piece *get_board(int x, int y){
	if (turn==rash) x = 7-x;
	return &board[x][y];
}

void toggle_turn(){
	turn = (turn==spi)?rash:spi;
}

int valid(int index, vec2 offset, vec2 pos){	// 0:dont do	1:doit	2:do it but break afterwards
	if (pos.x + offset.x > 7 || pos.x + offset.x < 0 || pos.y+offset.y > 7 || pos.y + offset.y < 0) return 0;
	int type = get_board(index/8, index%8)->type;
	if (type==batal)
		return 1;
	if (type!=batal) {
		if (get_board(index / 8, index % 8)->color != turn)
			return 2;
	}
}

std::vector<int> available_moves(vec2 pos){
	std::vector<int> to_return;
	
	int index = pos.y*8 + pos.x;
	
	int type = get_board(pos.y, pos.x)->type;
	
	if (type == sarbaz){
		for (int i=1; i<=2; i++){
			vec2 offset(0, i);
			int current = rtg(offset.x, offset.y);
			
			int ind = index + current;
			
			int v = valid(ind, offset, pos);
			if (v==1 || v==2){
				to_return.push_back(index + current);
				if (v==2) break;
			}
		}
	}
	if (type == qala || type == wazir || type==pasha){
		for (int bar = -1; bar<=0; bar++){
			for (int arasta=-1; arasta<=1; arasta+=2){
				for (int i=arasta;; i+=arasta){
					vec2 offset((bar+1)*i, -bar*i);
					int current = rtg(offset.x, offset.y);
					
					int ind = index + current;

					int v = valid(ind, offset, pos);
					if (v == 1 || v == 2) {
						to_return.push_back(index + current);
						if (v == 2) break;
					}else break;
					
					if (type==pasha) break;
				}
			}
		}
	}
	if (type == fil || type == wazir || type==pasha){
		for (int bar = -1; bar<=1; bar+=2){
			for (int arasta=-1; arasta<=1; arasta+=2){
				for (int i=arasta;; i+=arasta){
					vec2 offset(i, bar*i);
					int current = rtg(offset.x, offset.y);
					
					int ind = index + current;

					int v = valid(ind, offset, pos);
					if (v == 1 || v == 2) {
						to_return.push_back(index + current);
						if (v == 2) break;
					}else break;
					
					if (type==pasha) break;
				}
			}
		}
	}
	if (type == asp) {
		for (int aso = -1; aso <= 1; aso+=2){
			for (int stun = -1; stun <= 1; stun += 2) {
				for (int i = 1; i <= 2; i++) {
					vec2 offset(aso*(3-i), stun*i);
					int current = rtg(offset.x, offset.y);

					int ind = index + current;

					int v = valid(ind, offset, pos);
					if (v == 1 || v == 2) {
						to_return.push_back(index + current);
						if (v == 2) break;
					} else break;
				}
			}
		}
	}
	return to_return;
}

void move(vec2 pos){
	piece *this_cell = get_board(pos.y, pos.x);
	if (this_cell->color == turn){
		if (this_cell->type != batal){
			this_cell->state = this_piece;
			std::vector<int> av = available_moves(pos);
			for (int i=0; i<av.size(); i++){
				int st = available;
				if (get_board(av[i]/8, av[i]%8)->type!=batal)
					st = under_attack;
				get_board(av[i]/8, av[i]%8)->state = st;
			}
		}
	}
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
	board[3][4].type = asp;
	board[3][4].color = spi;
	board[6][6].type = asp;
	board[6][6].color = spi;
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
			screen.clear();
			screen.draw(background);

			// Drawing the board
			sf::Sprite s;
			
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					piece _this = *get_board(i, j);
					if (_this.state != normal) {
						sf::Texture tx;
						if (_this.state==available) tx.loadFromImage(av_img);
						if (_this.state==under_attack) tx.loadFromImage(at_img);
						if (_this.state==this_piece) tx.loadFromImage(th_img);
						s.setTexture(tx);
						s.setPosition(j*SIZE, i * SIZE);
						get_board(i, j)->state = normal;
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