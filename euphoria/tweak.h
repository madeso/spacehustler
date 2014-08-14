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

/** Forward declaration of internal type.
 */
typedef struct CTwBar TwBar;

namespace euphoria {

namespace tweaks {

/** basic unit of of something that is tweakable.
 */
class Tweakable {
 public:
  /** The constructor.
   */
  Tweakable(TwBar* bar, const std::string& id, const std::string& name);

  /** true if it has changed trough tweaking, false if not.
   */
  bool hasChanged;

  /** the amount of life left.
   */
  int life;

  /** Virtual destructor.
   */
  virtual ~Tweakable();

  /** Changes the label of the tweakable.
  @param value the new label
  @returns this object for chaining
   */
  Tweakable& label(const std::string& value);

  /** Changes the group of the tweakable.
  @param value the new group name
  @returns this object for chaining
   */
  Tweakable& group(const std::string& value);

  /** Changes the readonly status of the tweakable.
  @param readonly the readonly status
  @returns this object for chaining
   */
  Tweakable& readonly(bool readonly);

  /** macro helping function.
  @see TWEAK
  @returns true.
   */
  operator bool() const;

 protected:
  /** The tweak bar.
   */
  TwBar* bar;

  /** The id of the tweak.
   */
  std::string id;
};

/** Adds basic members to a Tweakable class.
@param CLASS the class name.
@param TYPE the containing type.
 */
#define TWEAKABLE_BASIC(CLASS, TYPE)                                 \
 public: /*NOLINT macro spaacing issue*/                             \
  CLASS(TwBar* bar, const std::string& id, const std::string& name); \
  ~CLASS();                                                          \
                                                                     \
 public: /*NOLINT macro spaacing issue*/                             \
  TYPE data

/** Adds numeric related members to a Tweakable class.
@param CLASS the class name.
@param TYPE the containing type.
 */
#define TWEAKABLE_NUM(CLASS, TYPE)       \
 public: /*NOLINT macro spaacing issue*/ \
  CLASS& minmax(TYPE min, TYPE max);     \
  CLASS& step(TYPE step)

/** Adds integer related members to a Tweakable class.
@param CLASS the class name.
@param TYPE the containing type.
 */
#define TWEAKABLE_INT(CLASS, TYPE)       \
 public: /*NOLINT macro spaacing issue*/ \
  CLASS& hexa(bool hex)

/** Adds float related members to a Tweakable class.
@param CLASS the class name.
@param TYPE the containing type.
 */
#define TWEAKABLE_FLOAT(CLASS, TYPE)     \
 public: /*NOLINT macro spaacing issue*/ \
  CLASS& precision(fuint8 p)

/** Tweakable for int32.
 */
class Int32Tweakable : public Tweakable {
 public:
  /** Basic tweakable operations.
   */
  TWEAKABLE_BASIC(Int32Tweakable, int32);

  /** Numeric tweakable operations.
   */
  TWEAKABLE_NUM(Int32Tweakable, int32);

  /** Int tweakable operations.
   */
  TWEAKABLE_INT(Int32Tweakable, int32);
};

/** Tweakable for uint32.
 */
class Uint32Tweakable : public Tweakable {
 public:
  /** Basic tweakable operations.
   */
  TWEAKABLE_BASIC(Uint32Tweakable, uint32);

  /** Numeric tweakable operations.
   */
  TWEAKABLE_NUM(Uint32Tweakable, uint32);

  /** Int tweakable operations.
   */
  TWEAKABLE_INT(Uint32Tweakable, uint32);
};

/** Tweakable for float.
 */
class FloatTweakable : public Tweakable {
 public:
  /** Basic tweakable operations.
   */
  TWEAKABLE_BASIC(FloatTweakable, float);

  /** Numeric tweakable operations.
   */
  TWEAKABLE_NUM(FloatTweakable, float);

  /** Float tweakable operations.
   */
  TWEAKABLE_FLOAT(FloatTweakable, float);
};

/** Tweakable for double.
 */
class DoubleTweakable : public Tweakable {
 public:
  /** Basic tweakable operations.
   */
  TWEAKABLE_BASIC(DoubleTweakable, double);

  /** Numeric tweakable operations.
   */
  TWEAKABLE_NUM(DoubleTweakable, double);

  /** Float tweakable operations.
   */
  TWEAKABLE_FLOAT(DoubleTweakable, double);
};

/** Tweakable for vec3.
 */
class Vec3Tweakable : public Tweakable {
 public:
  /** Constructor.
  @param bar the bar.
  @param id the id.
  @param name the name
   */
  Vec3Tweakable(TwBar* bar, const std::string& id, const std::string& name);

  /** changes is the vec3 is a direction or not.
  @param b true if it is a direction, false if not.
  @returns this for chaining.
   */
  Vec3Tweakable& isDirection(bool b);

  /** The data.
   */
  Vec3 data;

 private:
  bool isdirection;
  std::string name;
};
}  // namespace tweaks

/** A place to store and do basic operations of tweakables.
 */
class TweakerStore {
 public:
  /** Constructor.
   */
  TweakerStore();

  /** Destructor.
   */
  ~TweakerStore();

  /** Tweak this string.
  @param id the unique id of the tweakable.
  @param name the displayable name of the tweakable.
  @param data the tweak data.
  @returns this object for easily chaining.
   */
  tweaks::Tweakable& tweak(const std::string& id, const std::string& name,
                           std::string* data);

  /** Tweak a 32 bit integer
   */
  tweaks::Int32Tweakable& tweak(const std::string& id, const std::string& name,
                                int32* data);

  /** Tweak a unsigned 32 bit integer
   */
  tweaks::Uint32Tweakable& tweak(const std::string& id, const std::string& name,
                                 uint32* data);

  /** Tweak a bool.
   */
  tweaks::Tweakable& tweak(const std::string& id, const std::string& name,
                           bool* data);

  /** Tweak a float.
   */
  tweaks::FloatTweakable& tweak(const std::string& id, const std::string& name,
                                float* data);

  /** Tweak a double.
   */
  tweaks::DoubleTweakable& tweak(const std::string& id, const std::string& name,
                                 double* data);

  /** Tweak a quaternion.
   */
  tweaks::Tweakable& tweak(const std::string& id, const std::string& name,
                           Quat* data);

  /** Tweak a vec3.
   */
  tweaks::Vec3Tweakable& tweak(const std::string& id, const std::string& name,
                               Vec3* data);

  /** update the store.
  Remove tweaks that are no longer needed etc..
   */
  void update();

  /** The List of tweakables type.
   */
  typedef std::map<std::string, std::shared_ptr<tweaks::Tweakable>> Tweakables;

 private:
  TwBar* bar;
  Tweakables tweakables;
};

/** Get a instance to the glocal tweaker store.
Since it is a glocal, you need to create it locally. That instance is then
accessed through this function.
@returns the global TweakerStore.
 */
TweakerStore* GlocalTweakerStore();

/** Utility macro.
Not really useful, except for the internal use in STR()
@see STR
@param name the name to quote.
 */
#define QUOTE(name) #name

/** Generate a string from a defined macro.
@param macro the macro.
 */
#define STR(macro) QUOTE(macro)

/** Add/update a variable for tweaking.
@see Tweakable
@param x the variable for tweaking.
@returns a suitable Tweakable.
 */
#define TWEAK(x)                \
  assert(GlocalTweakerStore()), \
      GlocalTweakerStore() &&   \
          GlocalTweakerStore()->tweak(__FILE__ STR(__LINE__), #x, &x)

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
