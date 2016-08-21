// Euphoria - Copyright (c) Gustav

/** @file
Contains all the Tweakable code.
 */

#ifndef EUPHORIA_TWEAK_H_
#define EUPHORIA_TWEAK_H_

#include <memory>

#include <map>
#include <string>
#include <cassert>

#include "euphoria/math.h"
#include "euphoria/ints.h"

#include "euphoria-config.h"  // NOLINT this is the default way to include cmake files

#ifdef USE_TWEAKABLES

namespace euphoria {

namespace tweaks {

/** basic unit of of something that is tweakable.
 */
class Tweakable {
 public:
  /** The constructor.
   */
  Tweakable(const std::string& label);
  virtual ~Tweakable();

  virtual void Run() = 0;

  const std::string& label() const;
  Tweakable& set_label(const std::string& label);

 private:
  /** The label.
   */
  std::string label_;
};

class TweakableString : public Tweakable {
 public:
  TweakableString(const std::string& label, std::string* str);
  ~TweakableString();

  void Run();
 private:
  std::string* str_;
};

class TweakableVec3 : public Tweakable {
 public:
  TweakableVec3(const std::string& label, Vec3* vec);
  ~TweakableVec3();

  void Run();

  TweakableVec3& readonly();
 private:
  Vec3* vec_;
  bool readonly_;
};

class TweakableBool : public Tweakable {
 public:
  TweakableBool(const std::string& label, bool* b);
  ~TweakableBool();

  void Run();
 private:
  bool* bool_;
};

}  // namespace tweaks

class TweakerStore {
 public:
  TweakerStore();
  ~TweakerStore();

  void Add(std::shared_ptr<tweaks::Tweakable> t);
  void RunAll();

 private:
  std::vector<std::shared_ptr<tweaks::Tweakable> > tweaks_;
};

void RunAllTweaks();
tweaks::TweakableString& Tweak(const std::string& name, std::string* data);
tweaks::TweakableVec3& Tweak(const std::string& name, Vec3* data);
tweaks::TweakableBool& Tweak(const std::string& name, bool* data);


/** Add/update a variable for tweaking.
@see Tweakable
@param x the variable for tweaking.
@returns a suitable Tweakable.
 */
#define TWEAK(x)                \
  ::euphoria::Tweak(#x, &x)

/** Util macro for running tweak code.
Runs the code only if tweaking is enabled.
@param x the code to be run
 */
#define RUNTWEAKCODE(x) x

}  // namespace euphoria

#else

/** Tweaking is disabled, doesn't do anything.
This function should probably return a mock object or something.
@param x a param that is ignored
 */
#define TWEAK(x)

/** Tweaking is disabled, doesn't do anything.
@param x a param that is ignored
 */
#define RUNTWEAKCODE(x)

#endif

#endif  // EUPHORIA_TWEAK_H_
