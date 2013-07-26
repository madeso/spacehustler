All functions
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


