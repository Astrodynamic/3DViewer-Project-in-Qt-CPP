#ifndef ILLUMINATION_H
#define ILLUMINATION_H

#include "spot_light.h"

namespace s21 {
enum class LightType { kFlat = 0, kSoft };

class Illumination : public QObject {
  Q_OBJECT
 public:
  Illumination();
  ~Illumination();
  void addLight(QString);
  QMap<QString, QVariant> &GetLightInfo();
  QMap<QString, QVariant> &GetLightInfo(QString, int);

  bool ItemIsActive(QVariant &);
  void SetItemStatus(QString, int, bool);
  QMap<QString, QVector<QVariant> *> &GetAllLight() { return illumination_; }
  bool HasItem(QString, int);

  int GetItemDistance();
  LightType GetLightType() const;
  void SetLightType(LightType type);

 private:
  QVariant *currentItem_;
  LightType light_type_ = LightType::kSoft;

  QVector<QVariant> *dirLights_;
  QVector<QVariant> *pointLights_;
  QVector<QVariant> *spotLights_;
  QMap<QString, QVector<QVariant> *> illumination_;
  QPair<QString, int> current_light_;

 signals:
  void CurrentLightInfo(QMap<QString, QVariant>);
  void CurentLightDistance(int);
  void Scpecular(QVector3D);
  void Diffuse(QVector3D);
  void Intensity(QVector3D);

 public slots:
  void ChangeDirectionX(double);
  void ChangeDirectionY(double);
  void ChangeDirectionZ(double);

  void ChangePositionX(double);
  void ChangePositionY(double);
  void ChangePositionZ(double);
  void ChangeCurrent(QString, int);
  void SetItemDistance(int);
  void GetSpecular();
  void GetDiffuse();
  void GetIntensity();

  void SetSpecular(QVector3D);
  void SetDiffuse(QVector3D);
  void SetIntensity(QVector3D);
  void SetCut(double);
  void SetOuterCut(double);
  void RemoveLight(QString, int);
};
}  // namespace s21
#endif  // ILLUMINATION_H
