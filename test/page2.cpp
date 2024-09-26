#include "page2.h"
#include "page1.h"

#include "globaleventbus.h"
#include "customtype.h"

Page2::Page2(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void Page2::on_btn_send1_clicked() {
    if (ui.use_global->isChecked()) {
        QEventBus::globalBus().publish("Event1", ui.spinBox->value(), ui.lineEdit->text());
    } else {
        GlobalEventBus::getBus().publish("Event1", ui.spinBox->value(), ui.lineEdit->text());
    }
}

void Page2::on_btn_send2_clicked() {
    if (ui.use_global->isChecked()) {
        QEventBus::globalBus().publish("Event2", ui.spinBox->value(), ui.lineEdit->text());
    } else {
        GlobalEventBus::getBus().publish("Event2", ui.spinBox->value(), ui.lineEdit->text());
    }
}

void Page2::on_btn_send3_clicked() {
    if (ui.use_global->isChecked()) {
        QEventBus::globalBus().publish("Event3", CustomType{ ui.lineEdit->text(), ui.spinBox->value() });
    } else {
        GlobalEventBus::getBus().publish("Event3", CustomType{ ui.lineEdit->text(), ui.spinBox->value() });
    }
}

void Page2::on_btn_new_page1_clicked() {
    (new Page1)->show();
}
