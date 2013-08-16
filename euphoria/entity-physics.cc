// Euphoria - Copyright (c) Gustav

#include "euphoria/entity-physics.h"
#include <cassert>
#include <string>
#include <vector>
#include <map>

#include "euphoria/entity.h"
#include "euphoria/world.h"
#include "euphoria/tweak.h"
#include "euphoria/script.h"

#include "btBulletDynamicsCommon.h"  // NOLINT this is the proper way to include bullet


const std::string PhysicsSystemType = "Physics";

class PhysicsType : public ComponentType {
  public:
    explicit PhysicsType(const Json::Value& data) {
      mass = data.get("mass", 1.0f).asFloat();
      width = data.get("width", 1.0f).asFloat();
      height = data.get("height", 1.0f).asFloat();
      depth = data.get("depth", 1.0f).asFloat();
    }

    float mass;

    float width;
    float height;
    float depth;
};

class WorldBodyConenction {
  public:
    WorldBodyConenction(std::shared_ptr<btDiscreteDynamicsWorld> dworld,
                        std::shared_ptr<btRigidBody> rbody)
      : world(dworld)
      , body(rbody) {
      world->addRigidBody(body.get());
    }
    ~WorldBodyConenction() {
      world->removeCollisionObject(body.get());
    }

    std::shared_ptr<btDiscreteDynamicsWorld> world;
    std::shared_ptr<btRigidBody> body;
};

namespace {
  vec3 C(const btVector3& src) {
    return vec3(src.x(), src.y(), src.z());
  }

  btVector3 C(const vec3& src) {
    return btVector3(src[0], src[1], src[2]);
  }

  quat C(const btQuaternion& src) {
    return quat(src.x(), src.y(), src.z(), src.w());
  }

  btQuaternion C(const quat& src) {
    return btQuaternion(src[0], src[1], src[2], src[3]);
  }
}  // namespace

class PhysicsObject {
  public:
    PhysicsObject(Entity* entity,
                  std::shared_ptr<btDiscreteDynamicsWorld> world,
                  const PhysicsType& data)
      : entity(entity)
      , dynamicsWorld(world) {
      shape.reset(new btBoxShape(btVector3(data.width / 2, data.height / 2,
                                           data.depth / 2)));

      btTransform trans;
      trans.setIdentity();
      trans.setOrigin(C(entity->position));
      trans.setRotation(C(entity->rotation));


      btScalar mass = data.mass;

      // rigid body is dynamic if and only if mass is non zero, otherwise static
      bool isDynamic = (mass != 0.f);
      btVector3 localInertia(0, 0, 0);
      if (isDynamic) {
        shape->calculateLocalInertia(mass, localInertia);
      }

      myMotionState.reset(new btDefaultMotionState(trans));
      btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState.get()
          , shape.get(), localInertia);
      body.reset(new btRigidBody(rbInfo));

      // add the body to the dynamics world
      connection.reset(new WorldBodyConenction(dynamicsWorld, body));
    }

    void update() {
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      assert(entity);
      entity->position = C(trans.getOrigin());
      entity->rotation = C(trans.getRotation());
    }

    Entity* entity;
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    std::shared_ptr<btCollisionShape> shape;
    std::shared_ptr<btDefaultMotionState> myMotionState;
    std::shared_ptr<btRigidBody> body;
    std::shared_ptr<WorldBodyConenction> connection;
};

class StaticMesh {
  public:
    StaticMesh(const internal::MeshPart& part,
               std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld) {
      mesh.reset(new btTriangleMesh());

      const unsigned int facecount = part.faces.size() / 3;
      for (unsigned int f = 0; f < facecount; ++f) {
        const unsigned int fb = f * 3;
        const unsigned int p1 = part.faces[fb + 0];
        const unsigned int p2 = part.faces[fb + 1];
        const unsigned int p3 = part.faces[fb + 2];
        const vec3 v1 = part.getVertex(p1);
        const vec3 v2 = part.getVertex(p2);
        const vec3 v3 = part.getVertex(p3);
        mesh->addTriangle(C(v1), C(v2), C(v3));
      }

      shape.reset(new btBvhTriangleMeshShape(mesh.get(), true));

      btTransform trans;
      trans.setIdentity();

      btScalar mass = 0.0f;

      // rigid body is dynamic if and only if mass is non zero, otherwise static
      btVector3 localInertia(0, 0, 0);

      myMotionState.reset(new btDefaultMotionState(trans));
      btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState.get()
          , shape.get(), localInertia);
      body.reset(new btRigidBody(rbInfo));

