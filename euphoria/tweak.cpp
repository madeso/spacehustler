// Euphoria - Copyright (c) Gustav

#include "euphoria/tweak.h"

#include <AntTweakBar.h>

#include <cassert>
#include <string>
#include <algorithm>

namespace {
  TweakerStore* GlocalTweakerStoreVariable = 0;
}  // namespace

Tweakable::Tweakable(TwBar* bar, const std::string& name)
  : bar(bar), hasChanged(false), life(0) {
  assert(this);
  id = "t_" + name;
}

Tweakable::~Tweakable() {
}

Tweakable& Tweakable::label(const std::string& value) {
  assert(this);
  TwSetParam(bar, id.c_str(), "label", TW_PARAM_CSTRING, 1, value.c_str());
  return *this;
}

Tweakable& Tweakable::group(const std::string& value) {
  assert(this);
  TwSetParam(bar, id.c_str(), "group", TW_PARAM_CSTRING, 1, value.c_str());
  return *this;
}

Tweakable& Tweakable::readonly(bool readonly) {
  assert(this);
  int param = readonly ? 1 : 0;
  TwSetParam(bar, id.c_str(), "readonly", TW_PARAM_INT32, 1, &param);
  return *this;
}

Tweakable::operator bool() const {
  return true;
}

namespace {
  template<typename TTweakable, typename TType>
  void TW_CALL SetCallback(const void* value, void* clientData) {
    TTweakable* tweak = static_cast<TTweakable*>(clientData);
    tweak->data =
      (*static_cast<const TType*>(value));
    tweak->hasChanged = true;
  }

  // for std::strings you should use GetCallBackString instead
  template<typename TTweakable, typename TType>
  void TW_CALL GetCallback(void* value, void* clientData) {
    TTweakable* tweak = static_cast<TTweakable*>(clientData);
    *static_cast<TType*>(value) = tweak->data;
  }

  void TW_CALL GetCallbackString(void* value, void* clientData);

  class StringTweakable : public Tweakable {
    public:
      std::string data;

      StringTweakable(TwBar* bar, const std::string& name)
        : Tweakable(bar, name) {
        assert(this);
        //  TwAddVarRW(bar, id.c_str(), TW_TYPE_STDSTRING, &string, "");
        TwAddVarCB(bar, id.c_str(), TW_TYPE_STDSTRING,
                   SetCallback<StringTweakable, std::string>,
                   GetCallbackString, this, "");
        label(name);
      }
  };

  void TW_CALL GetCallbackString(void* value, void* clientData) {
    StringTweakable* tweak = static_cast<StringTweakable*>(clientData);
    TwCopyStdStringToLibrary(*static_cast<std::string*>(value), tweak->data);
  }

  bool IsDead(const TweakerStore::Tweakables::value_type& x) {
    return x.second->life > 3;
  }
}  // namespace


TweakerStore::TweakerStore() {
  assert(this);
  if (GlocalTweakerStoreVariable == 0) {
    GlocalTweakerStoreVariable = this;
  }
  bar = TwNewBar("Testing");
}


TweakerStore::~TweakerStore() {
  assert(this);
  if (GlocalTweakerStoreVariable == this) {
    GlocalTweakerStoreVariable = 0;
  }
  TwDeleteBar(bar);
}

namespace {
  template <typename TSpecifiedTweakable, typename TData>
  Tweakable& Tweakbase(TweakerStore::Tweakables* tweakables, TwBar* bar,
                       const std::string& name, TData* data) {
    auto found = tweakables->find(name);
    Tweakable* tweakable = 0;
    if (found != tweakables->end()) {
      tweakable = found->second.get();
    } else {
      boost::shared_ptr<Tweakable> newtweakable(new
          TSpecifiedTweakable(bar, name));
      tweakables->insert(TweakerStore::Tweakables::
                         value_type(name, newtweakable));
      tweakable = newtweakable.get();
    }

    assert(tweakable);

    StringTweakable* spec = reinterpret_cast<StringTweakable*>(tweakable);

    if (spec->hasChanged) {
      *data = spec->data;
    } else {
      spec->data = *data;
    }
    spec->life = 0;

    return *spec;
  }
}  // namespace

Tweakable& TweakerStore::tweak(const std::string& name, std::string* data) {
  assert(this);
  return Tweakbase<StringTweakable, std::string>(&tweakables, bar, name, data);
}

/** @todo move to a better place
 */
template< typename ContainerT, typename PredicateT >
void erase_if(ContainerT& items, const PredicateT& predicate) {
  for (auto it = items.begin(); it != items.end();) {
    if (predicate(*it)) {
      it = items.erase(it);
    } else {
      ++it;
    }
  }
};

void TweakerStore::update() {
  assert(this);
  for (auto tweak : tweakables) {
    tweak.second->life += 1;
  }
  erase_if(tweakables, IsDead);
}

TweakerStore* GlocalTweakerStore() {
  return GlocalTweakerStoreVariable;
}

/*

TwBar* bar;
bar = TwNewBar("Testing tweaks");

bool b = false;
TwAddVarRW(bar, "b", TW_TYPE_BOOLCPP, &b, "");

double tmpt = 30.0;
TwAddVarRW(bar, "Temperature", TW_TYPE_DOUBLE, &tmpt, " precision=3 ");

std::string s3 = "a STL string";
TwAddVarRW(bar, "s3", TW_TYPE_STDSTRING, &s3, "");

*/
