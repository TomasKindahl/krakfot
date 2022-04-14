/*******************************************************************************
 *  krakfot                                                                    *
 *                                                                             *
 *     Copyright 2022 Tomas Kindahl                                            *
 *                                                                             *
 *  Licensed under the Apache License, Version 2.0 (the "License");            *
 *  you may not use this file except in compliance with the License.           *
 *  You may obtain a copy of the License at                                    *
 *                                                                             *
 *      http://www.apache.org/licenses/LICENSE-2.0                             *
 *                                                                             *
 *  Unless required by applicable law or agreed to in writing, software        *
 *  distributed under the License is distributed on an "AS IS" BASIS,          *
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 *  See the License for the specific language governing permissions and        *
 *  limitations under the License.                                             *
 ******************************************************************************/

#include "bezier.h"

point bezier(point P[4], float frac) {
    point Q;
    float ifrac = 1-frac;
    float ifrac2 = ifrac*ifrac;
    float frac2 = frac*frac;
    float c0 = ifrac*ifrac2;
    float c1 = 3*ifrac2*frac;
    float c2 = 3*ifrac*frac2;
    float c3 = frac*frac2;
    Q.x = c0*P[0].x + c1*P[1].x + c2*P[2].x + c3*P[3].x;
    Q.y = c0*P[0].y + c1*P[1].y + c2*P[2].y + c3*P[3].y;
    return Q;
}
