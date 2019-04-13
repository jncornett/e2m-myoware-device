#include "filter.h"

/* Filter generator:
    https://www-users.cs.york.ac.uk/~fisher/cgi-bin/mkfscript
  filtertype   =   Butterworth
  passtype  =   Lowpass
  ripple  =
  order   =   3
  samplerate  =   100
  corner1   =   1
  corner2   =
  adzero  =
  logmin  =

  Command line: /www/usr/fisher/helpers/mkfilter -Bu -Lp -o 3 -a 1.0000000000e-02 0.0000000000e+00
  raw alpha1    =   0.0100000000
  raw alpha2    =   0.0100000000
  warped alpha1 =   0.0100032912
  warped alpha2 =   0.0100032912
  gain at dc    :   mag = 3.430944333e+04   phase =  -0.0000000000 pi
  gain at centre:   mag = 2.426044003e+04   phase =  -0.7500000000 pi
  gain at hf    :   mag = 0.000000000e+00
*/

#define NZEROS 3
#define NPOLES 3
#define GAIN 3.430944333e+04

float filter(float value)
{
  static float xv[NZEROS + 1];
  static float yv[NPOLES + 1];
  xv[0] = xv[1];
  xv[1] = xv[2];
  xv[2] = xv[3];
  xv[3] = value / GAIN;
  yv[0] = yv[1];
  yv[1] = yv[2];
  yv[2] = yv[3];
  yv[3] = (xv[0] + xv[3]) + 3 * (xv[1] + xv[2]) + (0.8818931306 * yv[0]) + (-2.7564831952 * yv[1]) + (2.8743568927 * yv[2]);
  return yv[3];
}
