// Euphoria - Copyright (c) Gustav

#include "euphoria/entity.h"

#include <cassert>
#include <string>
#include <stdexcept>
#include <fstream> // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/stringmerger.h"
#include "euphoria/stdutils.h"

#include "json/json.h"

// no implementation needed for the Entity.

System::System() {
  assert(this);
}

System::~System() {
  assert(this);
}

SystemContainer::SystemContainer() {
  assert(this);
}

SystemContainer::~SystemContainer() {
  assert(this);
}

EnumType& SystemType() {
  static EnumType r;
  return r;
}

void SystemContainer::step(float dt) {
  assert(this);
  for (auto s : systems) {
    s.second->step(dt);
  }
}

void SystemContainer::add(const std::string& name,
                          std::shared_ptr<System> sys) {
  assert(this);
  systems.insert(SystemMap::value_type(name, sys));
}

std::shared_ptr<System> SystemContainer::getSystem(const std::string& name) {
  assert(this);
  auto res = systems.find(name);
  if (res == systems.end()) {
    throw std::logic_error(Str() << "Unknown system: " << name
                           << ", valid systems are: "
                           << StringMerger::EnglishAnd()
                           .generate(Keys(systems)));
  }
  return res->second;
}

EntityDef::EntityDef(SystemContainer* container, const Json::Value& value) {
  assert(this);
  assert(container);
  for (Json::ArrayIndex i = 0; i < value.size(); ++i) {
    const std::string systemname = value[i].get("system", "").asString();
    auto system = container->getSystem(systemname);
    auto arg = value[i]["data"];
    ComponentType* type = system->addType(arg);
    componenttypes.push_back(std::make_pair(system, type));
  }
}

void EntityDef::addComponents(Entity* entity) {
  assert(this);
  for (auto component : componenttypes) {
    component.first->addComponent(entity, component.second);
  }
}

EntityList::EntityList() {
}

void EntityList::addDefs(SystemContainer* container,
                         const std::string& filename) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
                           << "Unable to load definitions from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                           << reader.getFormattedErrorMessages());
  }
  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    Json::Value d = root[i];
    const std::string name = d.get("name", "").asString();
    entitydefs.insert(EntityDefs::value_type(name,
                      EntityDef(container, d["data"])));
  }
}

void EntityList::createEntity(const std::string& entity, const vec3& pos,
                              const quat& rot) {
  auto res = entitydefs.find(entity);
  if (res == entitydefs.end()) {
    throw std::logic_error(Str() << "Unknown entity type: " << entity);
  }
  std::shared_ptr<Entity> e(new Entity());
  e->position = pos;
  e->rotation = rot;
  res->second.addComponents(e.get());
  entities.push_back(e);
}

namespace {
  vec3 ToVec3(const Json::Value& v) {
    if (v.isNull()) {
      return cvec3zero();
    }
    if (v.size() != 3) {
      throw std::logic_error(Str() << "Unable to load vec3 from array with"
                             " size: " << v.size());
    }
    return vec3(v[0].asFloat(), v[1].asFloat(), v[2].asFloat());
  }

  quat ToQuat(const Json::Value& v) {
    if (v.isNull()) {
      return cquatIdent();
    }
    if (v.size() != 4) {
      throw std::logic_error(Str() << "Unable to load vec3 from array with"
                             " size: " << v.size());
    }
    return quat(v[0].asFloat(), v[1].asFloat(), v[2].asFloat(), v[3].asFloat());
  }
}  // namespace

void LoadEntities(EntityList* list, const std::string& filename) {
  assert(list);

  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str()
                           << "Unable to load definitions from " << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                           << reader.getFormattedErrorMessages());
  }
  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    Json::Value ent = root[i];
    const std::string enttype = ent.get("type", "").asString();
    list->createEntity(enttype, ToVec3(ent["pos"]), ToQuat(ent["rot"]));
  }
}
