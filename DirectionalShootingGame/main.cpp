#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<math.h>

using namespace sf;

class Bullet {
public:
	CircleShape bullet;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float Radius = 5.f)
		: currVelocity(0.f, 0.f), maxSpeed(10.f)
	{
		this->bullet.setRadius(Radius);
		this->bullet.setFillColor(Color::Red);
	}
};

int main() {
	RenderWindow window(VideoMode(800, 600), "360 Shooting Game");
	window.setFramerateLimit(60);

	//Vector
	Vector2f playerCenter;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;

	//Player
	CircleShape player;
	player.setRadius(30.f);
	player.setFillColor(Color::Green);

	//Enemy
	RectangleShape enemy;
	enemy.setSize(Vector2f(30.f, 30.f));
	enemy.setFillColor(Color::Magenta);
	int spawnTimer = 0;

	std::vector<RectangleShape> enemies;
	enemies.push_back(RectangleShape(enemy));

	//Bullet
	Bullet bullet;
	
	std::vector<Bullet> bullets;
	bullets.push_back(Bullet(bullet));

	

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)){
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		//Update
			//vector
		playerCenter = Vector2f(player.getPosition().x + player.getRadius(), player.getPosition().y + player.getRadius());
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - playerCenter;
		aimDirNorm = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

			//player
		if (Keyboard::isKeyPressed(Keyboard::A))
			player.move(-10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::D))
			player.move(10.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::W))
			player.move(0.f, -10.f);
		if (Keyboard::isKeyPressed(Keyboard::S))
			player.move(0.f, 10.f);
			//enemy
		if (spawnTimer < 20) {
			spawnTimer++;
		}

		if (spawnTimer >= 20 && enemies.size() <= 20) {
			enemy.setPosition(rand() % window.getSize().x, rand() % window.getSize().y);
			enemies.push_back(RectangleShape(enemy));
			spawnTimer = 0;
		}
			//bullet
		if (Mouse::isButtonPressed(Mouse::Left)) {
			bullet.bullet.setPosition(playerCenter);
			bullet.currVelocity = aimDirNorm * bullet.maxSpeed;
			bullets.push_back(Bullet(bullet));
		}
		for (int i = 0; i < bullets.size(); i++) {
			bullets[i].bullet.move(bullets[i].currVelocity);
			//Out of Bounds
			if (bullets[i].bullet.getPosition().x < 0 || bullets[i].bullet.getPosition().x > window.getSize().x ||
				bullets[i].bullet.getPosition().y < 0 || bullets[i].bullet.getPosition().y > window.getSize().y) {
				bullets.erase(bullets.begin() + i);
			}
			else {
				for (int j = 0; j < enemies.size(); j++) {
					//bullet collisions with enemy
					if (bullets[i].bullet.getGlobalBounds().intersects(enemies[j].getGlobalBounds())) {
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + j);
					}
				}
			}
		}

		//Draw
		window.clear();

		for (int i = 0; i < enemies.size(); i++) {
			window.draw(enemies[i]);
		}
		window.draw(player);
		for (int i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].bullet);
		}

		window.display();
	}

	return 0;
}