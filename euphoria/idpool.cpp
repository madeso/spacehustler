// Euphoria - Copyright (c) Gustav

#include "euphoria/idpool.h"

IdGenerator::IdGenerator()
  : current(1) {
}

const IdGenerator::ID IdGenerator::generate() {
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

void IdGenerator::release(const IdGenerator::ID id) {
  released.push_back(id);
}

Id::Id(IdGenerator* generator)
  : value(generator->generate())
  , generator(generator) {
}

Id::~Id() {
  generator->release(value);
}
