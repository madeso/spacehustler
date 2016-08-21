// Euphoria - Copyright (c) Gustav

#include "euphoria/tweak.h"

#ifdef USE_TWEAKABLES

#include <imgui/imgui.h>

namespace { enum { MAX_STRING_SIZE = 255}; }

namespace euphoria {

tweaks::Tweakable::Tweakable(const std::string& label) : label_(label) {
}

tweaks::Tweakable::~Tweakable() {}

const std::string& tweaks::Tweakable::label() const {
  return label_;
}
tweaks::Tweakable& tweaks::Tweakable::set_label(const std::string& label) {
  label_ = label;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace {
TweakerStore*& StorePtr() {
  static TweakerStore* store = NULL;
  return store;
}

TweakerStore& Store() {
  TweakerStore* store = StorePtr();
  assert(store);
  return *store;
}

template<typename T>
T& Tweak(T* t) {
  std::shared_ptr<tweaks::Tweakable> s(t);
  Store().Add(s);
  return *t;
}

}  // namespace


TweakerStore::TweakerStore() {
  assert(StorePtr() == NULL);
  StorePtr() = this;
}

TweakerStore::~TweakerStore() {
  assert(StorePtr() == this);
  StorePtr() = NULL;
}

void TweakerStore::Add(std::shared_ptr<tweaks::Tweakable> t) {
  tweaks_.push_back(t);
}

void TweakerStore::RunAll() {
  for(auto& t : tweaks_) {
    t->Run();
  }
  tweaks_.resize(0);
}

void RunAllTweaks() {
  Store().RunAll();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tweaks::TweakableString::TweakableString(const std::string& label, std::string* str) : Tweakable(label), str_(str) {
}

tweaks::TweakableString::~TweakableString() {
}

void tweaks::TweakableString::Run() {
  char str[MAX_STRING_SIZE] = {0,};
  strcpy(str, str_->c_str());
  ImGui::InputText(label().c_str(), str, MAX_STRING_SIZE);
  *str_ = str;
}

tweaks::TweakableString& Tweak(const std::string& name, std::string* data) {
  return Tweak(new tweaks::TweakableString(name, data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

tweaks::TweakableVec3::TweakableVec3(const std::string& label, Vec3* vec) : Tweakable(label), vec_(vec), readonly_(false) {
}

tweaks::TweakableVec3::~TweakableVec3() {
}

void tweaks::TweakableVec3::Run() {
  int flags = 0;
  if( readonly_ ) flags |= ImGuiInputTextFlags_ReadOnly;
  ImGui::InputFloat3(label().c_str(), vec_->data(), -1, flags);
}

tweaks::TweakableVec3& tweaks::TweakableVec3::readonly() {
  readonly_ = true;
  return *this;
}

tweaks::TweakableVec3& Tweak(const std::string& name, Vec3* data) {
  return Tweak(new tweaks::TweakableVec3(name, data));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


tweaks::TweakableBool::TweakableBool(const std::string& label, bool* b) : Tweakable(label), bool_(b) {
}

tweaks::TweakableBool::~TweakableBool() {
}

void tweaks::TweakableBool::Run() {
  ImGui::Checkbox(label().c_str(), bool_);
}


tweaks::TweakableBool& Tweak(const std::string& name, bool* data) {
  return Tweak(new tweaks::TweakableBool(name, data));
}

}  // namespace euphoria

#endif
