#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "Field.hpp"
#include "CellItem.hpp"
#include "timer.h"

#include <QGLWidget>
#include <QGraphicsScene>
#include <QLCDNumber>
#include <QTimer>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lcdTimer = new QLCDNumber;
    lcdNumberOfMines = new QLCDNumber;
    timer = new QTimer;
    m_scene = new QGraphicsScene();
    labelTimer = new QLabel("Timer");
    labelMines = new QLabel("Mines");
    labelStatus = new QLabel("Win!");
    m_field = new Field();

    connect(timer, SIGNAL(timeout()), this, SLOT(slotUpdateDateTime()));

    QGLFormat f = QGLFormat::defaultFormat();
    f.setSampleBuffers(true);
    f.setSamples(4);

    ui->graphicsView->setViewport(new QGLWidget(f));
    ui->graphicsView->setScene(m_scene);
    ui->horizontalLayout->addWidget(labelTimer, 0);
    ui->horizontalLayout->addWidget(labelMines, 1);
    ui->horizontalLayout_2->addWidget(lcdTimer, 0);
    ui->horizontalLayout_2->addWidget(lcdNumberOfMines, 1);
    ui->horizontalLayout_3->addWidget(labelStatus, 0);

    labelTimer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    labelMines->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lcdTimer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    lcdNumberOfMines->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    labelStatus->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);

    setDifficulty(8, 8, 10);
    newGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, SLOT(updateSceneScale()));
}

void MainWindow::updateSceneScale()
{
    ui->graphicsView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
}

void MainWindow::on_actionNewGame_triggered()
{
    newGame();
}

void MainWindow::newGame()
{
    timer->stop();
    m_field->timeReset();
    slotUpdateDateTime();
    m_field->prepare();
    m_scene->update();
    lcdNumberOfMines->display(m_field->getNumberOfMines());
    labelStatus->hide();
    updateSceneScale();

    connect(m_field, SIGNAL(changedNumberOfMines()), this, SLOT(slotUpdateNumberOfMines()));
    connect(m_field, SIGNAL(changedNumberOfMines()), this, SLOT(slotUpdateNumberOfMines()));
    connect(m_field, SIGNAL(stopTimer()), this, SLOT(stopTime()));
    connect(m_field, SIGNAL(startTimer()), this, SLOT(startTime()));
    connect(m_field, SIGNAL(winGame()), this, SLOT(winGame()));
}

void MainWindow::on_action_Exit_triggered()
{
    this->close();
}

void MainWindow::setDifficulty(int x, int y, int mines){
    delete m_field;
    m_field = new Field();
    m_field->setSize(x, y);
    m_field->setNumberOfMines(mines);

    m_scene->clear();

    for (int y = 0; y < m_field->height(); ++y) {

        for (int x = 0; x < m_field->width(); ++x) {
            m_scene->addItem(new CellItem(m_field->cellAt(x, y)));
        }
    }

    newGame();
}

void MainWindow::on_action_Beginner_8x8_10_mines_triggered()
{
    setDifficulty(8, 8, 10);
}

void MainWindow::on_actionIntermediate_triggered()
{
    setDifficulty(16, 16, 40);
}

void MainWindow::on_actionExpert_16x30_99_mines_triggered()
{
    setDifficulty(30, 16, 99);
}

void MainWindow::slotUpdateDateTime() {
    lcdTimer->display(m_field->time());
    m_field->plusSec();
}

void MainWindow::slotUpdateNumberOfMines() {
    lcdNumberOfMines->display(m_field->minesLeft());
}

void MainWindow::winGame()
{
    lcdNumberOfMines->display(0);
    labelStatus->show();
}

void MainWindow::stopTime() {
    timer->stop();
}

void MainWindow::startTime()
{
    timer->start(1000);
}
