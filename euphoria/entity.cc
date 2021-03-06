// Euphoria - Copyright (c) Gustav

#include "euphoria/entity.h"

#include <cassert>
#include <string>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/stringmerger.h"
#include "euphoria/stdutils.h"

#include "euphoria/script.h"
#include "euphoria/scriptlib.h"

#include "json/json.h"

namespace euphoria {

Entity::Entity(Lua* lua) : table(lua) {}

System::System() { assert(this); }

System::~System() { assert(this); }

SystemContainer::SystemContainer() { assert(this); }

SystemContainer::~SystemContainer() { assert(this); }

void SystemContainer::Step(float dt) {
  assert(this);
  for (auto s : systemlist_) {
    s->Step(dt);
  }
}

void SystemContainer::Add(const std::string& name,
                          std::shared_ptr<System> sys) {
  assert(this);
  systems_.insert(SystemMap::value_type(name, sys));
  systemlist_.push_back(sys);
}

std::shared_ptr<System> SystemContainer::GetSystemByName(
    const std::string& name) {
  assert(this);
  auto res = systems_.find(name);
  if (res == systems_.end()) {
    throw std::logic_error(
        Str() << "Unknown system: " << name << ", valid systems are: "
              << StringMerger::EnglishAnd().Generate(Keys(systems_)));
  }
  return res->second;
}

EntityDef::EntityDef(SystemContainer* container, const Json::Value& value) {
  assert(this);
  assert(container);
  for (Json::ArrayIndex i = 0; i < value.size(); ++i) {
    const std::string systemname = value[i].get("system", "").asString();
    auto system = container->GetSystemByName(systemname);
    auto arg = value[i]["data"];
    ComponentType* type = system->AddType(arg);
    component_types_.push_back(std::make_pair(system, type));
  }
}

void EntityDef::AddComponents(Entity* entity) {
  assert(this);
  for (auto component : component_types_) {
    component.first->AddComponent(entity, component.second);
  }
}

EntityList::EntityList() {}

void EntityList::AddDefs(SystemContainer* container,
                         const std::string& filename) {
  assert(this);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load definitions from "
                                 << filename);
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
    entitydefs_.insert(
        EntityDefs::value_type(name, EntityDef(container, d["data"])));
  }
}

void EntityList::CreateEntity(const std::string& entity, const Vec3& pos,
                              const Quat& rot, Lua* lua) {
  assert(this);
  auto res = entitydefs_.find(entity);
  if (res == entitydefs_.end()) {
    throw std::logic_error(Str() << "Unknown entity type: " << entity);
  }
  std::shared_ptr<Entity> e(new Entity(lua));
  e->position = pos;
  e->rotation = rot;
  res->second.AddComponents(e.get());
  entities_.push_back(e);
}

namespace {
Vec3 ToVec3(const Json::Value& v) {
  if (v.isNull()) {
    return CreateZeroedVec3();
  }
  if (v.size() != 3) {
    throw std::logic_error(Str() << "Unable to load vec3 from array with"
                                    " size: " << v.size());
  }
  return Vec3(v[0].asFloat(), v[1].asFloat(), v[2].asFloat());
}

Quat ToQuat(const Json::Value& v) {
  if (v.isNull()) {
    return CreateIdentityQuat();
  }
  if (v.size() != 4) {
    throw std::logic_error(Str() << "Unable to load vec3 from array with"
                                    " size: " << v.size());
  }
  return Quat(v[0].asFloat(), v[1].asFloat(), v[2].asFloat(), v[3].asFloat());
}
}  // namespace

void LoadEntities(EntityList* list, const std::string& filename, Lua* lua) {
  assert(list);
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load definitions from "
                                 << filename);
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
    list->CreateEntity(enttype, ToVec3(ent["pos"]), ToQuat(ent["rot"]), lua);
  }
}

//////////////////////////////////////////////////////////////////////////

namespace scriptingentity {
// -- Module: Entity
const std::string LUA_MODULE_NAME = "Entity";
// -- Function: GetPosition
// -- Description:
// -- Gets the position a entity.
// -- Arguments:
// -- Entity The entity
// -- Returns:
// -- vec3 the position
void GetPosition(ScriptParams* params) {
  assert(params);
  Entity* entity = 0;

  if (ScriptOverload(params) << CLightUserData(&entity)) {
    assert(entity);
    Vec3* v = ReturnVec3(params);
    *v = entity->position;
  }
}
REGISTER_SCRIPT_FUNCTION("GetPosition", GetPosition);

// -- Function: SetPosition
// -- Description:
// -- Sets the position a entity.
// -- Arguments:
// -- Entity The entity
// -- vec3 the position
void SetPosition(ScriptParams* params) {
  assert(params);
  Entity* entity = 0;
  Vec3* p = 0;

  if (ScriptOverload(params) << CLightUserData(&entity)
                             << FULL_USER_DATA(Vec3, &p)) {
    assert(entity);
    assert(p);
    entity->position = *p;
  }
}
REGISTER_SCRIPT_FUNCTION("SetPosition", SetPosition);

// -- Function: GetRotation
// -- Description:
// -- Gets the rotation a entity.
// -- Arguments:
// -- Entity The entity
// -- Returns:
// -- quat the rotation
void GetRotation(ScriptParams* params) {
  assert(params);
  Entity* entity = 0;

  if (ScriptOverload(params) << CLightUserData(&entity)) {
    assert(entity);
    Quat* v = ReturnQuat(params);
    *v = entity->rotation;
  }
}
REGISTER_SCRIPT_FUNCTION("GetRotation", GetRotation);

// -- Function: SetRotation
// -- Description:
// -- Sets the rotation a entity.
// -- Arguments:
// -- Entity The entity
// -- quat the rotation
void SetRotation(ScriptParams* params) {
  assert(params);
  Entity* entity = 0;
  Quat* p = 0;

  if (ScriptOverload(params) << CLightUserData(&entity)
                             << FULL_USER_DATA(Quat, &p)) {
    assert(entity);
    assert(p);
    entity->rotation = *p;
  }
}
REGISTER_SCRIPT_FUNCTION("SetRotation", SetRotation);
}  // namespace scriptingentity
}  // namespace euphoria
