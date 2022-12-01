#include "dialog_format.h"

#include "ui_dialog_format.h"

Dialog_format::Dialog_format(QWidget *parent)
    : QDialog(parent), ui(new Ui::Dialog_format) {
  ui->setupUi(this);
  show();
}

Dialog_format::~Dialog_format() { delete ui; }

QPair<int, QRect> Dialog_format::GetFormat() { return data; }

void Dialog_format::MakeRect() {
  data.first = ui->format_cbx->currentIndex();
  QRect resolution;
  if (!ui->resolution_cbx->currentIndex()) {
    resolution.setRect(0, 0, ui->width_sbx->value(), ui->height_sbx->value());
  } else {
    if (ui->resolution_cbx->currentIndex() != 1) {
      QStringList res = ui->resolution_cbx->currentText().split("x");
      resolution.setRect(0, 0, res.at(0).toInt(), res.at(1).toInt());
    }
  }
  data.second = resolution;
}

void Dialog_format::on_buttonBox_accepted() { MakeRect(); }
