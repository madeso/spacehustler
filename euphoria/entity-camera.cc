// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-camera.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/entity.h"
#include "euphoria/camera.h"
#include "euphoria/mesh.h"
#include "euphoria/tweak.h"

namespace euphoria {

const std::string CameraSystemType = "Camera";

class CameraType : public ComponentType {
 public:
  explicit CameraType(const Json::Value& data)
      : fov(data.get("fov", 45.0f).asFloat()),
        nearfar(data.get("near", 1.0f).asFloat(),
                data.get("far", 100.0f).asFloat()) {}

  float fov;
  NearFar nearfar;
};

struct CameraObject {
 public:
  CameraObject(Entity* ent, const CameraType& t)
      : entity(ent), fov(t.fov), nearfar(t.nearfar) {
    assert(entity);
  }

  Entity* entity;
  float fov;
  NearFar nearfar;
};

class CameraSystem : public System {
 public:
  explicit CameraSystem(Camera* cam) : camera_(cam) {
    assert(this);
    assert(cam);
  }

  ComponentType* AddType(const Json::Value& data) override {
    assert(this);
    std::shared_ptr<CameraType> type(new CameraType(data));

    types_.push_back(type);
    return type.get();
  }

  void AddComponent(Entity* entity, ComponentType* type) override {
    assert(this);
    assert(entity);
    assert(type);
    CameraType* st = static_cast<CameraType*>(type);
    objects_.push_back(CameraObject(entity, *st));
  }

  void Step(float dt) override {
    assert(this);
    assert(camera_);

    for (auto& o : objects_) {
      Quat temp = o.entity->rotation;
      temp.conjugate();
      camera_->set_view(CreateMat44(temp) *
                        CreateMat44(Vec3(-o.entity->position)));
      camera_->set_projection(CreateCameraPerspective(
          o.fov, camera_->width(), camera_->height(), o.nearfar));
    }
  }

 private:
  Camera* camera_;

  std::vector<std::shared_ptr<CameraType> > types_;
  std::vector<CameraObject> objects_;
};

void AddCameraCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new CameraSystem(arg.camera));
  arg.container->Add(CameraSystemType, sys);
}

void EntityAddCamera(SystemCreatorList* sc) {
  sc->Add(CameraSystemType, AddCameraCallback);
}
}  // namespace euphoria
