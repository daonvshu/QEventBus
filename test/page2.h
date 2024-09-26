#pragma once

#include <QWidget>

#include "ui_page2.h"

class Page2 : public QWidget {
    Q_OBJECT

public:
    explicit Page2(QWidget *parent = nullptr);

private slots:
    void on_btn_send1_clicked();
    void on_btn_send2_clicked();
    void on_btn_send3_clicked();
    void on_btn_new_page1_clicked();

private:
    Ui::Page2 ui;
};
