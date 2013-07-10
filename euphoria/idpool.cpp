// Euphoria - Copyright (c) Gustav

#include "euphoria/idpool.h"

IdPool::IdPool()
  : current(1) {
}

const IdPool::ID IdPool::generate() {
  if (released.empty()) {
    const ID value = current;
    ++current;
    return value;
  } else {
    const ID value = *released.rbegin();
    released.pop_back();
    return value;
  }
}

void IdPool::release(const IdPool::ID id) {
  released.push_back(id);
}

Id::Id(IdPool* pool)
  : value(pool->generate())
  , pool(pool) {
}

Id::~Id() {
  pool->release(value);
}
