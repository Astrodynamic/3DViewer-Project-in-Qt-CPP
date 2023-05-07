#include "savior.h"

namespace s21 {

Savior::Savior(V3D_GL *gl_parent, [[maybe_unused]] QWidget *parent)
    : m_gl_parent_{gl_parent}, m_gif_flag_{false} {
  SetLifeTimer();
  PrepearToSave();
  SetPreviewWindow();

  buffer_.open(QIODevice::WriteOnly);
}

Savior::~Savior() { m_gl_parent_ = nullptr; }

void Savior::mousePressEvent([[maybe_unused]] QMouseEvent *event) {
  close();
  QString window_title("Save File " + file_format_);
  QString fileName = QFileDialog::getSaveFileName(
      this, window_title, QDateTime::currentDateTime().toString(),
      file_format_);
  QFile *m_ptr_save_file_ = new QFile(fileName);
  if (!m_ptr_save_file_->open(QIODevice::WriteOnly)) {
    m_ptr_save_file_ = nullptr;
  } else {
    m_ptr_save_file_->write(buffer_.buffer());
    m_ptr_save_file_->close();
  }
};

void Savior::SaveJpeg() {
  emit RecordStart();
  file_format_ = "Image (*.jpeg)";
  m_frame_.save(&buffer_, "JPEG");
  CloseBuffer();
}

void Savior::SaveBmp() {
  emit RecordStart();
  file_format_ = "Image (*.bmp)";
  m_frame_.save(&buffer_, "BMP");
  CloseBuffer();
}

void Savior::SaveGif() {
  file_format_ = "GIF Animation (*.gif)";
  m_gif_flag_ = true;
  GifToThread *gif_thread = new GifToThread(buffer_);

  connect(gif_thread, SIGNAL(GetNewFrame()), this, SLOT(SendNewFrame()));
  connect(this, SIGNAL(NewFrame(QImage)), gif_thread, SLOT(AddFrame(QImage)));
  connect(gif_thread, SIGNAL(RecordDone()), this, SLOT(RecordGif()));
  connect(gif_thread, SIGNAL(SaveBuffer()), this, SIGNAL(SaveBuffer()));
  gif_thread->StartRecord();
}

void Savior::SavePng() {
  emit RecordStart();
  file_format_ = "Image (*.png)";
  m_frame_.save(&buffer_, "PNG");
  CloseBuffer();
}

void Savior::PrepearToSave() {
  Dialog_format format;
  if (format.exec() == QDialog::Accepted) {
    frame_size_ = format.GetFormat().second;
    if (frame_size_.height() == 0 && frame_size_.width() == 0) {
      frame_size_.setSize(m_gl_parent_->grabFramebuffer().size());
    }
    m_frame_ = m_gl_parent_->grab(frame_size_).copy(frame_size_).toImage();
    widow_size_ = m_frame_.scaledToWidth(426).size();
    switch (format.GetFormat().first) {
      case 0:
        SaveJpeg();
        break;
      case 1:
        SaveBmp();
        break;
      case 2:
        SavePng();
        break;
      case 3:
        SaveGif();
        break;
      default:
        break;
    }
  }
}

void Savior::MoveWindow() {
  QRect rect = this->screen()->geometry();
  int x = (rect.width() - width() - 50);
  int y = (rect.height() - height() - 50);
  move(x, y);
}

void Savior::MakeBackground() {
  QLabel *lab = new QLabel(this);
  if (m_gif_flag_) {
    QMovie *m_animation = new QMovie(&buffer_);
    m_animation->setScaledSize(this->size());
    lab->setMovie(m_animation);
    m_animation->start();
  } else {
    lab->setPixmap(QPixmap::fromImage(m_frame_));
    lab->setScaledContents(true);
    lab->setFixedSize(widow_size_);
  }
}

void Savior::SetLifeTimer() {
  life_timer_ = new QTimer(this);
  life_timer_->setSingleShot(true);
  life_timer_->setInterval(3000);
  connect(life_timer_, SIGNAL(timeout()), this, SLOT(SwitchToPopup()));
}

void Savior::SetPreviewWindow() {
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setFixedSize(widow_size_);
  MoveWindow();
}

void Savior::CloseBuffer() {
  buffer_.close();
  buffer_.open(QIODevice::ReadOnly);
  MakeBackground();
  show();
  life_timer_->start();
  emit RecordDone();
}

void Savior::RecordGif() { CloseBuffer(); }

void Savior::SwitchToPopup() {
  setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Popup);
  show();
}

void Savior::SendNewFrame() {
  emit NewFrame(m_gl_parent_->grab(frame_size_).copy(frame_size_).toImage());
}

GifToThread::GifToThread(QBuffer &out_device) : device_{out_device} {}

void GifToThread::SetDurationGif() {
  m_gif_timer_ = new QTimer(this);
  m_gif_timer_->setSingleShot(true);
  m_gif_timer_->setInterval(5000);
  connect(this, SIGNAL(destroyed()), m_gif_timer_, SLOT(deleteLater()));
  connect(m_gif_timer_, SIGNAL(timeout()), this, SLOT(RecordGif()));
}

void GifToThread::SetFrequencyFrame() {
  m_frame_timer_ = new QTimer(this);
  connect(this, SIGNAL(destroyed()), m_frame_timer_, SLOT(deleteLater()));
  connect(m_frame_timer_, SIGNAL(timeout()), this, SIGNAL(GetNewFrame()));
  connect(m_gif_timer_, SIGNAL(timeout()), m_frame_timer_, SLOT(stop()));
  m_frame_timer_->setInterval(30);
}

void GifToThread::StartRecord() {
  SetDurationGif();
  SetFrequencyFrame();
  m_frame_timer_->start();
  m_gif_timer_->start();
}

void GifToThread::AddFrame(QImage frame) { addFrame(frame, 30); }

void GifToThread::RecordGif() {
  emit SaveBuffer();
  save(&device_);
  emit RecordDone();
}

}  // namespace s21
