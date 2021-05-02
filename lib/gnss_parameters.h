/* -*- c++ -*- */
/*
 * Copyright 2021 Lukasz Wiecaszek <lukasz.wiecaszek@gmail.com>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_GNSS_GNSS_PARAMETERS_H
#define INCLUDED_GNSS_GNSS_PARAMETERS_H

#include <cstdint>

namespace gr {
  namespace gnss {

  constexpr uint32_t GPS_F0 = 10230000;
  constexpr uint32_t GPS_L1_MULTIPLIER = 154;
  constexpr uint32_t GPS_L2_MULTIPLIER = 120;

  // IS-GPS-200 - Chapter 3.3.1.1 Frequency Plan
  // The nominal carrier frequencies shall be 1575.42 MHz,
  // and 1227.6 MHz for L1 and L2, respectively.
  constexpr uint32_t GPS_L1_FREQ_HZ = GPS_L1_MULTIPLIER * GPS_F0;
  constexpr uint32_t GPS_L2_FREQ_HZ = GPS_L2_MULTIPLIER * GPS_F0;

  constexpr uint32_t GPS_CA_CODE_LENGTH = 1023;
  constexpr uint32_t GPS_CA_CODE_RATE = 1000; // codes per second
  constexpr uint32_t GPS_CA_CODE_CHIP_RATE = GPS_CA_CODE_LENGTH * GPS_CA_CODE_RATE; // chips per second

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_GNSS_PARAMETERS_H */
