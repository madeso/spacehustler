// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_IDPOOL_H_
#define EUPHORIA_IDPOOL_H_

#include <boost/noncopyable.hpp>
#include <vector>
#include "euphoria/ints.h"

/** A pool of unique identifiers.
@todo rename to IdGenerator
 */
class IdPool : boost::noncopyable {
  public:
    /** The type containing the id.
     */
    typedef uint32 ID;

    /** Standard constructor.
     */
    IdPool();

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
    @param pool the pool to get the actual id from.
     */
    explicit Id(IdPool* pool);

    /** Recycle the id.
     */
    ~Id();

    /** The actual value of the id.
     */
    const IdPool::ID value;
  private:
    IdPool* pool;
};

#endif  // EUPHORIA_IDPOOL_H_
