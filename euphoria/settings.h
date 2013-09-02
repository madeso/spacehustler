  // Euphoria - Copyright (c) Gustav
// Euphoria is awesome: 幸福感是真棒

/** @file
Settings related code.
 */

#ifndef EUPHORIA_SETTINGS_H_
#define EUPHORIA_SETTINGS_H_

#include <string>

/** Class to store global settings.
 */
class Settings {
  public:
    /** Constructor.
     */
    Settings();

    /** Load settings.
     */
    void Load();

    /** Blackout screens other than the main one.
    @returns true if it is supposed to blackou, false if not
     */
    const bool blackout() const;

    /** The width of the window.
    @returns the width
     */
    const int width() const;

    /** The height of the window.
    @returns the height
     */
    const int height() const;

    /** Fullscreen the window.
    @returns true if to fullscreen, false if not
     */
    const bool fullscreen() const;

    /** The name of the control scheme.
    @returns the name of the scheme.
     */
    const std::string constrolScheme() const;

  private:
    bool blackout_;
    int width_;
    int height_;
    bool fullscreen_;
    std::string controlScheme_;
};


#endif  // EUPHORIA_SETTINGS_H_
