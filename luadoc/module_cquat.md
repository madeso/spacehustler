Module: cquat
===========

[Index](index.md)


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


