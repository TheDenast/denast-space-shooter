#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

class Player {
public:
  int health;
  Player() : health(100) {}
};

class Enemy {
public:
  int health;
  int posx;
  int posy;
  Enemy() : health(100), posx(0), posy(0) {}
};

class Projectile {
public:
  sf::CircleShape shape;
  float speed = 300.0f; // Speed in pixels per second

  Projectile(float radius = 3.0f) : shape(radius) {
    shape.setFillColor(sf::Color::Yellow); // Example color
  }

  // Call this function every frame to update the projectile's position
  void update(sf::Time deltaTime) {
    // Move the projectile upwards
    shape.move(0, -speed * deltaTime.asSeconds());
  }
};

// Vectors storing recurring items:
std::vector<Projectile> projectiles;
std::vector<Enemy> enemies;

void shootFrom(const sf::Sprite &sprite) {
  Projectile newProjectile; // create new entry in Projectile vector
  sf::Vector2f startPosition = sprite.getPosition();

  // Adjust start position to sprite's center
  sf::FloatRect bounds = sprite.getLocalBounds();
  startPosition.x += bounds.width / 2.0f - newProjectile.shape.getRadius();
  // startPosition.y += bounds.height / 2.0f - newProjectile.shape.getRadius();
  startPosition.y += 0;

  newProjectile.shape.setPosition(startPosition);
  projectiles.push_back(newProjectile);
}

// stupid function that doesn't work
// * it works now, but still stupid
// TODO: make some sort of sprite manager
sf::Sprite makeSprite(const std::string &file) {
  static sf::Texture texture;
  texture.loadFromFile(file);
  sf::Sprite sprite(texture);

  return sprite;
}

// This function is used to update window ratio is its
// size is altered by the user
void updateView(const sf::RenderWindow &window, sf::View &view) {
  float windowRatio = float(window.getSize().x) / float(window.getSize().y);
  float viewRatio = float(view.getSize().x) / float(view.getSize().y);
  float sizeX = 1;
  float sizeY = 1;
  float posX = 0;
  float posY = 0;

  if (windowRatio < viewRatio) {
    sizeX = windowRatio / viewRatio;
    posX = (1 - sizeX) / 2.f;
  } else {
    sizeY = viewRatio / windowRatio;
    posY = (1 - sizeY) / 2.f;
  }

  view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
}

int main() {
  // Window initializations
  sf::RenderWindow window(sf::VideoMode(1024, 1024), "Denast Space Shooter");
  sf::View view(sf::FloatRect(0.f, 0.f, 1024.f, 1024.f)); // Original view
  window.setView(view);

  // Center the window on the screen
  sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
  window.setPosition(sf::Vector2i((desktop.width - window.getSize().x) / 2,
                                  (desktop.height - window.getSize().y) / 2));

  // Used to set frame limit
  // window.setFramerateLimit(60);

  sf::Texture background_tex;
  if (!background_tex.loadFromFile("assets/sprites/spacebg.png")) {
    return -1;
  }
  sf::Sprite background_spr(background_tex);
  background_spr.setPosition(0, 0);

  // sf::Texture player_texture;
  // if (!player_texture.loadFromFile("assets/sprites/player_c.png")) {
  //   return -1;
  // }

  // sf::Sprite player_sprite(player_texture);
  sf::Sprite player_sprite = makeSprite("assets/sprites/player_c.png");

  // Set initial position of the sprite at the center of the window
  player_sprite.setPosition(window.getSize().x / 2.0f,
                            window.getSize().y / 2.0f);

  sf::Texture enemy_texture;
  if (!enemy_texture.loadFromFile("assets/sprites/enemyship_o.png")) {
    return -1;
  }

  sf::Sprite enemy_sprite(enemy_texture);

  enemy_sprite.setPosition(window.getSize().x / 2.0f, 200);

  enemy_sprite.setScale(2, 2);

  sf::Font font;
  if (!font.loadFromFile("assets/fonts/transformers.ttf")) {
    return -1;
  }
  sf::Text text;
  text.setFont(font);                   // Set the font to our text object
  text.setString("You win!");           // Set the text to display
  text.setCharacterSize(40);            // Set the character size in pixels
  text.setFillColor(sf::Color::Yellow); // Set the text color
  text.setStyle(sf::Text::Bold);        // Set the text style
  text.setPosition(window.getSize().x / 2.0f, 0);

  // Start the clock
  // Used to make in-game time stable regardless
  // of loop speed
  sf::Clock clock;

  sf::Clock fire_clock;  // Measures time since last projectile was fired
  float fire_rate = 0.1; // Time in seconds between shots

  bool won = false;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // Resize window: adjust the view
      if (event.type == sf::Event::Resized) {
        view.setSize(event.size.width, event.size.height);
        updateView(window, view);
        window.setView(view);
      }
    }

    // Reset the clock, save the elapsed time into deltaTime
    sf::Time deltaTime = clock.restart();

    // Calculate player speed adjusted to deltaTime
    float speed = 300 * deltaTime.asSeconds();

    // This section allows player movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      // Move left
      player_sprite.move(-speed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      // Move right
      player_sprite.move(speed, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      // Move up
      player_sprite.move(0, -speed);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
      // Move down
      player_sprite.move(0, speed);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      if (fire_clock.getElapsedTime().asSeconds() >= fire_rate) {
        shootFrom(player_sprite); // Your existing function to fire a projectile
        fire_clock.restart();     // Reset the clock after firing
      }
    }
    for (auto &projectile : projectiles) {
      projectile.update(deltaTime);
      if (projectile.shape.getGlobalBounds().intersects(
              enemy_sprite.getGlobalBounds())) {
        won = true;
      }
    }

    window.clear(sf::Color::Black);
    window.draw(background_spr);
    window.draw(player_sprite);
    if (!won) {
      window.draw(enemy_sprite);
    } else {
      window.draw(text);
    }
    for (const auto &projectile : projectiles) {
      window.draw(projectile.shape);
    }
    window.display();
  }

  return 0;
}
