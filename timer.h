#ifndef TIMER_H
#define TIMER_H

#include <QWidget>

class QTimer;
class QLCDNumber;

class Timer
{
public:
    Timer();

     int getTime() const { return time;}
     void startTimer();
     QLCDNumber *m_lcdNumber() const {return lcdNumber;}

private:
    int time;
    QTimer *timer;
    QLCDNumber *lcdNumber;

//private slots:
//    void slotUpdateDateTime();
};

#endif // TIMER_H
