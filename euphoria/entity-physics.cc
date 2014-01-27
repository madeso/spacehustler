  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

#include "euphoria/entity-physics.h"
#include <cassert>
#include <string>
#include <vector>
#include <map>

#include "euphoria/entity.h"
#include "euphoria/world.h"
#include "euphoria/tweak.h"
#include "euphoria/script.h"
#include "euphoria/scriptlib.h"

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

class PhysicsObject;

class CollisionInfo {
  public:
    CollisionInfo()
      : distance_(0.0f), impulse_(0.0f), other_(NULL) {
    }

    CollisionInfo(float distance, float impulse, PhysicsObject* other)
      : distance_(distance), impulse_(distance), other_(other) {
    }

  private:
    float distance_;
    float impulse_;
    PhysicsObject* other_;
};

class PhysicsObject {
  public:
    PhysicsObject(Entity* entity,
                  std::shared_ptr<btDiscreteDynamicsWorld> world,
                  const PhysicsType& data, size_t index)
      : entity(entity)
      , dynamicsWorld(world)
      , index_(new size_t(index)) {
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
      body->setUserPointer(index_.get());

      // add the body to the dynamics world
      connection.reset(new WorldBodyConenction(dynamicsWorld, body));
    }

    void Update() {
      collisioninfo.resize(0);
      btTransform trans;
      body->getMotionState()->getWorldTransform(trans);
      assert(entity);
      entity->position = C(trans.getOrigin());
      entity->rotation = C(trans.getRotation());
    }

    void addCollision(float distance, float impulse, PhysicsObject* other) {
      collisioninfo.push_back(CollisionInfo(distance, impulse, other));
    }

    Entity* entity;
    std::shared_ptr<btDiscreteDynamicsWorld> dynamicsWorld;
    std::shared_ptr<btCollisionShape> shape;
    std::shared_ptr<btDefaultMotionState> myMotionState;
    std::shared_ptr<btRigidBody> body;
    std::shared_ptr<WorldBodyConenction> connection;
    std::vector<CollisionInfo> collisioninfo;
    std::shared_ptr<size_t> index_;
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
        const vec3 v1 = part.GetVertex(p1);
        const vec3 v2 = part.GetVertex(p2);
        const vec3 v3 = part.GetVertex(p3);
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
      : world_(w)
      , mode_(DBG_DrawWireframe | DBG_DrawAabb | DBG_DrawFeaturesText
              | DBG_DrawContactPoints | DBG_NoDeactivation | DBG_NoHelpText
              | DBG_DrawText | DBG_ProfileTimings | DBG_EnableSatComparison
              | DBG_DisableBulletLCP | DBG_EnableCCD | DBG_DrawConstraints
              | DBG_DrawConstraintLimits | DBG_FastWireframe
              | DBG_DrawNormals) {
      assert(w);
    }
    void drawLine(const btVector3& from, const btVector3& to,
                  const btVector3& color) {
      assert(this);
      assert(world_);
      world_->debug_renderer().Line(C(from), C(to), ToColor(color));
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
      mode_ = debugMode;
    }

    int getDebugMode() const {
      return mode_;
    }

  private:
    World* world_;
    int mode_;
};

class PhysicsSystem : public System {
  public:
    explicit PhysicsSystem(World* world)
      : debug_drawer_(world) {
      assert(this);
      assert(world);

      collision_configuration_.reset(new btDefaultCollisionConfiguration());
      dispatcher_.reset(new btCollisionDispatcher(collision_configuration_
                        .get()));
      overlapping_pair_cache_.reset(new btDbvtBroadphase());
      solver_.reset(new btSequentialImpulseConstraintSolver);
      dynamics_world_.reset(new btDiscreteDynamicsWorld(dispatcher_.get(),
                            overlapping_pair_cache_.get(),
                            solver_.get(),
                            collision_configuration_.get()));
      /// @todo load this from file instead.
      dynamics_world_->setGravity(btVector3(0, -10, 0));

      for (const auto& p : world->collisionmesh().parts) {
        std::shared_ptr<StaticMesh> m(new StaticMesh(p, dynamics_world_));
        staticmeshes_.push_back(m);
      }

      dynamics_world_->setDebugDrawer(&debug_drawer_);

      assert(globalInstance == 0);
      globalInstance = this;
    }

    ~PhysicsSystem() {
      assert(globalInstance == this);
      globalInstance = 0;
    }

    ComponentType* AddType(const Json::Value& data) {
      assert(this);
      std::shared_ptr<PhysicsType> type(new PhysicsType(data));
      types_.push_back(type);
      return type.get();
    }

    virtual void AddComponent(Entity* entity, ComponentType* type) {
      assert(this);
      assert(entity);
      assert(type);
      PhysicsType* pt = static_cast<PhysicsType*>(type);
      std::size_t index = objects_.size();
      lookup_.insert(std::make_pair(entity, index));
      objects_.push_back(PhysicsObject(entity, dynamics_world_, *pt,
                                       objects_.size()));
    }

