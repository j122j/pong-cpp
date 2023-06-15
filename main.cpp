#include "ArialTff.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

class Player
{
public:
  int x;
  int y;
  int width;
  int height;
  sf::RenderWindow &window;
  int score = 0;

  Player(int x, int y, int width, int height, sf::RenderWindow &win) : x(x), y(y), width(width), height(height), window(win)
  {
  }

  void draw()
  {
    sf::RectangleShape rectangle(sf::Vector2f(this->width, this->height));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(this->x, this->y);
    this->window.draw(rectangle);
  }

  void move(int delta)
  {
    this->y += delta;
    if (this->y < 0)
    {
      this->y = 0;
    }
    else if (this->y > 800 - this->height)
    {
      this->y = 800 - this->height;
    }
  }
};

class Ball
{
public:
  int x;
  int y;
  int width;
  int height;
  int xSpeed = 10;
  int ySpeed = 10;
  sf::RenderWindow &window;

  Ball(int x, int y, int width, int height, sf::RenderWindow &win) : x(x), y(y), width(width), height(height), window(win)
  {
  }

  void draw()
  {
    sf::RectangleShape rectangle(sf::Vector2f(this->width, this->height));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(this->x, this->y);
    this->window.draw(rectangle);
  }

  int update()
  {
    this->x += this->xSpeed;
    this->y += this->ySpeed;

    if (this->y < 0)
    {
      this->y = 0;
      this->ySpeed *= -1;
    }
    else if (this->y > 800 - this->height)
    {
      this->y = 800 - this->height;
      this->ySpeed *= -1;
    }

    if (this->x < 0)
    {
      return 2;
    }
    else if (this->x > 1200 - this->width)
    {
      return 1;
    }

    return 0;
  }

  void collides(Player &player)
  {
    if (this->x<player.x + player.width &&this->x + this->width> player.x && this->y<player.y + player.height &&this->y + this->height> player.y)
    {
      this->xSpeed *= -1;
    }
  }

  void reset()
  {
    this->x = 1200 / 2 - 25 / 2;
    this->y = 800 / 2 - 25 / 2;
    this->xSpeed = 10;
    this->ySpeed = 10;
  }
};

int main()
{
  sf::RenderWindow window(sf::VideoMode(1200, 800), "Pong!");
  window.setFramerateLimit(60);

  Player p1(75, 400 - 75, 25, 200, window);
  Player p2(1200 - 75 - 25, 400 - 75, 25, 200, window);
  Ball ball(1200 / 2 - 25 / 2, 800 / 2 - 25 / 2, 25, 25, window);

  sf::Font font;
  font.loadFromMemory(&arial_ttf, arial_ttf_len);

  bool paused = true;

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::Escape)
        {
          window.close();
        }
        else if (event.key.code == sf::Keyboard::N)
        {
          paused = !paused;
        }
      }
    }

    window.clear();

    if (!paused)
    {

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      {
        p1.move(-10);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      {
        p1.move(10);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
      {
        p2.move(-10);
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
      {
        p2.move(10);
      }

      int result = ball.update();
      if (result)
      {
        if (result == 1)
        {
          p1.score++;
        }
        else
        {
          p2.score++;
        }
        ball.reset();
      }
      ball.collides(p1);
      ball.collides(p2);
    }
    else
    {
      sf::Text text;
      text.setFont(font);
      text.setString("Press N to start");
      text.setCharacterSize(50);
      text.setFillColor(sf::Color::White);
      text.setPosition(1200 / 2 - text.getLocalBounds().width / 2, 800 / 2 - text.getLocalBounds().height / 2 - 200);
      window.draw(text);
    }

    p1.draw();
    p2.draw();
    ball.draw();

    sf::Text text;
    text.setFont(font);
    text.setString(std::to_string(p1.score) + " - " + std::to_string(p2.score));
    text.setCharacterSize(50);
    text.setFillColor(sf::Color::White);
    text.setPosition(1200 / 2 - text.getLocalBounds().width / 2, 0);
    window.draw(text);

    window.display();
  }
  return 0;
}