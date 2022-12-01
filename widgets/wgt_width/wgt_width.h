#ifndef SRC_INCLUDE_DIALOG_SIZE_H_
#define SRC_INCLUDE_DIALOG_SIZE_H_

#include <QDialog>

namespace Ui {
class WgtWidth;
}

class WgtWidth : public QDialog {
  Q_OBJECT

 public:
  explicit WgtWidth(QWidget *parent = nullptr, double value = 1.0f);
  ~WgtWidth();

  void SetDefaultValue(double value);
  double GetValue() const;

 private slots:
  void on_sb_width_valueChanged(double value);

 private:
  Ui::WgtWidth *ui;
};

#endif  // SRC_INCLUDE_DIALOG_SIZE_H_
