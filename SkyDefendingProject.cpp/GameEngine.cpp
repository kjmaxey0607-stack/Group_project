#include "GameEngine.h"

// --- Enemy Implementation ---
Enemy::Enemy(int startX, int startY, int w, int h, int spd)
    : x(startX), y(startY), width(w), height(h), speed(spd), active(true) {
}

QColor Enemy::getColor() const {
    return QColor(180, 70, 255);
}

QRect Enemy::getBounds() const {
    return QRect(x, y, width, height);
}

int Enemy::getY() const {
    return y;
}

bool Enemy::isActive() const {
    return active;
}

void Enemy::destroy() {
    active = false;
}

// --- StandardEnemy Implementation ---
StandardEnemy::StandardEnemy(int startX, int startY) : Enemy(startX, startY, 36, 28, 2) {}

void StandardEnemy::move() {
    y += speed;
}

// --- FastEnemy Implementation ---
FastEnemy::FastEnemy(int startX, int startY) : Enemy(startX, startY, 32, 26, 3), dir(1) {}

void FastEnemy::move() {
    y += speed;
    x += dir * 2;
    if (x <= 10 || x >= 740) {
        dir = -dir;
    }
}

QColor FastEnemy::getColor() const {
    return QColor(255, 90, 90);
}

// --- Projectile Implementation ---
QRect Projectile::getBounds() const {
    return QRect(x, y, width, height);
}

// --- SkyDefendingGame Implementation ---
SkyDefendingGame::SkyDefendingGame()
    : score(0), bestScore(0), lives(3), isRunning(false),
    isPaused(false), isGameOver(false), spawnCounter(0) {
    resetPositions();
    statusMessage = "Press Start to begin defending the sky.";
}

void SkyDefendingGame::resetPositions() {
    playerX = (ARENA_WIDTH - PLAYER_WIDTH) / 2;
    playerY = ARENA_HEIGHT - PLAYER_HEIGHT - 12;
    enemies.clear();
    shots.clear();
    spawnCounter = 0;
}

bool SkyDefendingGame::start() {
    if (isRunning && !isGameOver) {
        statusMessage = "Warning: A game is already running! Use Pause or Restart.";
        return false;
    }
    score = 0;
    lives = 3;
    isRunning = true;
    isPaused = false;
    isGameOver = false;
    resetPositions();
    spawnWave();
    statusMessage = "Game started! Use Left/Right arrows to move and Space to shoot.";
    return true;
}

void SkyDefendingGame::restart() {
    isRunning = false;
    isGameOver = false;
    start();
    statusMessage = "Game restarted.";
}

void SkyDefendingGame::togglePause() {
    if (!isRunning || isGameOver) {
        statusMessage = "Cannot pause: No active game in progress.";
        return;
    }
    isPaused = !isPaused;
    statusMessage = isPaused ? "Game Paused." : "Game Resumed.";
}

bool SkyDefendingGame::movePlayer(int direction) {
    if (!isRunning || isPaused || isGameOver) {
        statusMessage = "Start the game before moving the defender.";
        return false;
    }
    int nextX = playerX + (direction * PLAYER_SPEED);
    if (nextX < 0) {
        playerX = 0;
        statusMessage = "Reached left boundary.";
        return false;
    }
    if (nextX > ARENA_WIDTH - PLAYER_WIDTH) {
        playerX = ARENA_WIDTH - PLAYER_WIDTH;
        statusMessage = "Reached right boundary.";
        return false;
    }
    playerX = nextX;
    return true;
}

bool SkyDefendingGame::fire() {
    if (!isRunning || isPaused || isGameOver) {
        statusMessage = "Cannot shoot while game is stopped or paused.";
        return false;
    }
    int shotX = playerX + (PLAYER_WIDTH / 2) - 3;
    int shotY = playerY - 14;
    shots.push_back({ shotX, shotY });
    return true;
}

void SkyDefendingGame::spawnWave() {
    for (int i = 0; i < 6; ++i) {
        int spawnX = 60 + i * 115;
        if (i % 2 == 0)
            enemies.push_back(std::make_unique<StandardEnemy>(spawnX, 20));
        else
            enemies.push_back(std::make_unique<FastEnemy>(spawnX, 20));
    }
}

void SkyDefendingGame::updateGameState() {
    if (!isRunning || isPaused || isGameOver) return;

    // 1. Move projectiles
    for (auto& s : shots) {
        s.y -= s.speed;
        if (s.y + s.height < 0) s.active = false;
    }

    // 2. Move enemies polymorphically & check bottom boundary breach
    for (auto& e : enemies) {
        e->move();
        if (e->getY() + 28 >= ARENA_HEIGHT) {
            e->destroy();
            lives--;
            statusMessage = "An enemy breached the sky! Life lost.";
            if (lives <= 0) {
                lives = 0;
                isGameOver = true;
                isRunning = false;
                if (score > bestScore) bestScore = score;
                statusMessage = "GAME OVER! Final Score: " + QString::number(score) + ". Press Restart to play again.";
                return;
            }
        }
    }

    // 3. Check collisions between shots and enemies
    for (auto& s : shots) {
        if (!s.active) continue;
        for (auto& e : enemies) {
            if (!e->isActive()) continue;
            if (s.getBounds().intersects(e->getBounds())) {
                s.active = false;
                e->destroy();
                score += 10;
                if (score > bestScore) bestScore = score;
                statusMessage = "Enemy hit! +10 points.";
                break;
            }
        }
    }

    // 4. Clean up inactive objects
    shots.erase(std::remove_if(shots.begin(), shots.end(),
        [](const Projectile& p) { return !p.active; }), shots.end());

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const std::unique_ptr<Enemy>& e) { return !e->isActive(); }), enemies.end());

    // 5. Spawn new wave periodically
    spawnCounter++;
    if (enemies.empty() || spawnCounter >= 140) {
        spawnWave();
        spawnCounter = 0;
    }
}