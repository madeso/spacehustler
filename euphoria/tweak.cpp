// Euphoria - Copyright (c) Gustav

#include "euphoria/tweak.h"

#include <AntTweakBar.h>

#include <cassert>
#include <string>
#include <algorithm>

namespace {
  TweakerStore* GlocalTweakerStoreVariable = 0;
}  // namespace

Tweakable::Tweakable(TwBar* bar, const std::string& id, const std::string& name)
  : bar(bar), hasChanged(false), life(0), id(id) {
  assert(this);
}

Tweakable::~Tweakable() {
  TwRemoveVar(bar, id.c_str());
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
      *static_cast<const TType*>(value);
    tweak->hasChanged = true;
  }

  // for std::strings you should use GetCallBackString instead
  template<typename TTweakable, typename TType>
  void TW_CALL GetCallback(void* value, void* clientData) {
    TTweakable* tweak = static_cast<TTweakable*>(clientData);
    *static_cast<TType*>(value) = tweak->data;
  }

  void TW_CALL GetCallbackString(void* value, void* clientData);

  template <typename TInt, TwType IntType>
  class IntTweakable : public Tweakable {
    public:
      typedef IntTweakable<TInt, IntType> TweakableType;
      TInt data;

      IntTweakable(TwBar* bar, const std::string& id, const std::string& name)
        : Tweakable(bar, id, name)
        , data(0) {
        assert(this);

        TwSetVarCallback set = SetCallback<TweakableType, TInt>;
        TwGetVarCallback get = GetCallback<TweakableType, TInt>;

        TwAddVarCB(bar, id.c_str(), IntType, set, get, this, "");
        label(name);
      }
  };

  class QuatTweakable : public Tweakable {
    public:
      quat data;

      static void TW_CALL QuatSetCallback(const void* value, void* clientData) {
        QuatTweakable* tweak = static_cast<QuatTweakable*>(clientData);
        const float* val = static_cast<const float*>(value);
        tweak->data[0] = val[0];
        tweak->data[1] = val[1];
        tweak->data[2] = val[2];
        tweak->data[3] = val[3];
        tweak->hasChanged = true;
      }

      static void TW_CALL QuatGetCallback(void* value, void* clientData) {
        QuatTweakable* tweak = static_cast<QuatTweakable*>(clientData);
        float* val = static_cast<float*>(value);
        val[0] = tweak->data[0];
        val[1] = tweak->data[1];
        val[2] = tweak->data[2];
        val[3] = tweak->data[3];
      }

      QuatTweakable(TwBar* bar, const std::string& id, const std::string& name)
        : Tweakable(bar, id, name)
        , data() {
        assert(this);

        TwSetVarCallback set = QuatSetCallback;
        TwGetVarCallback get = QuatGetCallback;

        TwAddVarCB(bar, id.c_str(), TW_TYPE_QUAT4F, set, get, this,
                   "axisx=x axisy=y axisz=-z");
        label(name);
      }
  };

  class StringTweakable : public Tweakable {
    public:
      std::string data;

      StringTweakable(TwBar* bar, const std::string& id,
                      const std::string& name)
        : Tweakable(bar, id, name) {
        assert(this);

        TwSetVarCallback set = SetCallback<StringTweakable, std::string>;
        TwGetVarCallback get = GetCallbackString;

        TwAddVarCB(bar, id.c_str(), TW_TYPE_STDSTRING, set, get, this, "");
        label(name);
      }
  };

  struct svec3 {
    float x;
    float y;
    float z;
  };

  TwType CreateVec3Struct() {
    TwStructMember vec3members[] = {
      { "x", TW_TYPE_FLOAT, offsetof(svec3, x), "" },
      { "y", TW_TYPE_FLOAT, offsetof(svec3, y), "" },
      { "z", TW_TYPE_FLOAT, offsetof(svec3, z), "" }
    };
    return TwDefineStruct("vec3", vec3members, 3, sizeof(svec3), NULL, NULL);
  }

  void TW_CALL SetCallbackVec3(const void* value, void* clientData);
  void TW_CALL GetCallbackVec3(void* value, void* clientData);

  TwType Vec3Struct() {
    static const TwType s = CreateVec3Struct();
    return s;
  }

  class Vec3Tweakable : public Tweakable {
    public:
      vec3 data;

      Vec3Tweakable(TwBar* bar, const std::string& id,
                    const std::string& name)
        : Tweakable(bar, id, name) {
        assert(this);

        TwSetVarCallback set = SetCallbackVec3;
        TwGetVarCallback get = GetCallbackVec3;

        TwAddVarCB(bar, id.c_str(), Vec3Struct(), set, get, this, "");
        label(name);
      }
  };

  void TW_CALL SetCallbackVec3(const void* value, void* clientData) {
    Vec3Tweakable* tweak = static_cast<Vec3Tweakable*>(clientData);
    const svec3 vec = *static_cast<const svec3*>(value);
    tweak->data[0] = vec.x;
    tweak->data[1] = vec.y;
    tweak->data[2] = vec.z;
    tweak->hasChanged = true;
  }

  void TW_CALL GetCallbackVec3(void* value, void* clientData) {
    Vec3Tweakable* tweak = static_cast<Vec3Tweakable*>(clientData);
    svec3* vec = static_cast<svec3*>(value);
    vec->x = tweak->data[0];
    vec->y = tweak->data[1];
    vec->z = tweak->data[2];
  }

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
                       const std::string& id, const std::string& name,
                       TData* data) {
    auto found = tweakables->find(id);
    Tweakable* tweakable = 0;
    if (found != tweakables->end()) {
      tweakable = found->second.get();
    } else {
      boost::shared_ptr<Tweakable> newtweakable(new
          TSpecifiedTweakable(bar, id, name));
      tweakables->insert(TweakerStore::Tweakables::
                         value_type(id, newtweakable));
      tweakable = newtweakable.get();
    }

    assert(tweakable);

    TSpecifiedTweakable* spec = reinterpret_cast<TSpecifiedTweakable*>(
                                  tweakable);

    if (spec->hasChanged) {
      *data = spec->data;
    } else {
      spec->data = *data;
    }
    spec->life = 0;

    return *spec;
  }
}  // namespace

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               std::string* data) {
  assert(this);
  return Tweakbase<StringTweakable, std::string>(&tweakables, bar, id, name,
         data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               int32* data) {
  assert(this);
  return Tweakbase < IntTweakable<TweakerStore::int32, TW_TYPE_INT32>,
         TweakerStore::int32 > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               uint32* data) {
  assert(this);
  return Tweakbase < IntTweakable<TweakerStore::uint32, TW_TYPE_UINT32>,
         TweakerStore::uint32 > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               bool* data) {
  assert(this);
  return Tweakbase < IntTweakable<bool, TW_TYPE_BOOLCPP>,
         bool > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               float* data) {
  assert(this);
  return Tweakbase < IntTweakable<float, TW_TYPE_FLOAT>,
         float > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               double* data) {
  assert(this);
  return Tweakbase < IntTweakable<double, TW_TYPE_DOUBLE>,
         double > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               quat* data) {
  assert(this);
  return Tweakbase < QuatTweakable, quat > (&tweakables, bar, id, name, data);
}

Tweakable& TweakerStore::tweak(const std::string& id, const std::string& name,
                               vec3* data) {
  assert(this);
  return Tweakbase < IntTweakable<vec3, TW_TYPE_DOUBLE>,
         vec3 > (&tweakables, bar, id, name, data);
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
