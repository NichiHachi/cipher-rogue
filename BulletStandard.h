#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Position.h"
#include "Bullet.h"

class BulletStandard : public Bullet {
    public:
        BulletStandard(Position position, float angle, float speed, float size, bool ally, bool destructible);
        void update(const float deltaTime) override;
        void draw(sf::RenderWindow &window) override;
        bool isDeletable() const override;

        std::string getType() const override { return "Standard"; };
};