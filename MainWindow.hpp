#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class QGraphicsScene;
class Field;
class QTimer;
class QLCDNumber;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void setDifficulty (int x, int y, int mines);
    void newGame();

protected:
    void resizeEvent(QResizeEvent *event);

protected slots:
    void updateSceneScale();
    void stopTime();
    void startTime();

private slots:
    void on_actionNewGame_triggered();
    void on_action_Exit_triggered();
    void on_action_Beginner_8x8_10_mines_triggered();
    void on_actionIntermediate_triggered();
    void on_actionExpert_16x30_99_mines_triggered();
    void slotUpdateDateTime();
    void slotUpdateNumberOfMines();
    void winGame();
    void wrappedGame(bool wrapped);

    void on_action_Wrapped_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
    Field *m_field;
    QTimer *timer;
    QLCDNumber *lcdTimer;
    QLCDNumber *lcdNumberOfMines;
    QLabel *labelTimer;
    QLabel *labelMines;
    QLabel *labelStatus;
};

#endif // MAINWINDOW_H
