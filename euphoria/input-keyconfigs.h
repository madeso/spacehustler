// Euphoria - Copyright (c) Gustav

/** @file
Classes for input handling.
 */

#ifndef EUPHORIA_INPUT_KEYCONFIGS_H_
#define EUPHORIA_INPUT_KEYCONFIGS_H_

#include <map>
#include <string>
#include <memory>

namespace input {
  class KeyConfig;
  class ConnectedUnits;
  class InputActionMap;

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

}  // namespace input

#endif  // EUPHORIA_INPUT_KEYCONFIGS_H_
