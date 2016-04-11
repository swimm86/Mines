#include "timer.h"
#include <QLCDNumber>
#include <QTimer>


Timer::Timer()
{
    time = 0;
    lcdNumber = new QLCDNumber;
    timer = new QTimer;

//    connect(timer, SIGNAL(timeout()), MainW, SLOT(slotUpdateDateTime()));
}

//void Timer::slotUpdateDateTime() {
//    lcdNumber->display(m_time);
//    ++m_time;
//}

void Timer::startTimer() {
    timer->start(1000);
    lcdNumber->display(0);
//    slotUpdateDateTime();
}

