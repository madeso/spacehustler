  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/entity-camera.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/entity.h"
#include "euphoria/camera.h"
#include "euphoria/mesh.h"
#include "euphoria/tweak.h"

const std::string CameraSystemType = "Camera";

class CameraType : public ComponentType {
  public:
    explicit CameraType(const Json::Value& data) {
    }
};

class CameraObject {
  public:
    CameraObject(Entity* ent, const CameraType& t)
      : entity(ent) {
      assert(entity);
    }

    Entity* entity;
};

class CameraSystem : public System {
  public:
    explicit CameraSystem(Camera* cam)
      : camera(cam) {
      assert(this);
      assert(cam);
    }

    ComponentType* AddType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<CameraType> type(new CameraType(data));
      types_.push_back(type);
      return type.get();
    }

    virtual void AddComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      CameraType* st = static_cast<CameraType*>(type);
      objects_.push_back(CameraObject(entity, *st));
    }

    void Step(float dt) {
      assert(this);
      assert(camera);

      for (auto& o : objects_) {
        quat temp = o.entity->rotation;
        temp.conjugate();
        camera->set_view(
          cmat44(temp)
          * cmat44(vec3(-o.entity->position)));
      }
    }

  private:
    Camera* camera;

    std::vector<std::shared_ptr<CameraType> > types_;
    std::vector<CameraObject> objects_;
};

void AddCameraCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new CameraSystem(arg.camera));
  arg.container->Add(CameraSystemType, sys);
}

void Entity_AddCamera(SystemCreatorList* sc) {
  sc->Add(CameraSystemType, AddCameraCallback);
}
