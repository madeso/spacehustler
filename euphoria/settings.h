// Euphoria - Copyright (c) Gustav

/** @file
Settings related code.
 */

#ifndef EUPHORIA_SETTINGS_H_
#define EUPHORIA_SETTINGS_H_

#include <string>

/** Determines how to detect a oculus vr device.
 */
enum class OculusVrDetection {
  AUTO  /// Automatic detection.
  ,
  NORMAL  /// Force a normal display with no vr input
  ,
  OCULUS_VR  /// Force a oculus vr display
};

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
  @returns true if it is supposed to blackout, false if not
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
  const std::string control_scheme() const;

  /** The window to launch at.
  @returns the window id to launch at.
   */
  int primary_display_id() const;

  /** How to detect the oculus vr kit.
  @returns how to detect the oculus.
   */
  OculusVrDetection oculus_vr_detection() const;

  /** Gets the anisotropic settings.
  http://en.wikipedia.org/wiki/Anistropic_filtering
  @returns the anisotropic settings
   */
  float anisotropic() const;

  /** Gets if the game should support joysticks or not
  @returns if the game should support joysticks or not
   */
  bool support_joystick() const;

  bool vsync_enabled() const;
  bool is_low_persistence() const;
  bool dynamic_prediction() const;
  bool display_sleep() const;
  bool mirror_to_window() const;
  bool pixel_luminance_overdrive() const;
  bool timewarp_enabled() const;
  bool timewarp_no_jit_enabled() const;
  bool multisample() const;
  bool position_tracking_enabled() const;
  bool mipmap_eye_textures() const;
  bool force_zero_ipd() const;
  bool rendertarget_is_shared_by_both_eyes() const;
  float desired_pixel_density() const;

 private:
  bool blackout_;
  int width_;
  int height_;
  bool fullscreen_;
  std::string control_scheme_;
  int primary_display_id_;
  OculusVrDetection oculus_vr_detection_;
  float anisotropic_;
  bool support_joystick_;

  bool vsync_enabled_;
  bool is_low_persistence_;
  bool dynamic_prediction_;
  bool display_sleep_;
  bool mirror_to_window_;
  bool pixel_luminance_overdrive_;
  bool timewarp_enabled_;
  bool timewarp_no_jit_enabled_;
  bool multisample_;
  bool position_tracking_enabled_;
  bool mipmap_eye_textures_;
  bool force_zero_ipd_;
  bool rendertarget_is_shared_by_both_eyes_;
  float desired_pixel_density_;
};

#endif  // EUPHORIA_SETTINGS_H_
