// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-physics.h"
#include <cassert>
#include <string>
#include <vector>

const std::string PhysicsSystemType = "Physics";

class PhysicsType : public ComponentType {
  public:
    explicit PhysicsType(const Json::Value& data) {
    }
};

class PhysicsObject {
  public:
    explicit PhysicsObject(Entity* entity)
      : entity(entity) {
    }

    Entity* entity;
};

class PhysicsSystem : public System {
  public:
    PhysicsSystem() {
      assert(this);
    }

    ComponentType* addType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<PhysicsType> type(new PhysicsType(data));
      types.push_back(type);
      return type.get();
    }

    virtual void addComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      PhysicsType* st = static_cast<PhysicsType*>(type);
      mat44 mat = cmat44(entity->position, entity->rotation);
      objects.push_back(PhysicsObject(entity));
    }

    void step(float dt) {
      assert(this);
      for (auto & o : objects) {
      }
    }

  private:
    std::vector<std::shared_ptr<PhysicsType> > types;
    std::vector<PhysicsObject> objects;
};

void Entity_AddRendering(SystemContainer* container) {
  assert(container);
  std::shared_ptr<System> sys(new PhysicsSystem());
  container->add(PhysicsSystemType, sys);
}
