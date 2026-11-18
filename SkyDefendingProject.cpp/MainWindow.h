#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include "GameEngine.h"
#include "GameCanvas.h"

class MainWindow : public QWidget {
    Q_OBJECT
private:
    SkyDefendingGame game;
    QLabel* scoreLabel;
    QLabel* livesLabel;
    QLabel* bestLabel;
    QLabel* messageLabel;
    GameCanvas* canvas;
    QTimer* gameTimer;

    void refreshUI();

protected:
    void keyPressEvent(QKeyEvent* event) override;

public:
    MainWindow(QWidget* parent = nullptr);
};