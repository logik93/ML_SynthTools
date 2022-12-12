/*
 * Copyright (c) 2022 Marcel Licence
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Dieses Programm ist Freie Software: Sie können es unter den Bedingungen
 * der GNU General Public License, wie von der Free Software Foundation,
 * Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
 * veröffentlichten Version, weiter verteilen und/oder modifizieren.
 *
 * Dieses Programm wird in der Hoffnung bereitgestellt, dass es nützlich sein wird, jedoch
 * OHNE JEDE GEWÄHR,; sogar ohne die implizite
 * Gewähr der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
 * Siehe die GNU General Public License für weitere Einzelheiten.
 *
 * Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
 * Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
 */

/**
 * @file ml_tremolo.cpp
 * @author Marcel Licence
 * @date 08.12.2022
 *
 * @brief   Tremolo stereo implementation
 *
 * @see little demo: https://youtu.be/zu2xtRKlNVU
 */


#include <ml_tremolo.h>

#include <stdio.h>


ML_Tremolo::ML_Tremolo(float sample_rate)
{
    this->sample_rate = sample_rate;

    value = 0;
    valueU32 = 0;

    setSpeed(6.5f);
    setPhaseShift(0.5f);

    depth = 0;
    depthInv = 1;

    float var = speed;
    var /= M_PI;
    var *= 4294967296;
    speedU32 = var;
}

void ML_Tremolo::process(float *left, float *right, int len)
{
    for (int n = 0; n < len; n++)
    {
        left[n] *= depthInv + depth * sinf(value);
        right[n] *= depthInv + depth * sinf(phase_shift + value);
        value += speed;
        valueU32 += speedU32;

        /* avoid the runaway */
        if (value >= 2 * M_PI)
        {
            value -= 2 * M_PI;
        }
    }
}

void ML_Tremolo::setSpeed(float speed)
{
    speed *= 2 * M_PI;
    speed /= sample_rate;
    this->speed = speed;
    speedU32 = (speed / (M_PI)) * (1 << 31);
}

void ML_Tremolo::setPhaseShift(float shift)
{
    phase_shift = shift * 2 * M_PI;
}

void ML_Tremolo::setDepth(float new_depth)
{
    this->depth = new_depth;
}
