#pragma once

#include <SFML/Graphics.hpp>

/**
 * @brief Represents a slider control.
 */
class Slider {
private:
    sf::RectangleShape track;   
    sf::CircleShape handle;     
    int minValue;               
    int maxValue;               
    int value;                  

public:
    /**
     * @brief Default constructor for Slider class.
     */
    Slider();

    /**
     * @brief Sets the position of the slider.
     * @param x The x-coordinate of the slider's position.
     * @param y The y-coordinate of the slider's position.
     */
    void setPosition(float x, float y);

    /**
     * @brief Sets the range of the slider.
     * @param min The minimum value of the slider.
     * @param max The maximum value of the slider.
     */
    void setRange(int min, int max);

    /**
     * @brief Draws the slider on the specified window.
     * @param window The SFML render window to draw the slider on.
     */
    void draw(sf::RenderWindow& window);

    // Add functions to handle mouse events and move the handle...
};

/**
 * @brief Represents the homepage of the application.
 */
class Homepage {
private:
    sf::Font font;     
    sf::Texture titleTexture;
    sf::Sprite titleSprite;              
    sf::RectangleShape playButton; 
    sf::RectangleShape quitButton;  
    sf::Text playButtonText;       
    sf::Text quitButtonText;        
    Slider fpsSlider;               

public:
    /**
     * @brief Default constructor for Homepage class.
     */
    Homepage();

    /**
     * @brief Draws the homepage on the specified window.
     * @param window The SFML render window to draw the homepage on.
     */
    void draw(sf::RenderWindow& window);

    /**
     * @brief Handles the click event on the homepage.
     * @param window The SFML render window where the click event occurred.
     * @return The result of the click event handling.
     */
    int handleClick(sf::RenderWindow& window);
};