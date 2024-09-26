#pragma once

#include <QWidget>

#include "ui_page1.h"

#include "customtype.h"

class Page1 : public QWidget {
    Q_OBJECT

public:
    explicit Page1(QWidget *parent = nullptr);

signals:
    void receiveMessage(const QString &message);

private:
    Q_INVOKABLE void onEventEvent1(int value, const QString& text);
    Q_INVOKABLE void onAsyncEventEvent2(int value, const QString& text);
    Q_INVOKABLE void onEventEvent3(const CustomType& customType);

private:
    Ui::Page1 ui;
};
