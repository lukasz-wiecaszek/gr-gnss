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

#ifndef INCLUDED_GNSS_CA_CODE_DOMAIN_H
#define INCLUDED_GNSS_CA_CODE_DOMAIN_H

namespace gr {
  namespace gnss {

    typedef enum {
        CA_CODE_DOMAIN_TIME,
        CA_CODE_DOMAIN_FREQUENCY,
        CA_CODE_DOMAIN_FREQUENCY_CONJUGATE,
    } ca_code_domain_e;

  } // namespace gnss
} // namespace gr

#endif /* INCLUDED_GNSS_CA_CODE_DOMAIN_H */

