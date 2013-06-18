// Euphoria - Copyright (c) Gustav

#ifndef EUPHORIA_INTS_H_
#define EUPHORIA_INTS_H_

#include <boost/cstdint.hpp>

// exactly x bits

typedef boost::int64_t int64;
typedef boost::uint64_t uint64;

typedef boost::int32_t int32;
typedef boost::uint32_t uint32;

typedef boost::int16_t int16;
typedef boost::uint16_t uint16;

typedef boost::int8_t int8;
typedef boost::uint8_t uint8;

// smallest data type with at least X bytes

typedef boost::int_least64_t lint64;
typedef boost::uint_least64_t luint64;

typedef boost::int_least32_t lint32;
typedef boost::uint_least32_t luint32;

typedef boost::int_least16_t lint16;
typedef boost::uint_least16_t luint16;

typedef boost::int_least8_t lint8;
typedef boost::uint_least8_t luint8;

// smallest data type with at least X bytes and best arithmetic performance

typedef boost::int_fast64_t fint64;
typedef boost::uint_fast64_t fuint64;

typedef boost::int_fast32_t fint32;
typedef boost::uint_fast32_t fuint32;

typedef boost::int_fast16_t fint16;
typedef boost::uint_fast16_t fuint16;

typedef boost::int_fast8_t fint8;
typedef boost::uint_fast8_t fuint8;

#endif  // EUPHORIA_INTS_H_
