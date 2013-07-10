// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_ENTITY_H_
#define EUPHORIA_ENTITY_H_

#include <boost/shared_ptr.hpp>
#include <map>

#include "euphoria/math.h"
#include "euphoria/enum.h"

#include "jsoncpp/json/json.h"

/** A entity in the component/entity framework.
 */
class Entity {
  public:
    /** The position.
     */
    vec3 position;

    /** The rotation.
     */
    quat rotation;

    // list of components?
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
    virtual void step(float dt) = 0;

    /** Add a component type of this system type to the system.
    @param data the data to the component.
     */
    virtual void addType(const Json::Value& data) = 0;

    // list of components
};

/** A shared pointer to a System.
 */
typedef boost::shared_ptr<System> SystemPtr;

/** The global system type.
@return the global system type.
 */
EnumType& SystemType();

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
    void step(float dt);

    /** Add a system to the container.
    @param systemType the type of system
    @param sys the system to add.
     */
    void add(EnumValue systemType, SystemPtr sys);

  private:
    typedef std::map<EnumValue, SystemPtr> SystemMap;
    SystemMap systems;
};

#endif  // EUPHORIA_ENTITY_H_