      // add the body to the dynamics world
      connection.reset(new WorldBodyConenction(dynamicsWorld, body));
    }

    std::shared_ptr<btTriangleMesh> mesh;
    std::shared_ptr<btCollisionShape> shape;
    std::shared_ptr<btDefaultMotionState> myMotionState;
    std::shared_ptr<btRigidBody> body;
    std::shared_ptr<WorldBodyConenction> connection;
};

namespace {
  Color ToColor(const btVector3& c) {
    return Color(c.x(), c.y(), c.z());
  }
}

class DebugDrawing : public btIDebugDraw {
  public:
    explicit DebugDrawing(World* w)
      : world(w)
      , mode(DBG_DrawWireframe | DBG_DrawAabb | DBG_DrawFeaturesText
             | DBG_DrawContactPoints | DBG_NoDeactivation | DBG_NoHelpText
             | DBG_DrawText | DBG_ProfileTimings | DBG_EnableSatComparison
             | DBG_DisableBulletLCP | DBG_EnableCCD | DBG_DrawConstraints
             | DBG_DrawConstraintLimits | DBG_FastWireframe | DBG_DrawNormals) {
      assert(w);
    }
    void drawLine(const btVector3& from, const btVector3& to,
                  const btVector3& color) {
      assert(this);
      assert(world);
      world->debug().line(C(from), C(to), ToColor(color));
    }

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
                          btScalar distance, int lifeTime,
                          const btVector3& color) {
    }

    void reportErrorWarning(const char* warningString) {
    }

    void draw3dText(const btVector3& location, const char* textString) {
    }

    void setDebugMode(int debugMode) {
      mode = debugMode;
    }

    int getDebugMode() const {
      return mode;
    }

    World* world;
    int mode;
};

class PhysicsSystem : public System {
  public:
    explicit PhysicsSystem(World* world)
      : debugDrawer(world) {
      assert(this);
      assert(world);

      collisionConfiguration.reset(new btDefaultCollisionConfiguration());
      dispatcher.reset(new btCollisionDispatcher(collisionConfiguration.get()));
      overlappingPairCache.reset(new btDbvtBroadphase());
      solver.reset(new btSequentialImpulseConstraintSolver);
      dynamicsWorld.reset(new btDiscreteDynamicsWorld(dispatcher.get(),
                          overlappingPairCache.get(),
                          solver.get(),
                          collisionConfiguration.get()));
      /// @todo load this from file instead.
      dynamicsWorld->setGravity(btVector3(0, -10, 0));

      for (const auto & p : world->getCollisionMesh().parts) {
        std::shared_ptr<StaticMesh> m(new StaticMesh(p, dynamicsWorld));
        staticmeshes.push_back(m);
      }

      dynamicsWorld->setDebugDrawer(&debugDrawer);

      assert(globalInstance == 0);
      globalInstance = this;
    }

    ~PhysicsSystem() {
      assert(globalInstance == this);
      globalInstance = 0;
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
      PhysicsType* pt = static_cast<PhysicsType*>(type);
      std::size_t index = objects.size();
      lookup.insert(std::make_pair(entity, index));
      objects.push_back(PhysicsObject(entity, dynamicsWorld, *pt));
    }

    void step(float dt) {
      assert(this);
      dynamicsWorld->stepSimulation(dt, 10);
      for (auto & o : objects) {
        o.update();
      }
      bool debugPhysics = true;
      TWEAK(debugPhysics);
      if (debugPhysics) {
        dynamicsWorld->debugDrawWorld();
      }
    }

    PhysicsObject* getObject(Entity* ent) {
      auto res = lookup.find(ent);
      if (res == lookup.end()) {
        return 0;
      }
      return &objects[res->second];
    }

    static PhysicsSystem* globalInstance;

  private:
    // collision configuration contains default setup for memory, collision
    // setup. Advanced users can create their own configuration.
    std::shared_ptr<btDefaultCollisionConfiguration> collisionConfiguration;

    // use the default collision dispatcher. For parallel processing you can
    // use a different dispatcher (see Extras/BulletMultiThreaded)
    std::shared_ptr<btCollisionDispatcher> dispatcher;

