// Euphoria - Copyright (c) Gustav

#include "euphoria/id.h"

IdGenerator::IdGenerator() : current_(1) {}

const IdGenerator::ID IdGenerator::Generate() {
  if (released_.empty()) {
    const ID value = current_;
    ++current_;
    return value;
  } else {
    const ID value = *released_.rbegin();
    released_.pop_back();
    return value;
  }
}

void IdGenerator::Release(const IdGenerator::ID id) { released_.push_back(id); }

Id::Id(IdGenerator* generator)
    : value_(generator->Generate()), generator_(generator) {}

Id::~Id() { generator_->Release(value_); }

const IdGenerator::ID Id::value() const { return value_; }
