#include "page1.h"

#include "globaleventbus.h"

#include <qthread.h>

Page1::Page1(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    connect(this, &Page1::receiveMessage, this, [&] (const QString& msg) {
        ui.textBrowser->append(msg + "\n");
    });

    GlobalEventBus::getBus().registerObserver(this);
    QEventBus::globalBus().registerObserver(this);
}

void Page1::onEventEvent1(int value, const QString &text) {
    receiveMessage(QString("receive event1:%1 %2").arg(value).arg(text));
}

void Page1::onAsyncEventEvent2(int value, const QString &text) {
    bool inThread = QThread::currentThread() != this->thread();
    receiveMessage(QString("receive event2:%1 %2 %3").arg(value).arg(text, inThread ? "in work thread" : "in main thread"));
}

void Page1::onEventEvent3(const CustomType &customType) {
    receiveMessage(QString("receive event3:%1 %2").arg(customType.data1).arg(customType.data2));
}
