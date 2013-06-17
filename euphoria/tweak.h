// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_TWEAK_H_
#define EUPHORIA_TWEAK_H_

#include <boost/shared_ptr.hpp>
#include <boost/cstdint.hpp>

#include <map>
#include <string>
#include <cassert>

#include "euphoria/math.h"

// forward declaration of internal type
typedef struct CTwBar TwBar;

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

    /** A 32 bit integer.
     */
    typedef boost::int32_t int32;

    /** A 32 bit unsigned integer.
     */
    typedef boost::uint32_t uint32;

    /** Tweak this string.
    @param id the unique id of the tweakable.
    @param name the displayable name of the tweakable.
    @param data the tweak data.
    @returns this object for easily chaining.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     std::string* data);

    /** Tweak a 32 bit integer
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     int32* data);

    /** Tweak a unsigned 32 bit integer
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     uint32* data);

    /** Tweak a bool.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     bool* data);

    /** Tweak a float.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     float* data);

    /** Tweak a double.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     double* data);

    /** Tweak a quaternion.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     quat* data);

    /** Tweak a vec3.
     */
    Tweakable& tweak(const std::string& id, const std::string& name,
                     vec3* data);

    /** update the store.
    Remove tweaks that are no longer needed etc..
     */
    void update();

    /** The List of tweakables type.
     */
    typedef std::map<std::string, boost::shared_ptr<Tweakable>> Tweakables;

  private:
    TwBar* bar;
    Tweakables tweakables;
};

TweakerStore* GlocalTweakerStore();

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)

#define TWEAK(x) assert(GlocalTweakerStore()), GlocalTweakerStore() && \
  GlocalTweakerStore()->tweak(__FILE__  STR(__LINE__), #x, &x)

#endif  // EUPHORIA_TWEAK_H_
