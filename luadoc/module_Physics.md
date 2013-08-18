Module: Physics
===========

[Index](index.md)


Physics.GetPhysics()
-----------

Gets the physics object of a entity.

###Arguments:
  - [Entity](type_Entity.md) The entity to get the physics from

####Returns:
  - **PhsyObj** The physics object.



Physics.ApplyForce()
-----------

Applies force to a physics object by providing the values

###Arguments:
  - **PhysObj** The physics object
  - **Number** The x axis of the force
  - **Number** The y axis of the force
  - **Number** The z axis of the force



Physics.ApplyForce()
-----------

Applies force to a physics object by providing a force vector

###Arguments:
  - **PhysObj** The physics object
  - [vec3](type_vec3.md) The force



Physics.ApplyTorque()
-----------

Applies torque to a physics object by providing the values

###Arguments:
  - **PhysObj** The physics object
  - **Number** The x axis of the torque
  - **Number** The y axis of the torque
  - **Number** The z axis of the torque



Physics.ApplyTorque()
-----------

Applies torque to a physics object by providing a torque vector

###Arguments:
  - **PhysObj** The physics object
  - [vec3](type_vec3.md) The torque



Physics.GetOrientation()
-----------

Gets the orientation of a physics object.

###Arguments:
  - **PhysObj** The physics object

####Returns:
  - [quat](type_quat.md) The orientation



Physics.SetOrientation()
-----------

Gets the orientation of a physics object.

###Arguments:
  - **PhysObj** The physics object

####Returns:
  - [quat](type_quat.md) The orientation


