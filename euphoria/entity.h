// Euphoria - Copyright (c) Gustav

/** @file
Core Entity related code.
 */

#ifndef EUPHORIA_ENTITY_H_
#define EUPHORIA_ENTITY_H_

#include <memory>
#include <map>
#include <vector>
#include <utility>
#include <string>

#include "euphoria/math.h"
#include "euphoria/enum.h"
#include "euphoria/id.h"
#include "json/json.h"

#include "euphoria/lua.h"

/** A entity in the component/entity framework.
 */
class Entity {
  public:
    /** Constructor.
    @param lua the lua engine.
     */
    explicit Entity(Lua* lua);

    /** The position.
     */
    vec3 position;

    /** The rotation.
     */
    quat rotation;

    /** Lua table,
     */
    Table table;

    // list of components?
};

/** Basis of a component type.
 */
class ComponentType {
};

/** A system in the component/entity framework.
 */
class System {
  public:
    /** Constructor.
     */
    System();

    /** Destructor.
     */
    virtual ~System();

    /** Update function.
    @param dt the delta t (in seconds).
     */
    virtual void Step(float dt) = 0;

    /** Add a component type of this system type to the system.
    @param data the data to the component.
    @returns the component type.
     */
    virtual ComponentType* AddType(const Json::Value& data) = 0;

    /** Add a component to a entity.
    @param entity the entity.
    @param type the type created by the System.
     */
    virtual void AddComponent(Entity* entity, ComponentType* type) = 0;

    // list of components
};

/** Contains systems
 */
class SystemContainer {
  public:
    /** Constructor.
     */
    SystemContainer();

    /** Destructor.
     */
    ~SystemContainer();

    /** Step all systems.
    @param dt the delta t (in seconds).
     */
    void Step(float dt);

    /** Add a system to the container.
    @param name the name of the system.
    @param sys the system to add.
     */
    void Add(const std::string& name, std::shared_ptr<System> sys);

    /** Get a system that has already been added.
    @param name the name of the system
    @returns the system that was added
    @throws when a requested system can't be found
     */
    std::shared_ptr<System> getSystem(const std::string& name);

  private:
    typedef std::map<std::string, std::shared_ptr<System> > SystemMap;
    SystemMap systems_;
    std::vector<std::shared_ptr<System>> systemlist_;
};

/** The definition of a entity.
 */
class EntityDef {
  public:
    /** Read data from a json value.
    @param container the system container, containing the registred systems.
    @param value the data to read from.
     */
    explicit EntityDef(SystemContainer* container, const Json::Value& value);

    /** Add components to a entity.
    @param entity the entity.
     */
    void AddComponents(Entity* entity);
  private:
    std::vector<std::pair<std::shared_ptr<System>, ComponentType*>>
        component_types_;
};

/** List of entities.
 */
class EntityList {
  public:
    /** Constructor.
     */
    EntityList();

    /** Load definitions from file.
    @param container the system container
    @param filename the filename
     */
    void AddDefs(SystemContainer* container, const std::string& filename);

    /** Create entity.
    @param entity the entity name
    @param pos the position of the entity
    @param rot the rotation of the entity
    @param lua the lua engine
     */
    void CreateEntity(const std::string& entity, const vec3& pos,
                      const quat& rot, Lua* lua);

  private:
    typedef std::map<std::string, EntityDef> EntityDefs;
    EntityDefs entitydefs_;
    std::vector<std::shared_ptr<Entity>> entities_;
};

/** Load entities from a file.
@param list store the entities here
@param filename load from this file
@param lua the lua engine
@todo merge with the World loader.
 */
void LoadEntities(EntityList* list, const std::string& filename, Lua* lua);

#endif  // EUPHORIA_ENTITY_H_
