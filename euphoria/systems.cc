// Euphoria - Copyright (c) Gustav

#include "euphoria/systems.h"

#include <cassert>
#include <string>
#include <stdexcept>
#include <fstream>  // NOLINT for loading data

#include "euphoria/str.h"
#include "euphoria/entity-rendering.h"
#include "euphoria/entity-physics.h"
#include "euphoria/entity-camera.h"
#include "euphoria/entity-script.h"
#include "euphoria/entity-name.h"
#include "euphoria/entity-player.h"
#include "euphoria/stringmerger.h"
#include "euphoria/stdutils.h"

#include "json/json.h"

namespace euphoria {

CreateSystemArg::CreateSystemArg(SystemContainer* acontainer, World* aworld,
                                 TextureCache* atexturecache,
                                 ShaderCache* ashadercache, Camera* acamera,
                                 Lua* ascript, const Settings& asettings,
                                 input::InputSystem* ainputsystem)
    : container(acontainer),
      world(aworld),
      texturecache(atexturecache),
      shadercache(ashadercache),
      camera(acamera),
      script(ascript),
      settings(asettings),
      inputsystem(ainputsystem) {
  assert(container);
  assert(world);
  assert(texturecache);
  assert(shadercache);
  assert(camera);
  assert(script);
  assert(inputsystem);
}

SystemCreatorList::SystemCreatorList() {}

void SystemCreatorList::Add(const std::string& name, Callback callback) {
  creators_.insert(std::make_pair(name, callback));
}

Callback SystemCreatorList::Get(const std::string& name) const {
  auto r = creators_.find(name);
  if (r == creators_.end()) {
    throw std::logic_error(
        Str() << "Unable to create built-in system " << name
              << ", valid systems are: "
              << StringMerger::EnglishAnd().Generate(Keys(creators_)));
  }
  return r->second;
}

void LoadSystems(const std::string& filename, const CreateSystemArg& arg) {
  std::ifstream in(filename.c_str());
  if (!in.good()) {
    throw std::logic_error(Str() << "Unable to load system definitions from "
                                 << filename);
  }
  Json::Value root;
  Json::Reader reader;
  if (false == reader.parse(in, root)) {
    throw std::logic_error(Str() << "Unable to parse " << filename << ": "
                                 << reader.getFormattedErrorMessages());
  }

  SystemCreatorList creators;

  EntityAddRendering(&creators);
  EntityAddPhysics(&creators);
  EntityAddCamera(&creators);
  EntityAddScript(&creators);
  EntityAddName(&creators);
  EntityAddPlayer(&creators);

  for (Json::ArrayIndex i = 0; i < root.size(); ++i) {
    const std::string systemname = root[i].get("system", "").asString();
    auto data = root[i]["data"];
    creators.Get(systemname)(arg, data);
  }
}
}  // namespace euphoria
