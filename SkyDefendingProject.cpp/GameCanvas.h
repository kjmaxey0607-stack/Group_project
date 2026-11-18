#pragma once

#include <QWidget>
#include "GameEngine.h"

class GameCanvas : public QWidget {
    Q_OBJECT
private:
    SkyDefendingGame& game;

public:
    explicit GameCanvas(SkyDefendingGame& g, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;
};