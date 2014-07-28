// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-player.h"
#include <cassert>
#include <string>
#include <vector>
#include "euphoria/entity.h"

#include "euphoria/input-system.h"
#include "euphoria/input-player.h"

const std::string PlayerSystemType = "Player";

class PlayerType : public ComponentType {
 public:
  explicit PlayerType(const Json::Value& data)
      : name(data.get("name", "").asString()) {}

  std::string name;
};

struct PlayerObject {
  PlayerObject(Entity* ent, const PlayerType& t, input::InputSystem* input)
      : entity(ent), player(input->GetPlayer(t.name)) {
    assert(entity);
  }

  std::shared_ptr<input::Player> player;
  Entity* entity;
};

class PlayerSystem : public System {
 public:
  explicit PlayerSystem(input::InputSystem* input) : input_(input) {
    assert(this);
  }

  ComponentType* AddType(const Json::Value& data) {
    assert(this);
    std::shared_ptr<PlayerType> type(new PlayerType(data));
    types_.push_back(type);
    return type.get();
  }

  virtual void AddComponent(Entity* entity, ComponentType* type) {
    assert(this);
    assert(entity);
    assert(type);
    PlayerType* st = static_cast<PlayerType*>(type);
    objects_.push_back(PlayerObject(entity, *st, input_));
  }

  void Step(float dt) {
    assert(this);

    for (auto& o : objects_) {
      o.player->UpdateTable(&o.entity->table);
    }
  }

 private:
  input::InputSystem* input_;
  std::vector<std::shared_ptr<PlayerType> > types_;
  std::vector<PlayerObject> objects_;
};

void AddPlayerCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  assert(arg.camera);
  std::shared_ptr<System> sys(new PlayerSystem(arg.inputsystem));
  arg.container->Add(PlayerSystemType, sys);
}

void Entity_AddPlayer(SystemCreatorList* sc) {
  sc->Add(PlayerSystemType, AddPlayerCallback);
}
