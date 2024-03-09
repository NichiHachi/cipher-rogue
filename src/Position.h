#pragma once
class Position {
public:
    float x;
    float y;

    Position (float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    Position operator+(const Position& other) const {
        return Position(x + other.x, y + other.y);
    }

    Position operator-(const Position& other) const {
        return Position(x - other.x, y - other.y);
    }

    Position operator*(float scalar) const {
        return Position(x * scalar, y * scalar);
    }
    
    Position operator*(int scalar) const {
        return Position(x * scalar, y * scalar);
    }

    Position operator/(float scalar) const {
        return Position(x / scalar, y / scalar);
    }

    Position operator/(int scalar) const {
        return Position(x / scalar, y / scalar);
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
        x *= scalar;
        y *= scalar;
        return *this;
    }

    Position& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    Position& operator/=(int scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};
