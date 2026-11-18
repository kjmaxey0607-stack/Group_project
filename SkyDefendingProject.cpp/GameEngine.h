#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <QString>
#include <QRect>
#include <QColor>

// Base Enemy class using polymorphism
class Enemy {
protected:
    int x, y;
    int width, height;
    int speed;
    bool active;

public:
    Enemy(int startX, int startY, int w = 36, int h = 28, int spd = 2);
    virtual ~Enemy() = default;

    virtual void move() = 0; // Pure virtual function for polymorphic movement
    virtual QColor getColor() const;

    QRect getBounds() const;
    int getY() const;
    bool isActive() const;
    void destroy();
};

class StandardEnemy : public Enemy {
public:
    StandardEnemy(int startX, int startY);
    void move() override;
};

class FastEnemy : public Enemy {
private:
    int dir;
public:
    FastEnemy(int startX, int startY);
    void move() override;
    QColor getColor() const override;
};

struct Projectile {
    int x, y;
    int width = 6;
    int height = 16;
    int speed = 10;
    bool active = true;

    QRect getBounds() const;
};

// Core Game Logic & Rules (Independent of GUI)
class SkyDefendingGame {
public:
    static constexpr int ARENA_WIDTH = 780;
    static constexpr int ARENA_HEIGHT = 460;
    static constexpr int PLAYER_WIDTH = 44;
    static constexpr int PLAYER_HEIGHT = 36;
    static constexpr int PLAYER_SPEED = 20;

    int playerX;
    int playerY;
    int score;
    int bestScore;
    int lives;
    bool isRunning;
    bool isPaused;
    bool isGameOver;
    QString statusMessage;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<Projectile> shots;
    int spawnCounter;

    SkyDefendingGame();

    void resetPositions();
    bool start();
    void restart();
    void togglePause();
    bool movePlayer(int direction);
    bool fire();
    void spawnWave();
    void updateGameState();
};