#ifndef SAVIOR_H
#define SAVIOR_H

#include <QBuffer>
#include <QFileDialog>
#include <QLabel>
#include <QMessageBox>
#include <QMovie>
#include <QObject>
#include <QThread>
#include <QTimer>

#include "dialog_format.h"
#include "qgifimage.h"
#include "v3d_gl.h"

namespace s21 {

class Savior : public QWidget {
  Q_OBJECT
 public:
  explicit Savior(V3D_GL *, QWidget *parent = nullptr);
  ~Savior();

 protected:
  virtual void mousePressEvent(QMouseEvent *event);

 private:
  void PrepearToSave();
  void MoveWindow();
  void MakeBackground();
  void SetLifeTimer();
  void SetPreviewWindow();

  V3D_GL *m_gl_parent_;
  bool m_gif_flag_;
  QString file_format_;
  QImage m_frame_;
  QBuffer buffer_;
  QTimer *life_timer_;
  QRect frame_size_;
  QSize widow_size_;

 private slots:
  void CloseBuffer();
  void RecordGif();
  void SwitchToPopup();
  void SendNewFrame();
  void SaveJpeg();
  void SaveBmp();
  void SaveGif();
  void SavePng();

 signals:
  void RecordStart();
  void RecordDone();
  void SaveBuffer();
  void NewFrame(QImage);
};

class GifToThread : public QObject, public QGifImage {
  Q_OBJECT
 public:
  explicit GifToThread(QBuffer &);

 private:
  void SetDurationGif();
  void SetFrequencyFrame();

  QTimer *m_frame_timer_;
  QTimer *m_gif_timer_;
  QBuffer &device_;

 public slots:
  void AddFrame(QImage);
  void RecordGif();
  void StartRecord();

 signals:
  void GetNewFrame();
  void RecordDone();
  void SaveBuffer();
};

}  // namespace s21

#endif  // SAVIOR_H