    void Step(float dt) {
      assert(this);
      dynamics_world_->stepSimulation(dt, 10);
      for (auto& o : objects_) {
        o.Update();
      }
      bool debugPhysics = false;
      TWEAK(debugPhysics);
      if (debugPhysics) {
        dynamics_world_->debugDrawWorld();
      }

      int numManifolds = dynamics_world_->getDispatcher()->getNumManifolds();
      for (int i = 0; i < numManifolds; i++) {
        btPersistentManifold* contactManifold =  dynamics_world_->
            getDispatcher()->getManifoldByIndexInternal(i);

        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        void* userDataA = obA->getUserPointer();
        void* userDataB = obB->getUserPointer();

        PhysicsObject* physA = userDataA ? &objects_[*static_cast<size_t*>
                               (userDataA)] : NULL;
        PhysicsObject* physB = userDataB ? &objects_[*static_cast<size_t*>
                               (userDataB)] : NULL;

        int numContacts = contactManifold->getNumContacts();
        for (int j = 0; j < numContacts; j++) {
          btManifoldPoint& pt = contactManifold->getContactPoint(j);
          if (pt.getDistance() < 0.f) {
            const btVector3& ptA = pt.getPositionWorldOnA();
            const btVector3& ptB = pt.getPositionWorldOnB();
            const btVector3& normalOnB = pt.m_normalWorldOnB;
            if (physA) {
              physA->addCollision(pt.m_distance1, pt.m_appliedImpulse, physB);
            }
            if (physB) {
              physA->addCollision(pt.m_distance1, pt.m_appliedImpulse, physA);
            }
          }
        }
      }
    }

    PhysicsObject* getObject(Entity* ent) {
      auto res = lookup_.find(ent);
      if (res == lookup_.end()) {
        return 0;
      }
      return &objects_[res->second];
    }

    static PhysicsSystem* globalInstance;

  private:
    // collision configuration contains default setup for memory, collision
    // setup. Advanced users can create their own configuration.
    std::shared_ptr<btDefaultCollisionConfiguration> collision_configuration_;

    // use the default collision dispatcher. For parallel processing you can
    // use a different dispatcher (see Extras/BulletMultiThreaded)
    std::shared_ptr<btCollisionDispatcher> dispatcher_;

    // btDbvtBroadphase is a good general purpose broad phase. You can also try
    // out btAxis3Sweep.
    std::shared_ptr<btBroadphaseInterface> overlapping_pair_cache_;

    // the default constraint solver. For parallel processing you can use a
    // different solver (see Extras/BulletMultiThreaded)
    std::shared_ptr<btSequentialImpulseConstraintSolver> solver_;

    std::shared_ptr<btDiscreteDynamicsWorld> dynamics_world_;

    std::vector<std::shared_ptr<PhysicsType> > types_;
    std::vector<PhysicsObject> objects_;
    std::vector<std::shared_ptr<StaticMesh>> staticmeshes_;

    std::map<Entity*, std::size_t> lookup_;
    DebugDrawing debug_drawer_;
};

PhysicsSystem* PhysicsSystem::globalInstance = 0;

void AddPhysicsCallback(const CreateSystemArg& arg, Json::Value data) {
  assert(arg.container);
  std::shared_ptr<System> sys(new PhysicsSystem(arg.world));
  arg.container->Add(PhysicsSystemType, sys);
}

void Entity_AddPhysics(SystemCreatorList* sc) {
  sc->Add(PhysicsSystemType, AddPhysicsCallback);
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
      params->Return(obj);
    }
  }
  REGISTER_SCRIPT_FUNCTION("Get", GetPhysics);

  // -- Function: ApplyForce
  // -- Description:
  // -- Applies force to a physics object by providing the values
  // -- Arguments:
  // -- PhysObj The physics object
  // -- Number The x axis of the force
  // -- Number The y axis of the force
  // -- Number The z axis of the force
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
  // -- Number The x axis of the torque
  // -- Number The y axis of the torque
  // -- Number The z axis of the torque
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

  // -- Function: GetOrientation
  // -- Description:
  // -- Gets the orientation of a physics object.
  // -- Arguments:
  // -- PhysObj The physics object
  // -- Returns:
  // -- quat The orientation
  void GetOrientation(ScriptParams* params) {
    assert(params);
    PhysicsObject* obj = 0;

    if (ScriptOverload(params) << cLightUserData(&obj)) {
      assert(obj);
      assert(obj->body);
      const btQuaternion orientation = obj->body->getOrientation();
      quat* rot = ReturnQuat(params);
      *rot = C(orientation);
    }
  }
  REGISTER_SCRIPT_FUNCTION("GetOrientation", GetOrientation);

  // -- Function: SetOrientation
  // -- Description:
  // -- Gets the orientation of a physics object.
  // -- Arguments:
  // -- PhysObj The physics object
  // -- Returns:
  // -- quat The orientation
  void SetOrientation(ScriptParams* params) {
    assert(params);
    PhysicsObject* obj = 0;
    quat* rot;

    if (ScriptOverload(params) << cLightUserData(&obj)
        << mFullUserData(quat, &rot)) {
      assert(obj);
      assert(obj->body);
      assert(rot);

      btTransform trans = obj->body->getWorldTransform();
      trans.setRotation(C(*rot));
      obj->body->setWorldTransform(trans);
    }
  }
  REGISTER_SCRIPT_FUNCTION("SetOrientation", SetOrientation);

}  // namespace scriptingphysics
