#include "wgt_width.h"

#include "ui_wgt_width.h"

WgtWidth::WgtWidth(QWidget *parent, double value)
    : QDialog(parent), ui(new Ui::WgtWidth) {
  ui->setupUi(this);
  SetDefaultValue(value);
  this->show();
}

WgtWidth::~WgtWidth() { delete ui; }

void WgtWidth::SetDefaultValue(double value) {
  ui->sb_width->setValue(value);
  ui->l_line->setLineWidth(value);
}

double WgtWidth::GetValue() const { return ui->sb_width->value(); }

void WgtWidth::on_sb_width_valueChanged(double value) {
  ui->l_line->setLineWidth(value);
}
