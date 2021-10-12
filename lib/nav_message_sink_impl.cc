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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "nav_message_sink_impl.h"
#include "gps_nav_message_subframe.h"
#include "verlet_integration.h"

#include <string>
#include <sstream>

namespace gr {
  namespace gnss {

    nav_message_sink::sptr
    nav_message_sink::make(const char* filename)
    {
      return gnuradio::get_initial_sptr
        (new nav_message_sink_impl<uint8_t>(filename));
    }

    template<typename ITYPE0>
    nav_message_sink_impl<ITYPE0>::nav_message_sink_impl(const char* filename)
      : gr::sync_block("nav_message_sink",
                       gr::io_signature::make(1, 1, sizeof(ITYPE0) * IVLEN0),
                       gr::io_signature::make(0, 0, 0)),
        d_fp{NULL},
        d_ephemeris{}
    {
      d_fp = fopen(filename, "w");
      if (d_fp == NULL) {
        std::stringstream s;
        s << "cannot open fifo file '" << filename << "': '" << strerror(errno) << "'" << std::endl;
        throw std::runtime_error(s.str());
      }
    }

    template<typename ITYPE0>
    nav_message_sink_impl<ITYPE0>::~nav_message_sink_impl()
    {
      if (d_fp != NULL)
        fclose(d_fp);
    }

    template<typename ITYPE0>
    int
    nav_message_sink_impl<ITYPE0>::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const ITYPE0* iptr0 = (const ITYPE0*) input_items[0];
      int nproduced = 0;
      gps_nav_message_subframe subframe;

      while (nproduced < noutput_items) {
        subframe.init(&iptr0);
        std::string str = subframe.to_string();

        fprintf(stdout, "%s\n", str.c_str());
        fprintf(d_fp, "%s\n", str.c_str());

        if (!d_ephemeris.is_valid)
        {
          if (subframe.subframe_id() == 2)
          {
            gps_nav_message_subframe_2 subframe_2 = subframe;
            int IODE = d_ephemeris.IODE[0];

            if ((IODE == -1) || (IODE != subframe_2.IODE()))
            {
              d_ephemeris.IODE[0]               = subframe_2.IODE();
              d_ephemeris.correction_terms.C_RS = subframe_2.C_RS()       * GPS_SCALE_FACTOR_C_R;
              d_ephemeris.delta_n               = subframe_2.DELTA_N()    * GPS_SCALE_FACTOR_DELTA_N * M_PI;
              d_ephemeris.M_0                   = subframe_2.M_0()        * GPS_SCALE_FACTOR_M_0 * M_PI;
              d_ephemeris.correction_terms.C_UC = subframe_2.C_UC()       * GPS_SCALE_FACTOR_C_U;
              d_ephemeris.e                     = subframe_2.e()          * GPS_SCALE_FACTOR_E;
              d_ephemeris.correction_terms.C_US = subframe_2.C_US()       * GPS_SCALE_FACTOR_C_U;
              d_ephemeris.sqrt_a                = subframe_2.SQRT_A()     * GPS_SCALE_FACTOR_SQRT_A;
              d_ephemeris.t_oe                  = subframe_2.t_oe()       * GPS_SCALE_FACTOR_T_OE;
            }
          }

          if (subframe.subframe_id() == 3)
          {
            gps_nav_message_subframe_3 subframe_3 = subframe;
            int IODE = d_ephemeris.IODE[1];

            if ((IODE == -1) || (IODE != subframe_3.IODE()))
            {
              d_ephemeris.IODE[1]               = subframe_3.IODE();
              d_ephemeris.correction_terms.C_IC = subframe_3.C_IC()       * GPS_SCALE_FACTOR_C_I;
              d_ephemeris.OMEGA_0               = subframe_3.OMEGA_0()    * GPS_SCALE_FACTOR_OMEGA_0 * M_PI;
              d_ephemeris.correction_terms.C_IS = subframe_3.C_IS()       * GPS_SCALE_FACTOR_C_I;
              d_ephemeris.i_0                   = subframe_3.i_0()        * GPS_SCALE_FACTOR_I_0 * M_PI;
              d_ephemeris.correction_terms.C_RC = subframe_3.C_RC()       * GPS_SCALE_FACTOR_C_R;
              d_ephemeris.omega                 = subframe_3.omega()      * GPS_SCALE_FACTOR_OMEGA * M_PI;
              d_ephemeris.dOMEGA_dt             = subframe_3.dOMEGA_dt()  * GPS_SCALE_FACTOR_D_OMEGA_DT * M_PI;
              d_ephemeris.dI_dt                 = subframe_3.dI_dt()      * GPS_SCALE_FACTOR_IDOT * M_PI;
            }
          }

          if ((d_ephemeris.IODE[0] != -1) && (d_ephemeris.IODE[0] == d_ephemeris.IODE[1]))
          {
            d_ephemeris.is_valid = true;
            fprintf(stdout, "%s\n", d_ephemeris.to_string().c_str());
            fprintf(d_fp, "%s\n", d_ephemeris.to_string().c_str());

            double t = subframe.tow_count_message() * 6;
            double dt = 1.0; // 1 second
            vector position;
            vector velocity;
            vector acceleration;

            d_ephemeris.get_vectors(t, &position, &velocity, &acceleration);
            fprintf(stdout, "r: %s [%e], v: %s [%e], a: %s [%e]\n",
              position.to_string().c_str(), abs(position),
              velocity.to_string().c_str(), abs(velocity),
              acceleration.to_string().c_str(), abs(acceleration));

            vector r = position;
            vector v = velocity;
            vector a = acceleration;

            const std::size_t niterations = 60 * 60;
            for (std::size_t i = 0; i < niterations; i++) {
              t += dt;
              d_ephemeris.get_vectors(t, &position, &velocity, &acceleration);
              fprintf(d_fp, "r: %s [%e], v: %s [%e], a: %s [%e]\n",
                position.to_string().c_str(), abs(position),
                velocity.to_string().c_str(), abs(velocity),
                acceleration.to_string().c_str(), abs(acceleration));
            }

            fprintf(d_fp, "\n");

            for (std::size_t i = 0; i < niterations; i++) {
              verlet_integration::get_vectors(dt, r, v, a);
              fprintf(d_fp, "r: %s [%e], v: %s [%e], a: %s [%e]\n",
                r.to_string().c_str(), abs(r),
                v.to_string().c_str(), abs(v),
                a.to_string().c_str(), abs(a));
            }
          }
        }

        nproduced++;
      }

      // Tell runtime system how many output items we produced.
      return nproduced;
    }

  } /* namespace gnss */
} /* namespace gr */

