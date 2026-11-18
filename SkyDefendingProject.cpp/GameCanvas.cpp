#include "GameCanvas.h"
#include <QPainter>
#include <QPolygon>

GameCanvas::GameCanvas(SkyDefendingGame& g, QWidget* parent)
    : QWidget(parent), game(g) {
    setFixedSize(SkyDefendingGame::ARENA_WIDTH, SkyDefendingGame::ARENA_HEIGHT);
}

void GameCanvas::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Background sky
    painter.fillRect(rect(), QColor(18, 12, 36));

    // Stars
    painter.setPen(Qt::white);
    for (int i = 0; i < 45; ++i) {
        int sx = (i * 173) % width();
        int sy = (i * 97) % height();
        painter.drawPoint(sx, sy);
    }

    // Draw Defender (Player ship at bottom)
    QPolygon ship;
    ship << QPoint(game.playerX + SkyDefendingGame::PLAYER_WIDTH / 2, game.playerY)
        << QPoint(game.playerX, game.playerY + SkyDefendingGame::PLAYER_HEIGHT)
        << QPoint(game.playerX + SkyDefendingGame::PLAYER_WIDTH, game.playerY + SkyDefendingGame::PLAYER_HEIGHT);
    painter.setBrush(QColor(80, 220, 140));
    painter.setPen(Qt::NoPen);
    painter.drawPolygon(ship);

    // Draw Enemies (polymorphic colors)
    for (const auto& e : game.enemies) {
        painter.setBrush(e->getColor());
        painter.drawRoundedRect(e->getBounds(), 6, 6);
    }

    // Draw Projectiles
    painter.setBrush(QColor(255, 230, 80));
    for (const auto& s : game.shots) {
        painter.drawRect(s.getBounds());
    }

    // Overlay text when Game Over
    if (game.isGameOver) {
        painter.fillRect(rect(), QColor(0, 0, 0, 150));
        painter.setPen(QColor(255, 80, 80));
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    }
}