  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUTSYSTEM_H_
#define EUPHORIA_INPUTSYSTEM_H_

#include <vector>
#include <map>
#include <string>
#include <memory>

#include "euphoria/key.h"
#include "euphoria/axis.h"

/** A input action.
 */
class InputAction {
  public:
    /** Constructor.
    @param scriptvarname the name of the variable in the script.
     */
    explicit InputAction(const std::string& scriptvarname);

    /** Get the name of the script var.
     */
    const std::string& scriptvarname() const;

    /** Get the current value of the input.
    @returns the state
     */
    float state() const;

    /** Set the value of the input.
    @param state the new value
     */
    void set_state(float state);

  private:
    std::string scriptvarname_;
    float state_;
};

/** A map of all the available input actions.
 */
class InputActionMap {
  public:
    /** Constructor.
     */
    InputActionMap();

    /** Add a action.
    @param name the name of the action
    @param action the action itself
     */
    void Add(const std::string& name, std::shared_ptr<InputAction> action);

    /** Get a certain action.
    @param name the name of the action
    @returns the input action
     */
    std::shared_ptr<InputAction> Get(const std::string& name) const;

  private:
    std::map<std::string, std::shared_ptr<InputAction> > actions_;
};

/** Load actions from file.
@param map the action map to fill
@param filename the filename to load from
 */
void Load(InputActionMap* map, const std::string& filename);

class KeyConfig;
class ConnectedUnits;
/** Contains a list of configurations.
 */
class KeyConfigs {
  public:
    /** Constructor.
     */
    KeyConfigs();

    /** Add a configuration.
    @param name the name of the configuration
    @param config the configuration to add
     */
    void Add(const std::string& name, std::shared_ptr<KeyConfig> config);

    /** Get a configuration.
    @param name the name of the configuration
    @returns the found configuration
     */
    std::shared_ptr<KeyConfig> Get(const std::string& name) const;

    /** Start configuration auto detection.
     */
    void BeginAutoDetect();

    /** Abort auto detection.
     */
    void AbortAutoDetect();

    /** Get the detected configuration.
    If there are many detected, this will only return the first.
    @returns the first detected configuration
     */
    std::shared_ptr<ConnectedUnits> GetFirstAutoDetectedConfig() const;

  private:
    std::map<std::string, std::shared_ptr<KeyConfig>> configs_;
};

/** Load configurations.
@param configs the configurations
@param filename the filename
@param map the actions
 */
void Load(KeyConfigs* configs, const std::string& filename,
          const InputActionMap& map);

class KeyboardActiveUnit;

/** Container class for directing input.
 */
class InputDirector {
  public:
    /** Add a keyboard.
    @param kb the keyboard
     */
    void Add(KeyboardActiveUnit* kb);

    /** Remove a keyboard.
    @param kb the keyboard
     */
    void Remove(KeyboardActiveUnit* kb);

    /** Send a keyboard event.
    @param key the key to act upon
    @param down true if it is down, false if not
     */
    void OnKeyboardKey(Key::Type key, bool down);

    /** Send a mouse axis.
    @param axis the axis
    @param value the value
     */
    void OnMouseAxis(Axis::Type axis, float value);

    /** Send a mouse button.
    @param button the button to act upon
    @param down true if it is down, false if not
     */
    void OnMouseButton(MouseButton::Type button, bool down);

    /** Send a joystick pov.
    @param type the pov type
    @param joystick the joystick id
    @param value the value
     */
    void OnJoystickPov(JoystickPov::Type type, int joystick, float value);

    /** Send a joystick button.
    @param button the button to act upon
    @param joystick the joystick id
    @param down true if it is down, false if not
     */
    void OnJoystickButton(int button, int joystick, bool down);

    /** Send a joystick axis.
    @param axis the axis
    @param joystick the joystick id
    @param value the value
     */
    void OnJoystickAxis(int axis, int joystick, float value);

  private:
    std::vector<KeyboardActiveUnit*> keyboards_;
};

class Player;
/** the master class that controls the input system.
 */
class InputSystem {
  public:
    /** Constructor.
     */
    InputSystem();

    /** Get a action.
    @param name the name of the action
    @returns the action
     */
    std::shared_ptr<InputAction> GetAction(const std::string& name);

