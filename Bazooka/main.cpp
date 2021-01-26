#include <SFML\Graphics.hpp>
#include <iostream>
#include <list>
#include <vector>
#include <bitset>
#include "Particles.h"
#include "GameObjects.h"

using namespace sf;
using namespace std;

constexpr float FRAME_RATE = 120.0f;

constexpr float GRAVITY = 0.005f;
constexpr float GROUND_HEIGHT = 5.0f * (float)HEIGHT / 9.0f;
const IntRect GROUND = IntRect(0, GROUND_HEIGHT, WIDTH, HEIGHT - GROUND_HEIGHT);

constexpr float PLAYER_SCALE = 1.0f;
constexpr float PLAYER_MAX_SPEED = 1.0f;

constexpr float ROCKET_SCALE = 1.0f;
constexpr float ROCKET_SPEED = 1.5f;

constexpr float EXPLOSION_SCALE = 1.0f;
constexpr float NUCLEAR_EXPLOSION_SPEED = 60.0f * 0.5f / FRAME_RATE;
constexpr float ROUNDED_EXPLOSION_SPEED = 60.0f * 1.0f / FRAME_RATE;

constexpr int NB_PARTICLES = 20000;
constexpr float PARTICLES_SPEED = 0.4f;
constexpr float PARTICLES_LIFETIME = 500.0f;
constexpr float PARTICLES_DRAG_COEFFICIENT = 0.02f;


bool moveParticles(Particles<NB_PARTICLES> &p, float dt, float gravity, float groundHeight, float dragCoeff) {
	for (int i = 0; i < NB_PARTICLES; i++) {
		p.speed[i].x *= (1.0f - dragCoeff);
		p.position[i].x += dt*p.speed[i].x;
		if (p.position[i].x < 0) p.position[i].x = WIDTH;
		if (p.position[i].x > WIDTH) p.position[i].x = 0;

		if (p.position[i].y > groundHeight) {
			p.position[i].y = groundHeight;
			if(p.speed[i].y > 0) p.speed[i].y *= -0.5f;
		}
		p.speed[i].y *= (1.0f - dragCoeff);
		p.position[i].y += p.speed[i].y*dt + 0.5f*gravity*dt*dt;
		p.speed[i].y += gravity*dt;

		p.points[i].position = p.position[i];
		p.points[i].color.a = 255 *(1.0f - p._time / p._lifeTime);
		p.points[i].color.g = 255 * (1.0f - powf((p._time / p._lifeTime), 1.75f));
	}
	p._time += dt;
	return p._time > p._lifeTime;
}

