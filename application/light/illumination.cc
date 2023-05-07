#include "illumination.h"

namespace s21 {
Illumination::Illumination()
    : currentItem_{nullptr},
      dirLights_{new QVector<QVariant>()},
      pointLights_{new QVector<QVariant>()},
      spotLights_{new QVector<QVariant>()},
      illumination_{{"dirLight", dirLights_},
                    {"pointLight", pointLights_},
                    {"spotLight", spotLights_}},
      current_light_{"dirLight", 0}

{
  addLight("dirLight");
  emit CurrentLightInfo(GetLightInfo());
}

Illumination::~Illumination() {
  for (auto &it : illumination_) {
    for ([[maybe_unused]] auto &item : *it) {
      RemoveLight(illumination_.key(it), 0);
    }
    delete it;
  }
}

void Illumination::addLight(QString type) {
  if (type == "dirLight") {
    illumination_.value(type)->append(QVariant::fromValue(new Light()));
  } else if (type == "pointLight") {
    illumination_.value(type)->append(QVariant::fromValue(new PointLight()));
  } else if (type == "spotLight") {
    illumination_.value(type)->append(QVariant::fromValue(new SpotLight()));
  }
}

QMap<QString, QVariant> &Illumination::GetLightInfo() {
  return GetLightInfo(current_light_.first, current_light_.second);
}

QMap<QString, QVariant> &Illumination::GetLightInfo(QString type, int index) {
  QVariant item = illumination_.value(type)->at(index);
  QString type_item = item.metaType().name();

  if (type_item == "s21::PointLight*") {
    return item.value<PointLight *>()->GetInfo();
  } else if (type_item == "s21::SpotLight*") {
    return item.value<SpotLight *>()->GetInfo();
  }
  return item.value<Light *>()->GetInfo();
}

void Illumination::RemoveLight(QString type, int index) {
  QVariant item = illumination_.value(type)->at(index);
  QString type_item = item.metaType().name();
  if (type_item == "s21::Light*") {
    illumination_.value(type)->remove(index);
    delete item.value<Light *>();
  } else if (type_item == "s21::PointLight*") {
    illumination_.value(type)->remove(index);
    delete item.value<PointLight *>();
  } else if (type_item == "s21::SpotLight*") {
    illumination_.value(type)->remove(index);
    delete item.value<SpotLight *>();
  }
}

bool Illumination::ItemIsActive(QVariant &item) {
  bool status = true;
  QString type_item = item.typeName();
  if (type_item == "s21::Light*") {
    status = item.value<Light *>()->isActive();
  } else if (type_item == "s21::PointLight*") {
    status = item.value<PointLight *>()->isActive();
  } else if (type_item == "s21::SpotLight*") {
    status = item.value<SpotLight *>()->isActive();
  }
  return status;
}

void Illumination::SetItemStatus(QString type, int index, bool status) {
  QVariant item = illumination_.value(type)->at(index);
  if (type == "dirLight") {
    item.value<Light *>()->SetActive(status);
  } else if (type == "pointLight") {
    item.value<PointLight *>()->SetActive(status);
  } else if (type == "spotLight") {
    item.value<SpotLight *>()->SetActive(status);
  }
}

bool Illumination::HasItem(QString type, int index) {
  if (illumination_.value(type)->size() > index) {
    return true;
  } else {
    return false;
  }
}

void Illumination::SetItemDistance(int index) {
  QVariant item =
      illumination_.value(current_light_.first)->at(current_light_.second);
  if (current_light_.first == "pointLight") {
    item.value<PointLight *>()->SetDistance(index);
  } else if (current_light_.first == "spotLight") {
    item.value<SpotLight *>()->SetDistance(index);
  }
}

void Illumination::GetSpecular() {
  emit Scpecular(*GetLightInfo().value(".specular").value<QVector3D *>());
}

void Illumination::GetDiffuse() {
  emit Diffuse(*GetLightInfo().value(".diffuse").value<QVector3D *>());
}

void Illumination::GetIntensity() {
  emit Intensity(*GetLightInfo().value(".ambient").value<QVector3D *>());
}

void Illumination::SetSpecular(QVector3D data) {
  *GetLightInfo().value(".specular").value<QVector3D *>() = data;
}

void Illumination::SetDiffuse(QVector3D data) {
  *GetLightInfo().value(".diffuse").value<QVector3D *>() = data;
}

void Illumination::SetIntensity(QVector3D data) {
  *GetLightInfo().value(".ambient").value<QVector3D *>() = data;
}

void Illumination::SetCut(double value) {
  *GetLightInfo().value(".cutOff").value<float *>() = value;
}

void Illumination::SetOuterCut(double value) {
  *GetLightInfo().value(".outerCutOff").value<float *>() = value;
}

LightType Illumination::GetLightType() const { return light_type_; }

void Illumination::SetLightType(LightType type) { light_type_ = type; }

int Illumination::GetItemDistance() {
  QVariant item =
      illumination_.value(current_light_.first)->at(current_light_.second);
  int distance = 0;
  if (current_light_.first == "pointLight") {
    distance = item.value<PointLight *>()->GetDistance();
  } else if (current_light_.first == "spotLight") {
    distance = item.value<SpotLight *>()->GetDistance();
  }
  return distance;
}

void Illumination::ChangeCurrent(QString type, int index) {
  if (HasItem(type, index)) {
    current_light_.first = type;
    current_light_.second = index;
    if (type != "dirLight") {
      emit CurentLightDistance(GetItemDistance());
    }
    emit CurrentLightInfo(GetLightInfo());
  }
}

void Illumination::ChangeDirectionX(double value) {
  GetLightInfo().value(".direction").value<QVector3D *>()->setX(value);
}

void Illumination::ChangeDirectionY(double value) {
  GetLightInfo().value(".direction").value<QVector3D *>()->setY(value);
}

void Illumination::ChangeDirectionZ(double value) {
  GetLightInfo().value(".direction").value<QVector3D *>()->setZ(value);
}

void Illumination::ChangePositionX(double value) {
  GetLightInfo().value(".position").value<QVector3D *>()->setX(value);
}

void Illumination::ChangePositionY(double value) {
  GetLightInfo().value(".position").value<QVector3D *>()->setY(value);
}

void Illumination::ChangePositionZ(double value) {
  GetLightInfo().value(".position").value<QVector3D *>()->setZ(value);
}
}  // namespace s21
