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
    const ID generate();

    /** Release a id that is no longer required.
    @see Id
    @param id the id to be removed.
     */
    void release(const ID id);
  private:
    ID current;
    std::vector<ID> released;
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

    /** The actual value of the id.
     */
    const IdGenerator::ID value;
  private:
    IdGenerator* generator;
};

#endif  // EUPHORIA_ID_H_