int main() {
	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Test");
	window.setFramerateLimit(FRAME_RATE)

	// Loading textures
	Texture t_background;
	if (!t_background.loadFromFile("Images/Backgrounds/street.jpg")) cout << "Failed to load background sprite (street) !" << endl;
	else cout << "Background (street) loading successful" << endl;

	Texture t_nuclear_explosion;
	if (!t_nuclear_explosion.loadFromFile("Images/Explosions/Nuclear Explosion.png")) cout << "Failed to load nuclear explosion sprite !" << endl;
	else cout << "nuclear explosion loading successful" << endl;
	
	Texture t_rounded_explosion;
	if (!t_rounded_explosion.loadFromFile("Images/Explosions/Rounded Explosion 3.png")) cout << "Failed to load rounded explosion sprite !" << endl;
	else cout << "rounded explosion loading successful" << endl;
	
	Texture t_rocket;
	if (!t_rocket.loadFromFile("Images/Rockets/Ammo.png")) cout << "Failed to load rocket sprite !" << endl;
	else cout << "rocket loading successful" << endl;

	Texture t_player;
	if (!t_player.loadFromFile("Images/Players/Player.png")) cout << "Failed to load player sprite !" << endl;
	else cout << "player loading successful" << endl;


	// Setting Animation
	// texture,( x, y ) : coord begin sprite sheet, Vector2i : nb sprite on sprite sheet (x and y), refresh rate
	Animation a_nuclear_explosion(t_nuclear_explosion, 0, 0, 320, 232, Vector2i(5, 5), NUCLEAR_EXPLOSION_SPEED, false);
	Animation a_rounded_explosion(t_rounded_explosion, 0, 0, 100, 100, Vector2i(9, 9), ROUNDED_EXPLOSION_SPEED, false);
	Animation a_rocket(t_rocket, 0, 132, 31, 21, Vector2i(1, 1), 0.0f, false);
	Animation a_player_still_right(t_player, 0, 0, 62, 75, Vector2i(1, 1), 0.0f, false);
	Animation a_player_still_left(t_player, 0, 0, 62, 75, Vector2i(1, 1), 0.0f, true);
	Animation a_player_go_right(t_player, 0, 75, 66, 75, Vector2i(8, 1), 0.250f, false);
	Animation a_player_go_left(t_player, 0, 75, 66, 75, Vector2i(8, 1), 0.250f, true);
	Animation a_player_air_right(t_player, 230, 230, 66, 75, Vector2i(1, 1), 0.0f, false);
	Animation a_player_air_left(t_player, 230, 230, 66, 75, Vector2i(1, 1), 0.0f, true);
	Animation a_player_die_right(t_player, 184, 0, 66, 75, Vector2i(2, 1), 0.01f, false);
	Animation a_player_die_left(t_player, 184, 0, 66, 75, Vector2i(2, 1), 0.01f, true);

	// arrays for our entities
	vector<Player> players;
	vector<Player*> deadPlayers;
	vector<Explosion*> explosions;
	vector<Rocket*> rockets;
	vector<Particles<NB_PARTICLES>*> vec_particles;

	// Sprite background
	t_background.setSmooth(true);
	Vector2u size = t_background.getSize();
	Sprite background(t_background);
	background.scale(Vector2f((float)WIDTH / (float)size.x, (float)HEIGHT / (float)size.y));

	// adding players
	Player p1(PLAYER_MAX_SPEED, GRAVITY, GROUND);
	p1.settings(a_player_still_right, WIDTH / 4, 0, 0.0f, Vector2u(35,65));
	players.push_back(p1);
	Player p2(PLAYER_MAX_SPEED, GRAVITY, GROUND);
	p2.settings(a_player_still_left, 3 * WIDTH / 4, 0, 0.0f, Vector2u(35, 65));
	players.push_back(p2);
	int currentPlayerState = ON_AIR | NO_DIRECTION;

	Clock time;
	float dt = 0.0000001f;
	Event evnt;
	while (window.isOpen()) {
		while (window.pollEvent(evnt)) {
			if (evnt.type == Event::Closed) {
				window.close();
			}
			// Ponctual actions
			if (evnt.type == Event::MouseButtonPressed) {
				if (evnt.key.code == Mouse::Left) {
					Vector2i mpos = Mouse::getPosition(window);
					Rocket *r = new Rocket(GROUND);
					r->settings(a_rocket);

					Vector2f ppos = players[0].getPosition();
					if (mpos.x > ppos.x) r->launch(ppos.x, ppos.y, atanf(-((float)mpos.y - ppos.y) / ((float)mpos.x - ppos.x)) / DEG_TO_RAD, ROCKET_SPEED);
					else r->launch(ppos.x, ppos.y, 180.0f + atanf(-((float)mpos.y - ppos.y) / ((float)mpos.x - ppos.x)) / DEG_TO_RAD, ROCKET_SPEED);
					rockets.push_back(r);
				}
			}
			if (evnt.type == Event::KeyPressed) {
				if (evnt.key.code == Keyboard::Space) {
					players[0].jump(PLAYER_MAX_SPEED,dt);
					if (currentPlayerState != players[0].getPlayerState()) {
						currentPlayerState = players[0].getPlayerState();
						currentPlayerState & RIGHT ? players[0].changeAnimation(a_player_air_right) : players[0].changeAnimation(a_player_air_left);
					}
				}
			}
		}
		// Continuous actions
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			players[0].move(PlayerMovement::MoveRight, dt);
			if (currentPlayerState != players[0].getPlayerState()) {
				currentPlayerState = players[0].getPlayerState();
				currentPlayerState & ON_GROUND ? players[0].changeAnimation(a_player_go_right) : players[0].changeAnimation(a_player_air_right);
			}
		}
		else if (Keyboard::isKeyPressed(Keyboard::Q)) {
			players[0].move(PlayerMovement::MoveLeft, dt);
			if (currentPlayerState != players[0].getPlayerState()) {
				currentPlayerState = players[0].getPlayerState();
				currentPlayerState & ON_GROUND ? players[0].changeAnimation(a_player_go_left) : players[0].changeAnimation(a_player_air_left);
			}
		}
		else {
			players[0].move(PlayerMovement::NoMove, dt);
			if (currentPlayerState != players[0].getPlayerState()) {
				currentPlayerState = players[0].getPlayerState();
				currentPlayerState & ON_GROUND ? currentPlayerState & RIGHT ? players[0].changeAnimation(a_player_still_right) : players[0].changeAnimation(a_player_still_left) : currentPlayerState & RIGHT ? players[0].changeAnimation(a_player_air_right) : players[0].changeAnimation(a_player_air_left);
			}
		}

		// Checking for collision
		for (auto i : rockets) {
			if (i->collidesWith(GROUND)) {
				i->setActionOnEntity(ActionOnEntity::RoundedExplosion);
			}
			else if (i->collidesWith(players[1].getHitBox()) && players.size() > 1) {
				i->setActionOnEntity(ActionOnEntity::NuclearExplosion);
				players[1].setActionOnEntity(ActionOnEntity::Die);
			}
		}

		// deadPlayers
		for (int i = 0; i < deadPlayers.size();) {
			deadPlayers[i]->setScale(PLAYER_SCALE);
			deadPlayers[i]->move(PlayerMovement::MoveRight, dt);
			deadPlayers[i]->update(dt);
			if (deadPlayers[i]->animationEnded() || GROUND.intersects(deadPlayers[i]->getHitBox())) {
				delete deadPlayers[i];
				deadPlayers.erase(deadPlayers.begin() + i);
			}
			else i++;
		}

		// Updating each sprite
		// players
		for (int i = 0; i < players.size();) {
			players[i].setScale(PLAYER_SCALE);
			switch (players[i].update(dt)) {
			case ActionOnEntity::Die: {
				Player *p = new Player(PLAYER_MAX_SPEED / 4.0f, GRAVITY, GROUND);
				p->settings(players[i].getPlayerState() & RIGHT ? a_player_die_right : a_player_die_left, players[i].getPosition().x, players[i].getPosition().y + 2, 90.0f, Vector2u(66, 75));
				p->jump(PLAYER_MAX_SPEED, dt);
				deadPlayers.push_back(p);
			}
			case ActionOnEntity::Disappear :
				players.erase(players.begin() + i);
				break;

			default :
				i++;
				break;

			}
		}

		// rockets
		for (int i = 0; i < rockets.size();) {
			Rocket *r = rockets[i];
			r->setScale(ROCKET_SCALE);
			switch (r->update(dt)) {
			case ActionOnEntity::Disappear: {
				delete r;
				rockets.erase(rockets.begin() + i);
				break;
			}
			case ActionOnEntity::RoundedExplosion: {
				Explosion *exp_rounded = new Explosion();
				exp_rounded->settings(a_rounded_explosion, r->getPosition().x, r->getPosition().y, 0.0f, Vector2u(50, 50));
				explosions.push_back(exp_rounded);
				vec_particles.push_back(new Particles<NB_PARTICLES>(r->getPosition().x, r->getPosition().y, PARTICLES_SPEED, PARTICLES_LIFETIME));
				rockets.erase(rockets.begin() + i);
				delete r;
				break;
			}
			case ActionOnEntity::NuclearExplosion: {
				Explosion *exp_nuclear = new Explosion();
				exp_nuclear->settings(a_nuclear_explosion, r->getPosition().x, r->getPosition().y, 0.0f, Vector2u(150,150));
				explosions.push_back(exp_nuclear);
				rockets.erase(rockets.begin() + i);
				delete r;
				break;
			}
			default:
				i++;
				break;
			}
		}

		// explosions
		for (int i = 0; i < explosions.size();) 
		{
			Explosion *e = explosions[i];
			switch (e->update(dt)) {
				case ActionOnEntity::Disappear: {
					explosions.erase(explosions.begin() + i);
					delete e;
					break;
				}
				default :
					i++;
					break;
			}			
		}

		// Particles
		for (int i = 0; i < vec_particles.size();) {
			if (moveParticles(*vec_particles[i], dt, GRAVITY / 10.0f, HEIGHT, PARTICLES_DRAG_COEFFICIENT)) {
				delete vec_particles[i];
				vec_particles.erase(vec_particles.begin() + i);
			}
			else i++;
		}

		// Drawing
		window.clear();
		window.draw(background);
		for (auto i : players) i.draw(window);
		for (auto i : deadPlayers) i->draw(window);
		for (auto i : rockets) i->draw(window);
		for (auto i : explosions) i->draw(window);
		for (auto i : vec_particles) window.draw(i->points, NB_PARTICLES, PrimitiveType::Points);
		// represnting ground
		RectangleShape ground(Vector2f(GROUND.width, GROUND.height));
		ground.setPosition(GROUND.left, GROUND.top);
		ground.setFillColor(Color(0, 255, 0, 127));
		//window.draw(ground);

		window.display();
		dt = time.restart().asMilliseconds();
	}

	for (auto i : rockets) {
		delete i;
	}
	for (auto i : explosions) {
		delete i;
	}
	return 0;
}
