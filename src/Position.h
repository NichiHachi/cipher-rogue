#pragma once
struct Position {
    float x;
    float y;

    Position() : x(0.0f), y(0.0f) {}
    Position (float x, float y) : x(x), y(y) {}
    Position (int x, int y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}
    Position (double x, double y) : x(static_cast<float>(x)), y(static_cast<float>(y)) {}

    Position operator+(const Position& other) const {
        return {x + other.x, y + other.y};
    }

    Position operator-(const Position& other) const {
        return {x - other.x, y - other.y};
    }

    Position operator*(float scalar) const {
        return {x * scalar, y * scalar};
    }

    Position operator*(double scalar) const {
        return {x * scalar, y * scalar};
    }
    
    Position operator*(int scalar) const {
        return {x * static_cast<float>(scalar), y * static_cast<float>(scalar)};
    }

    Position operator/(float scalar) const {
        if(scalar == 0) throw std::invalid_argument("Division by zero");
        return {x / scalar, y / scalar};
    }

    Position operator/(double scalar) const {
        if(scalar == 0) throw std::invalid_argument("Division by zero");
        return {x / scalar, y / scalar};
    }

    Position operator/(int scalar) const {
        if(scalar == 0) throw std::invalid_argument("Division by zero");
        return {x / static_cast<float>(scalar), y / static_cast<float>(scalar)};
    }

    Position& operator+=(const Position& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Position& operator-=(const Position& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Position& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    } 
    
    Position& operator*=(int scalar) {
        x *= static_cast<float>(scalar);
        y *= static_cast<float>(scalar);
        return *this;
    }

    Position& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Position& operator/=(int scalar) {
        if(scalar == 0) throw std::invalid_argument("Division by zero");
        x /= static_cast<float>(scalar);
        y /= static_cast<float>(scalar);
        return *this;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Position& other) const {
        return x != other.x || y != other.y;
    }

    bool operator<(const Position& other) const {
        return y*y+x*x < other.y*other.y+other.x*other.x;
    }

};
