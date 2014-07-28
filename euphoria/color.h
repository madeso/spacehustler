// Euphoria - Copyright (c) Gustav

/** @file
Basic class for handling colors.
 */

#ifndef EUPHORIA_COLOR_H_
#define EUPHORIA_COLOR_H_

/** A color.
 */
struct Color {
 public:
  /** Construct a gray color.
  @param gray the intensity of gray ranging from 0 to 1.
   */
  explicit Color(float gray);

  /** Construct a new RGB.
  @param R the red, ranging from 0 to 1.
  @param G the green, ranging from 0 to 1.
  @param B the blue, ranging from 0 to 1.
   */
  Color(float R, float G, float B);

  /** Construct a new RGBA.
  @param R the red, ranging from 0 to 1.
  @param G the green, ranging from 0 to 1.
  @param B the blue, ranging from 0 to 1.
  @param A the alpha, ranging from 0 to 1 where 0 ins transparent and 1 is
  solid.
   */
  Color(float R, float G, float B, float A);

  /** Construct a new RGBA.
  @param c the color to grab the RGB from.
  @param A the alpha, ranging from 0 to 1 where 0 ins transparent and 1 is
  solid.
   */
  Color(const Color& c, float A);

  /** Named constructor that converts a hex value into a Color.
  @param hex the hex value.
  @returns the color
   */
  static Color FromHex(unsigned int hex);

  /** The Red component., ranging from 0 to 1.
   */
  float r;

  /** The Green component., ranging from 0 to 1.
   */
  float g;

  /** The Blue component., ranging from 0 to 1.
   */
  float b;

  /** The Alpha component., ranging from 0 to 1 where 0 ins transparent and 1 is
   * solid.
   */
  float a;
};

#endif  // EUPHORIA_COLOR_H_
