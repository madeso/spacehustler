Module: Physics
===========

[Index](index.md)


Physics.GetPhysics()
-----------

Gets the physics object of a entity.

###Arguments:
  - **Entity** The entity to get the physics from

####Returns:
  - **PhsyObj** The physics object.



Physics.ApplyForce()
-----------

Applies force to a physics object by providing the values

###Arguments:
  - **PhysObj** The physics object
  - **Float** The x axis of the force
  - **Float** The y axis of the force
  - **Float** The z axis of the force



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
  - **Float** The x axis of the torque
  - **Float** The y axis of the torque
  - **Float** The z axis of the torque



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


