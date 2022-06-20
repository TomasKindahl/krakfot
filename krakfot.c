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
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "bezier.h"
#include "scan.h"
#include "parse.h"

int fullScreen = 0;

static void drawGlyph(glyph *G) {
	int line, num_lines;
	int pnt, num_pnts;
	int ix;
	float adv_x, adv_y;
    float P[2];
    point bez_ctrl[4];
    int bez_num = 0;

	if(!G) return;
	num_lines = get_num_lines(G);
	for(line = 0; line < num_lines; line++) {
		num_pnts = get_num_points_on_line(G, line);
		glBegin(GL_LINE_STRIP);
		for(pnt = 0; pnt < num_pnts; pnt++) {
			switch(is_bez_ctrl(G, line, pnt)) {
			  case 0:
			  	if(bez_num == 0) {
			  		float x, y;
			  		x = get_x(G, line, pnt);
			  		y = get_y(G, line, pnt);
			  	    bez_ctrl[0].x = x;
			  	    bez_ctrl[0].y = y;
			  	    P[0] = x;
			  	    P[1] = y;
			  	    glVertex2fv(P);
			  	}
			  	else if(bez_num == 2) {
			  		point Q;
			  		float x, y;
			  		bez_num = 0;
			  		x = get_x(G, line, pnt);
			  		y = get_y(G, line, pnt);
			  		
			  	    bez_ctrl[3].x = x;
			  	    bez_ctrl[3].y = y;
			  	    for(ix = 1; ix <= 16; ix++) {
            	        float frac = ix/16.0;
            	        Q = bezier(bez_ctrl, frac);
                        glVertex2fv((const float *)&Q);
                    }
			  	    bez_ctrl[0].x = x;
			  	    bez_ctrl[0].y = y;
			  	}
			  	break;
			  case 1:
			  	if(bez_num == 0) {
			  	    bez_num = 1;
			  	    bez_ctrl[1].x = get_x(G, line, pnt);
			  	    bez_ctrl[1].y = get_y(G, line, pnt);
			  	}
			    else if(bez_num == 1) {
			  	    bez_num = 2;
			  	    bez_ctrl[2].x = get_x(G, line, pnt);
			  	    bez_ctrl[2].y = get_y(G, line, pnt);
			  	}
			  	break;
			}  	
		}
		glEnd();
	}
	adv_x = get_adv_x(G);
	adv_y = get_adv_y(G);
	glTranslatef(adv_x, adv_y, 0.0);
}

void mkglyph(GLuint L, int ch, parser *P) {
    glNewList(L+ch, GL_COMPILE); drawGlyph(getelem(P, ch)); glEndList();
}

int tryLoadGlyphs(parser *P, char *path) {
    scanner *scan = new_scanner(path);

	if(!scan) {
		fprintf(stderr, "INFO: glyph path %s not loaded\n", path);
		return 0;
	}
	
    set_scanner(P, scan);
	parse_glyphs(P);
	return 1;
}

static void initGlyphs(void) {
    GLuint L;
    int ix;
    parser *P = new_parser(65536);
    char *USE_INTERNAL_DATA = 0;

    tryLoadGlyphs(P, USE_INTERNAL_DATA);
    tryLoadGlyphs(P, "~/.krakfot/glyphs.gly");
    tryLoadGlyphs(P, "glyphs.gly");
    /* print_glyphs(P,1,0,1,0); */

    L = glGenLists(65536);
    glListBase(L);
    mkglyph(L, ' ', P);
    for(ix = 0; ix <= 255; ix++)
    	mkglyph(L, ix, P);
}

static void init(void) {
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(2.0);
    
    initGlyphs();

}

char *str1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *str2 = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
char *str3 = "abcdefghijklmnopqrstuvwxyz";

static void printString(char *s) {
    GLsizei len = strlen(s);
    glCallLists(len, GL_BYTE, (GLbyte *)s);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
    {
        glPushMatrix();
        glTranslatef(3.0, 180.0, 0.0);
        glScalef(6.0, 6.0, 6.0);
        printString(str3);
        glPopMatrix();
    }
    {
        glPushMatrix();
        glTranslatef(3.0, 100.0, 0.0);
        glScalef(6.0, 6.0, 6.0);
        printString(str1);
        glPopMatrix();
    }
    {
        glPushMatrix();
        glTranslatef(3.0, 50.0, 0.0);
        glScalef(3.0, 3.0, 3.0);
        printString(str2);
        glPopMatrix();
    }
    {
        glPushMatrix();
        glTranslatef(3.0, 13.0, 0.0);
        glScalef(2.0, 2.0, 2.0);
        printString(str1);
        glPopMatrix();
    }
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)width, 0.0, (GLdouble)height);
}

#define CTRL_F 0x06
#define CTRL_H 0x08
#define CTRL_Q 0x11
#define CTRL_R 0x12

void keyboard(unsigned char key, int x, int y) {
    printf("'%c' (%x)\n", key, key);
    switch (key) {
      case ' ':
        glutPostRedisplay();
        break;
      case CTRL_H:
        /* set some help-mode here! */
        break;
      case CTRL_F:
        if(fullScreen) 
            glutLeaveFullScreen();
        else 
            glutFullScreen();
        fullScreen = 1 - fullScreen;
        break;
      case CTRL_R:
        /* some reload here! */
        break;
      case CTRL_Q:
        exit(0);
      default:
        break;
    }
}

/*  Main Loop
 *  Open window with initial window size, title bar,
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    
    /* Multiple windows? https://stackoverflow.com/questions/10465462/multiple-windows-opengl-glut */

    glutInitWindowSize(1000, 1000);
    glutCreateWindow("krakfot");
    init();
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    /* glutSpecialFunc(specialKey); */
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
