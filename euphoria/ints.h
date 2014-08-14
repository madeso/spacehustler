// Euphoria - Copyright (c) Gustav

/** @file
Definitions for various integers.
 */

#ifndef EUPHORIA_INTS_H_
#define EUPHORIA_INTS_H_

#include <boost/cstdint.hpp>

namespace euphoria {

/** @defgroup int Integer typedefs
@{
 */

///////////////////////////////////////////////////////////////////////////////

/** Signed integer, exactly 64 bytes.
 */
typedef boost::int64_t int64;

/** Unsigned integer, exactly 32 bytes.
 */
typedef boost::uint64_t uint64;

/** Signed integer, exactly 32 bytes.
 */
typedef boost::int32_t int32;

/** Unsigned integer, exactly 32 bytes.
 */
typedef boost::uint32_t uint32;

/** Signed integer, exactly 16 bytes.
 */
typedef boost::int16_t int16;

/** Unsigned integer, exactly 16 bytes.
 */
typedef boost::uint16_t uint16;

/** Signed integer, exactly 8 bytes.
 */
typedef boost::int8_t int8;

/** Unsigned integer, exactly 8 bytes.
 */
typedef boost::uint8_t uint8;

///////////////////////////////////////////////////////////////////////////////

/** Signed integer, smallest data type with at least 64 bytes
 */
typedef boost::int_least64_t lint64;

/** Unsigned integer, smallest data type with at least 64 bytes
 */
typedef boost::uint_least64_t luint64;

/** Signed integer, smallest data type with at least 32 bytes
 */
typedef boost::int_least32_t lint32;

/** Unsigned integer, smallest data type with at least 32 bytes
 */
typedef boost::uint_least32_t luint32;

/** Signed integer, smallest data type with at least 16 bytes
 */
typedef boost::int_least16_t lint16;

/** Unsigned integer, smallest data type with at least 16 bytes
 */
typedef boost::uint_least16_t luint16;

/** Signed integer, smallest data type with at least 8 bytes
 */
typedef boost::int_least8_t lint8;

/** Unsigned integer, smallest data type with at least 8 bytes
 */
typedef boost::uint_least8_t luint8;

///////////////////////////////////////////////////////////////////////////////

/** Signed integer, smallest data type with at least 64 bytes and best
 * arithmetic performance.
 */
typedef boost::int_fast64_t fint64;

/** Unsigned integer, smallest data type with at least 64 bytes and best
 * arithmetic performance.
 */
typedef boost::uint_fast64_t fuint64;

/** Signed integer, smallest data type with at least 32 bytes and best
 * arithmetic performance.
 */
typedef boost::int_fast32_t fint32;

/** Unsigned integer, smallest data type with at least 32 bytes and best
 * arithmetic performance.
 */
typedef boost::uint_fast32_t fuint32;

/** Signed integer, smallest data type with at least 16 bytes and best
 * arithmetic performance.
 */
typedef boost::int_fast16_t fint16;

/** Unsigned integer, smallest data type with at least 16 bytes and best
 * arithmetic performance.
 */
typedef boost::uint_fast16_t fuint16;

/** Signed integer, smallest data type with at least 8 bytes and best arithmetic
 * performance.
 */
typedef boost::int_fast8_t fint8;

/** Unsigned integer, smallest data type with at least 8 bytes and best
 * arithmetic performance.
 */
typedef boost::uint_fast8_t fuint8;

/** @} */

}  // namespace euphoria

#endif  // EUPHORIA_INTS_H_
