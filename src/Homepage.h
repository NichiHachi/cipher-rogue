#include <SFML/Graphics.hpp>

class Slider {
private:
    sf::RectangleShape track;
    sf::CircleShape handle;
    int minValue;
    int maxValue;
    int value;

public:
    Slider();

    void setPosition(float x, float y);
    void setRange(int min, int max);
    void draw(sf::RenderWindow& window);

    // Add functions to handle mouse events and move the handle...
};

class Homepage {
private:
    sf::Font font;
    sf::Text title;
    sf::RectangleShape playButton;
    sf::RectangleShape quitButton;
    sf::Text playButtonText;
    sf::Text quitButtonText;
    Slider fpsSlider;

public:
    Homepage();

    void draw(sf::RenderWindow& window);
    int handleClick(sf::RenderWindow& window);
};