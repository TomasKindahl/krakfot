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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bezier.h"

int main(int argc, char **argv) {
    point P[4], Q;
    float frac;
    int ix;
    if(0) printf("num args %d\n", argc);
    if(argc < 10) {
        printf("too few arguments\n");
        return -1;
    }
    if(0)
    for(ix = 1; ix < argc; ix++) {
        printf("arg[%d] = %s\n", ix, argv[ix]);
    }

    for(ix = 0; ix < 4; ix++) {
        P[ix].x = atof(argv[(ix<<1)+1]);
        P[ix].y = atof(argv[(ix<<1)+2]);
        printf("<%g,%g>\n", P[ix].x, P[ix].y);
    }
    frac = atof(argv[9]);

    for(ix = 0; ix <= (int)frac; ix++) {
        float F = ix/frac;
        Q = bezier(P, F);
        printf("%g: <%g, %g>\n", F, Q.x, Q.y);
    }
    return 0;
}

