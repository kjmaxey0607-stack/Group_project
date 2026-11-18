#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent) {
    setWindowTitle("Sky Defending — Qt Widgets");
    setFocusPolicy(Qt::StrongFocus);

    auto* mainLayout = new QVBoxLayout(this);

    // Top HUD Bar
    auto* topBar = new QHBoxLayout();
    auto* titleLabel = new QLabel("SKY DEFENDING");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 16px; color: #fff;");
    scoreLabel = new QLabel("Score: 0");
    livesLabel = new QLabel("Lives: 3");
    bestLabel = new QLabel("Best: 0");

    scoreLabel->setStyleSheet("color: #fff;");
    livesLabel->setStyleSheet("color: #fff;");
    bestLabel->setStyleSheet("color: #fff;");

    topBar->addWidget(titleLabel);
    topBar->addStretch();
    topBar->addWidget(scoreLabel);
    topBar->addSpacing(20);
    topBar->addWidget(livesLabel);
    topBar->addSpacing(20);
    topBar->addWidget(bestLabel);

    // Middle Play Area Canvas
    canvas = new GameCanvas(game, this);

    // Bottom Control Bar
    auto* controlBar = new QHBoxLayout();
    auto* startBtn = new QPushButton("Start");
    auto* pauseBtn = new QPushButton("Pause");
    auto* restartBtn = new QPushButton("Restart");
    auto* controlsHint = new QLabel("Move: ← / →    |    Shoot: Space");
    controlsHint->setStyleSheet("color: #ccc;");

    // Prevent buttons from stealing arrow/space keyboard focus from the window
    startBtn->setFocusPolicy(Qt::NoFocus);
    pauseBtn->setFocusPolicy(Qt::NoFocus);
    restartBtn->setFocusPolicy(Qt::NoFocus);

    controlBar->addWidget(startBtn);
    controlBar->addWidget(pauseBtn);
    controlBar->addWidget(restartBtn);
    controlBar->addStretch();
    controlBar->addWidget(controlsHint);

    // Message Area
    messageLabel = new QLabel(game.statusMessage);
    messageLabel->setStyleSheet("padding: 6px; background-color: #222; color: #f0f0f0; border-radius: 4px;");

    mainLayout->addLayout(topBar);
    mainLayout->addWidget(canvas);
    mainLayout->addLayout(controlBar);
    mainLayout->addWidget(messageLabel);

    // Connect Buttons to Game Functions
    connect(startBtn, &QPushButton::clicked, this, [this]() {
        game.start();
        this->setFocus();
        refreshUI();
        });
    connect(pauseBtn, &QPushButton::clicked, this, [this]() {
        game.togglePause();
        this->setFocus();
        refreshUI();
        });
    connect(restartBtn, &QPushButton::clicked, this, [this]() {
        game.restart();
        this->setFocus();
        refreshUI();
        });

    // Game Loop Timer (~33 FPS)
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, [this]() {
        game.updateGameState();
        refreshUI();
        });
    gameTimer->start(30);
}

void MainWindow::refreshUI() {
    scoreLabel->setText("Score: " + QString::number(game.score));
    livesLabel->setText("Lives: " + QString::number(game.lives));
    bestLabel->setText("Best: " + QString::number(game.bestScore));
    messageLabel->setText(game.statusMessage);
    canvas->update();
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Left) {
        game.movePlayer(-1);
    }
    else if (event->key() == Qt::Key_Right) {
        game.movePlayer(1);
    }
    else if (event->key() == Qt::Key_Space) {
        game.fire();
    }
    else {
        QWidget::keyPressEvent(event);
        return;
    }
    refreshUI();
}