    /** Setups a input unit for a player.
    @param playerName the name of the player
    @param inputName the name of the input
     */
    void SetUnitForPlayer(const std::string& playerName,
                          const std::string& inputName);

    /** Send a keyboard event.
    @param key the key to act upon
    @param down true if it is down, false if not
     */
    void OnKeyboardKey(Key::Type key, bool down);

    /** Send a mouse axis.
    @param axis the axis
    @param value the value
     */
    void OnMouseAxis(Axis::Type axis, float value);

    /** Send a mouse button.
    @param button the button to act upon
    @param down true if it is down, false if not
     */
    void OnMouseButton(MouseButton::Type button, bool down);

    /** Send a joystick pov.
    @param type the pov type
    @param joystick the joystick id
    @param value the value
     */
    void OnJoystickPov(JoystickPov::Type type, int joystick, float value);

    /** Send a joystick button.
    @param button the button to act upon
    @param joystick the joystick id
    @param down true if it is down, false if not
     */
    void OnJoystickButton(int button, int joystick, bool down);

    /** Send a joystick axis.
    @param axis the axis
    @param joystick the joystick id
    @param value the value
     */
    void OnJoystickAxis(int axis, int joystick, float value);

    /** Get a player.
    @param name the name of the player
    @returns the player
     */
    std::shared_ptr<Player> GetPlayer(const std::string& name);

    /** Add a player.
    @name the name of the player.
     */
    void AddPlayer(const std::string& name);

  private:
    InputActionMap actions_;
    std::map<std::string, std::shared_ptr<Player>> players_;
    KeyConfigs configs_;
    std::unique_ptr<InputDirector> input_;
};

class ActiveUnit;
class Table;

/** Contains a list of active units.
 */
class ConnectedUnits {
  public:
    /** Constructor.
     */
    ConnectedUnits();

    /** Updates all connected units.
    @param table the table to update
     */
    void UpdateTable(Table* table);

    /** Add a unit.
    @param unit the unit to add
     */
    void Add(std::shared_ptr<ActiveUnit> unit);

    /** Are there any connected units?
    @returns true if there are no active units, false if not
     */
    bool IsEmpty() const;
  private:
    std::vector<std::shared_ptr<ActiveUnit>> units_;
};

/** Represents a player.
The idea behind decoupling the active units and the player is that the unit
could be disconnected and swapped but the player should remain.
 */
class Player {
  public:
    /** Constructor.
     */
    Player();

    /** Updates all connected units.
    @param table the table to update
     */
    void UpdateTable(Table* table);

    /** Sets the connected units.
    @param units the units
     */
    void set_units(ConnectedUnits units);

  private:
    ConnectedUnits units_;
};

class Table;
/** Represents a active unit.
A perfect example is a connected joystick.
 */
class ActiveUnit {
  public:
    /** Update the table with the actions.
     */
    void UpdateTable(Table* table);

    /** Destructor.
     */
    virtual ~ActiveUnit();

    /** Causes the active unit to rumble.
     */
    virtual void Rumble() = 0;

  protected:
    /** Constructor.
     */
    ActiveUnit();

    /** Add a action to be updated.
     */
    void Add(std::shared_ptr<InputAction> action);

  private:
    std::vector<std::shared_ptr<InputAction>> actions_;
};

class UnitDef;

/** Contains a list of configurations.
A good example is Mouse+Keyboard.
 */
class KeyConfig {
  public:
    /** Constructor.
     */
    KeyConfig();

    /** Add a definition.
    @param def the definition to add.
     */
    void Add(std::shared_ptr<UnitDef> def);

    /** Connect.
    @param director the input director
    @returns the connected units
     */
    ConnectedUnits Connect(InputDirector* director) const;

  private:
    std::vector<std::shared_ptr<UnitDef>> definitions_;
};

/** Definition of a certain input unit.
Usually loaded from a file.
 */
class UnitDef {
  public:
    /** Destructor.
     */
    virtual ~UnitDef();

    /** Create a active unit.
    @param director the input director
    @returns the active unit.
     */
    virtual std::shared_ptr<ActiveUnit> Create(InputDirector* director) = 0;
};

#endif  // EUPHORIA_INPUTSYSTEM_H_
