// Euphoria - Copyright (c) Gustav

#include "euphoria/angle.h"

#include <cassert>
#include <cmath>

#include "euphoria/scalar.h"

namespace euphoria {

namespace {
const float rad2deg(const float rad) { return (180.0f / Pi()) * rad; }
const float deg2rad(const float deg) { return Pi() / 180.0f * deg; }
}  // namespace

const float Angle::InDegrees() const { return rad2deg(radians_); }

const float Angle::InRadians() const { return radians_; }

const Angle Angle::FromDegrees(float deg) { return Angle(deg2rad(deg)); }

const Angle Angle::FromRadians(float rad) { return Angle(rad); }

Angle::Angle(float rad) : radians_(rad) {}

void Angle::Wrap() { radians_ = ::euphoria::Wrap(0, radians_, Pi() * 2.0f); }

void Angle::operator+=(const Angle& rhs) { radians_ += rhs.radians_; }

void Angle::operator-=(const Angle& rhs) { radians_ -= rhs.radians_; }

void Angle::operator*=(const float rhs) { radians_ *= rhs; }

const Angle operator+(const Angle& lhs, const Angle& rhs) {
  Angle temp(lhs);
  temp += rhs;
  return temp;
}

const Angle operator-(const Angle& lhs, const Angle& rhs) {
  Angle temp(lhs);
  temp -= rhs;
  return temp;
}

const Angle operator*(const Angle& lhs, const float rhs) {
  Angle temp(lhs);
  temp *= rhs;
  return temp;
}

const Angle operator*(const float rhs, const Angle& lhs) { return lhs * rhs; }

namespace angle {

const float Sin(const Angle& ang) { return std::sin(ang.InRadians()); }

const float Cos(const Angle& ang) { return std::cos(ang.InRadians()); }

const float Tan(const Angle& ang) { return std::tan(ang.InRadians()); }

const Angle Asin(const float v) {
  assert(v <= 1 &&
         "v must be smaller than 1, use Limmit or Max on the value to not "
         "trigger this assert");
  assert(v >= -1 &&
         "v must be greater than -1, use Limmit or Min on the value to not "
         "trigger this assert");
  return Angle::FromRadians(std::asin(v));
}

const Angle Acos(const float v) {
  assert(v <= 1 &&
         "v must be smaller than 1, use Limmit or Max on the value to not "
         "trigger this assert");
  assert(v >= -1 &&
         "v must be greater than -1, use Limmit or Min on the value to not "
         "trigger this assert");
  return Angle::FromRadians(std::acos(v));
}

const Angle Atan(const float v) { return Angle::FromRadians(std::atan(v)); }

const Angle GetWrapped(const Angle& a) {
  Angle temp(a);
  temp.Wrap();
  return temp;
}

const Angle FromPercentOf360(const float percent) {
  return Angle::FromRadians(percent * Pi() * 2.0f);
}

const Angle Zero() { return Angle::FromRadians(0); }

}  // namespace angle
}  // namespace euphoria
