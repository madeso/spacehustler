All functions
===========

[Index](index.md)

quat.getx()
-----------

Gets the x base vector

###Arguments:
  - [quat](type_quat.md) The quaternion

####Returns:
  - [vec3](type_vec3.md) the basis vector



quat.gety()
-----------

Gets the y base vector

###Arguments:
  - [quat](type_quat.md) The quaternion

####Returns:
  - [vec3](type_vec3.md) the basis vector



quat.getz()
-----------

Gets the z base vector

###Arguments:
  - [quat](type_quat.md) The quaternion

####Returns:
  - [vec3](type_vec3.md) the basis vector



quat.Operator unary -
-----------

Gets the conjugate of the quaterion

###Arguments:
  - [quat](type_quat.md) The quaternion

####Returns:
  - [quat](type_quat.md) the conjugate



Game.Quit()
-----------

Quits the game.



Entity.GetPosition()
-----------

Gets the position a entity.

###Arguments:
  - [Entity](type_Entity.md) The entity

####Returns:
  - [vec3](type_vec3.md) the position



Entity.SetPosition()
-----------

Sets the position a entity.

###Arguments:
  - [Entity](type_Entity.md) The entity
  - [vec3](type_vec3.md) the position



Entity.GetRotation()
-----------

Gets the rotation a entity.

###Arguments:
  - [Entity](type_Entity.md) The entity

####Returns:
  - [quat](type_quat.md) the rotation



Entity.SetRotation()
-----------

Sets the rotation a entity.

###Arguments:
  - [Entity](type_Entity.md) The entity
  - [quat](type_quat.md) the rotation



Json.Get()
-----------

Gets a value from the json data

###Arguments:
  - [Json](type_Json.md) the json data to get the value from
  - **String** the name of the variable
  - **Number** the default

####Returns:
  - **Number** the value or the default



cquat.identity()
-----------

Creates a identity quaternion

####Returns:
  - [quat](type_quat.md) the quaternion



cquat.worldx()
-----------

Creates a rotation quaternion around world x

###Arguments:
  - **Number** the angle in radians

####Returns:
  - [quat](type_quat.md) the quaternion



cquat.worldy()
-----------

Creates a rotation quaternion around world y

###Arguments:
  - **Number** the angle in radians

####Returns:
  - [quat](type_quat.md) the identity quaternion



cquat.worldz()
-----------

Creates a rotation quaternion around world z

###Arguments:
  - **Number** the angle in radians

####Returns:
  - [quat](type_quat.md) the quaternion



cquat.axisangle()
-----------

Creates a rotation quaternion from axis and angle

###Arguments:
  - [vec3](type_vec3.md) the axis
  - **Number** the angle in radians

####Returns:
  - [quat](type_quat.md) the quaternion



cquat.yawpitchroll()
-----------

Creates a rotation quaternion from yaw, pitch and roll

###Arguments:
  - [vec3](type_vec3.md) the axis
  - **Number** the yaw
  - **Number** the pitch
  - **Number** the roll

####Returns:
  - [quat](type_quat.md) the quaternion



cvec3.create()
-----------

Creates a 3 dimensional vector

###Arguments:
  - **Number** The X coordinate
  - **Number** The Y coordinate
  - **Number** The Z coordinate

####Returns:
  - [vec3](type_vec3.md) a vector



cvec3.create()
-----------

Creates a vector at origin

####Returns:
  - [vec3](type_vec3.md) a vector placed at origin



cvec3.cardinal()
-----------

Create a cardinal vector.

###Arguments:
  - **Number** The cardinal vector index, pass 1 to create (1,0,0) etc.

####Returns:
  - **The** created vector.



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



Physics.SetOrientation()
-----------

Gets the orientation of a physics object.

###Arguments:
  - **PhysObj** The physics object

####Returns:
  - [quat](type_quat.md) The orientation


