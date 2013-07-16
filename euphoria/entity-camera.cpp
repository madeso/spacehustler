// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-rendering.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/mesh.h"

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
        camera->view = cmat44(o.entity->position, o.entity->rotation);
      }
    }

  private:
    Camera* camera;

    std::vector<std::shared_ptr<CameraType> > types;
    std::vector<CameraObject> objects;
};

void Entity_AddCamera(SystemContainer* container, Camera* camera) {
  assert(container);
  assert(camera);
  std::shared_ptr<System> sys(new CameraSystem(camera));
  container->add(CameraSystemType, sys);
}
