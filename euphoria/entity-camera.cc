// Euphoria - Copyright (c) Gustav

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

    ComponentType* addType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<CameraType> type(new CameraType(data));
      types.push_back(type);
      return type.get();
    }

    virtual void addComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      CameraType* st = static_cast<CameraType*>(type);
      objects.push_back(CameraObject(entity, *st));
    }

    void step(float dt) {
      assert(this);
      assert(camera);

      for (auto & o : objects) {
        TWEAK(o.entity->rotation);
        camera->set_view(cmat44(o.entity->rotation)
                         * cmat44(vec3(-o.entity->position)));
      }
    }

  private:
    Camera* camera;

    std::vector<std::shared_ptr<CameraType> > types;
    std::vector<CameraObject> objects;
};

void AddCameraCallback(CreateSystemArg arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new CameraSystem(arg.camera));
  arg.container->add(CameraSystemType, sys);
}

void Entity_AddCamera(SystemCreatorList* sc) {
  sc->add(CameraSystemType, AddCameraCallback);
}
