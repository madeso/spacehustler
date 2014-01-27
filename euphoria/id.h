// Euphoria - Copyright (c) Gustav

/** @file
Id related code.
 */

#ifndef EUPHORIA_ID_H_
#define EUPHORIA_ID_H_

#include <boost/noncopyable.hpp>
#include <vector>
#include "euphoria/ints.h"

/** A generator for unique identifiers.
 */
class IdGenerator : boost::noncopyable {
 public:
  /** The type containing the id.
   */
  typedef uint32 ID;

  /** Standard constructor.
   */
  IdGenerator();

  /** Generate a new id or recycle a old one.
  @see Id
  @returns the new id.
   */
  const ID Generate();

  /** Release a id that is no longer required.
  @see Id
  @param id the id to be removed.
   */
  void Release(const ID id);

 private:
  ID current_;
  std::vector<ID> released_;
};

/** A id container.
@see IdPool
 */
class Id : boost::noncopyable {
 public:
  /** Create a new id.
  @param generator the pool to get the actual id from.
   */
  explicit Id(IdGenerator* generator);

  /** Recycle the id.
   */
  ~Id();

  /** Get the actual value of the id.
   */
  const IdGenerator::ID value() const;

 private:
  const IdGenerator::ID value_;
  IdGenerator* generator_;
};

#endif  // EUPHORIA_ID_H_