    // btDbvtBroadphase is a good general purpose broad phase. You can also try
    // out btAxis3Sweep.
    std::shared_ptr<btBroadphaseInterface> overlappingPairCache;

    // the default constraint solver. For parallel processing you can use a
    // different solver (see Extras/BulletMultiThreaded)
    std::shared_ptr<btSequentialImpulseConstraintSolver> solver;

    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;

    std::vector<std::shared_ptr<PhysicsType> > types;
    std::vector<PhysicsObject> objects;
    std::vector<std::shared_ptr<StaticMesh>> staticmeshes;

    std::map<Entity*, std::size_t> lookup;
    DebugDrawing debugDrawer;
};

PhysicsSystem* PhysicsSystem::globalInstance = 0;

void AddPhysicsCallback(CreateSystemArg arg, Json::Value data) {
  assert(arg.container);
  std::shared_ptr<System> sys(new PhysicsSystem(arg.world));
  arg.container->add(PhysicsSystemType, sys);
}

void Entity_AddPhysics(SystemCreatorList* sc) {
  sc->add(PhysicsSystemType, AddPhysicsCallback);
}

//////////////////////////////////////////////////////////////////////////

namespace scriptingphysics {
  // -- Module: Physics
  const std::string LUA_MODULE_NAME = "Physics";
  // -- Function: GetPhysics
  // -- Description:
  // -- Gets the physics object of a entity.
  // -- Arguments:
  // -- Entity The entity to get the physics from
  // -- Returns:
  // -- PhsyObj The physics object.
  void GetPhysics(ScriptParams* params) {
    assert(params);
    Entity* entity = 0;

    if (ScriptOverload(params) << cLightUserData(&entity)) {
      if (PhysicsSystem::globalInstance == 0) {
        throw std::logic_error("Physics system is not initialized.");
      }
      PhysicsObject* obj = PhysicsSystem::globalInstance->getObject(entity);
      params->returnvar(obj);
    }
  }
  REGISTER_SCRIPT_FUNCTION("Get", GetPhysics);

  // -- Function: ApplyForce
  // -- Description:
  // -- Applies force to a physics object by providing the values
  // -- Arguments:
  // -- PhysObj The physics object
  // -- Float The x axis of the force
  // -- Float The y axis of the force
  // -- Float The z axis of the force
  // -- Description:
  // -- Applies force to a physics object by providing a force vector
  // -- Arguments:
  // -- PhysObj The physics object
  // -- vec3 The force
  void ApplyForce(ScriptParams* params) {
    assert(params);
    PhysicsObject* obj = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    vec3* force = 0;

    if (ScriptOverload(params) << cLightUserData(&obj) << &x << &y << &z) {
      assert(obj);
      assert(obj->body);
      obj->body->applyCentralForce(btVector3(x, y, z));
    } else if (ScriptOverload(params) << cLightUserData(&obj)
               << mFullUserData(vec3, &force)) {
      assert(obj);
      assert(obj->body);
      assert(force);
      obj->body->applyCentralForce(btVector3((*force)[0], (*force)[1],
                                             (*force)[2]));
    }
  }
  REGISTER_SCRIPT_FUNCTION("ApplyForce", ApplyForce);

  // -- Function: ApplyTorque
  // -- Description:
  // -- Applies torque to a physics object by providing the values
  // -- Arguments:
  // -- PhysObj The physics object
  // -- Float The x axis of the torque
  // -- Float The y axis of the torque
  // -- Float The z axis of the torque
  // -- Description:
  // -- Applies torque to a physics object by providing a torque vector
  // -- Arguments:
  // -- PhysObj The physics object
  // -- vec3 The torque
  void ApplyTorque(ScriptParams* params) {
    assert(params);
    PhysicsObject* obj = 0;
    float x = 0;
    float y = 0;
    float z = 0;
    vec3* force = 0;

    if (ScriptOverload(params) << cLightUserData(&obj) << &x << &y << &z) {
      assert(obj);
      assert(obj->body);
      obj->body->applyTorque(btVector3(x, y, z));
    } else if (ScriptOverload(params) << cLightUserData(&obj)
               << mFullUserData(vec3, &force)) {
      assert(obj);
      assert(obj->body);
      assert(force);
      obj->body->applyTorque(btVector3((*force)[0], (*force)[1],
                                       (*force)[2]));
    }
  }
  REGISTER_SCRIPT_FUNCTION("ApplyTorque", ApplyTorque);

}  // namespace scriptingphysics
