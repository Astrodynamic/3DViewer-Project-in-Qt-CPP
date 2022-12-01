#ifndef DIALOG_FORMAT_H
#define DIALOG_FORMAT_H

#include <QDialog>

namespace Ui {
class Dialog_format;
}

class Dialog_format : public QDialog {
  Q_OBJECT

 public:
  explicit Dialog_format(QWidget *parent = nullptr);
  ~Dialog_format();
  QPair<int, QRect> GetFormat();

 private:
  void MakeRect();
  Ui::Dialog_format *ui;
  QPair<int, QRect> data;

 private slots:
  void on_buttonBox_accepted();
};

#endif  // DIALOG_FORMAT_H
