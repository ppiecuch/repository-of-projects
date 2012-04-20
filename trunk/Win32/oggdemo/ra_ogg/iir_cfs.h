/*
 *   PCM time-domain equalizer
 *
 *   Copyright (C) 2002-2006  Felipe Rivera <liebremx at users.sourceforge.net>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 *   $Id: iir_cfs.h,v 1.2 2006/01/15 00:17:46 liebremx Exp $
 */
#ifndef IIR_CFS_H
#define IIR_CFS_H
#include <math.h>
#define M_SQRT2 1.41421356237309504880 
#define M_PI    3.1415926
/* Coefficients entry */
typedef struct 
{
    float beta;
    float alpha; 
    float gamma;
    float dummy; // Word alignment
}sIIRCoefficients;

sIIRCoefficients* get_coeffs(int *bands, int sfreq, bool use_xmms_original_freqs);
void calc_coeffs();

#endif
