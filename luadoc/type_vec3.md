Type: vec3
===========

[Index](index.md)


Operator .
-----------

Get or sets a value from the vector

###Arguments:
  - **String** the name, x for x, y for z for z

####Returns:
  - **Number** the value of x, y or z



Operator .
-----------

Getor sets a value from the vector

###Arguments:
  - **Number** the index between 1 and 3 that indicates the x,y or z value

####Returns:
  - **Number** The value



Operator unary -
-----------

Returns a negative representation of the vector

###Arguments:
  - [vec3](type_vec3.md) the vector to negate

####Returns:
  - [vec3](type_vec3.md) the negated vector



Operator +
-----------

Add two vectors.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - [vec3](type_vec3.md) the right operand

####Returns:
  - [vec3](type_vec3.md) the sum of the 2 vectors.



Operator +
-----------

Add a vector and a number.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - **Number** the number to add

####Returns:
  - [vec3](type_vec3.md) the sum of the vector and the float



Operator -
-----------

Substract two vectors.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - [vec3](type_vec3.md) the right operand

####Returns:
  - [vec3](type_vec3.md) the result.



Operator -
-----------

Subtract a vector and a number.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - **Number** the number to subtract

####Returns:
  - [vec3](type_vec3.md) the result



Operator *
-----------

Multiply a vector and a number.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - **float** the number to multiply with

####Returns:
  - [vec3](type_vec3.md) the result



Operator /
-----------

Divide a vector and a number.

###Arguments:
  - [vec3](type_vec3.md) the left operand
  - **float** the number to divide

####Returns:
  - [vec3](type_vec3.md) the result



length()
-----------

Gets the length of the vector.

###Arguments:
  - [vec3](type_vec3.md) the vector

####Returns:
  - **Number** the length



length2()
-----------

Returns the squared length.

###Arguments:
  - [vec3](type_vec3.md) the vector

####Returns:
  - **Number** the length squared



zero()
-----------

Sets all values to 0

###Arguments:
  - [vec3](type_vec3.md) the vector to clear.



set()
-----------

sets the value of all values

###Arguments:
  - **Number** the X
  - **Number** the Y
  - **Number** the Z



set()
-----------

sets the values to a single value

###Arguments:
  - **Number** the the value of X, Y and Z



maximize()
-----------

Sets the vector to the pairwise maximum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - **Number** the X
  - **Number** the Y
  - **Number** the Z



maximize()
-----------

Sets the vector to the pairwise maximum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - **Number** the X, Y and Z



maximize()
-----------

Sets the vector to the pairwise maximum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - [vec3](type_vec3.md) the other vector



maximize()
-----------

Sets the vector to the pairwise minimum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - **Number** the X
  - **Number** the Y
  - **Number** the Z



maximize()
-----------

Sets the vector to the pairwise minimum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - **Number** the X, Y and Z



maximize()
-----------

Sets the vector to the pairwise minimum.

###Arguments:
  - [vec3](type_vec3.md) the vector
  - [vec3](type_vec3.md) the other vector



toString()
-----------

Generate a string representation of the vector.

###Arguments:
  - [vec3](type_vec3.md) the vector

####Returns:
  - **string** the resulting string



dot()
-----------

Calculates the dot product

###Arguments:
  - [vec3](type_vec3.md) the lhs
  - [vec3](type_vec3.md) the rhs

####Returns:
  - **Number** the dot product between lhs and rhs



cross()
-----------

Calculates the cross product

###Arguments:
  - [vec3](type_vec3.md) the lhs
  - [vec3](type_vec3.md) the rhs
  - [vec3](type_vec3.md) the lhs
  - [vec3](type_vec3.md) the rhs

####Returns:
  - [vec3](type_vec3.md) the cross product between lhs and rhs
  - **Number** the normalized cross product of two vec3
  - **Number** the normalized cross product of lhs and rhs



rotate()
-----------

Rotate a vector around a unitlength axis

###Arguments:
  - [vec3](type_vec3.md) the vector
  - [vec3](type_vec3.md) the axis
  - **Number** the angle in radians



angle()
-----------

Returnsd the unsigned angle between 2 vectors ofarbitary length

###Arguments:
  - [vec3](type_vec3.md) the first vector
  - [vec3](type_vec3.md) the second vector

####Returns:
  - **Number** the angle in radians



angle()
-----------

Returns the signed angle between 2 vectors relativeto a third.

###Arguments:
  - [vec3](type_vec3.md) the first vector
  - [vec3](type_vec3.md) the second vector
  - [vec3](type_vec3.md) the reference vector

####Returns:
  - **Number** the singed angle in radians, measured from the first vector



normalize()
-----------

Normalize the vector.

###Arguments:
  - [vec3](type_vec3.md) the vector to normalize



getNormalized()
-----------

Gets the normalized vector without modifyingthe argument

###Arguments:
  - [vec3](type_vec3.md) the vector to normalize

####Returns:
  - [vec3](type_vec3.md) the normalized vector


