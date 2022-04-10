/*
 *  krakfot
 *
 *     Copyright 2022 Tomas Kindahl
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#include "bezier.h"

int fullScreen = 0;

/* https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Paths  */
/* https://personal.math.ubc.ca/~cass/graphics/text/www/pdf/ch6.pdf */

/*  here7  svg path    PostScript  */
/*---------------------------------*/
/*  MV     M           moveto      */
/*  LT     L           lineto      */
/*  C1     (C,)        (curveto)   */
/*  C2     (C,)        (curveto)   */
/*  CT     C           curveto     */
/*  STK    (implicit)  stroke      */
/*  ADJ    (implicit)  translate   */
/*---------------------------------*/

#define MV_OP  1
#define MV(X, Y)  {MV_OP,  (X), (Y)}
#define LT_OP  2
#define LT(X, Y)  {LT_OP,  (X), (Y)}
#define C1_OP  3
#define C1(X, Y)  {C1_OP,  (X), (Y)}
#define C2_OP  4
#define C2(X, Y)  {C2_OP,  (X), (Y)}
#define CT_OP  5
#define CT(X, Y)  {CT_OP,  (X), (Y)}
#define STK_OP 6
#define STK(X, Y) {STK_OP, (X), (Y)}
#define ADJ_OP 7
#define ADJ(X)    {ADJ_OP, (X),  0}

typedef struct _oparg_S {
    int type;
    GLfloat x, y;
} oparg;


oparg spaceGlyph[] = {
    ADJ(6.0)
};

oparg Aglyph[] = {
    MV(0, 0), STK(2, 0),
    MV(6, 0), STK(8, 0),
    MV(1, 0), LT(4, 10.2), STK(7, 0),
    MV(2, 3.5), STK(6, 3.5), ADJ(8.0)
};

oparg Bglyph[] = {
    MV(2, 0), STK(2, 10),
    MV(2, 0), STK(2, 10),
    MV(1, 10), LT(3.5, 10), C1(7.5, 10), C2(7.5, 5), CT(3.5, 5), STK(2, 5),
    MV(2, 5), LT(4, 5), C1(8, 5), C2(8, 0), CT(4, 0), STK(1, 0),
    ADJ(8.0)
};

oparg Cglyph[] = {
	MV(6.5, 10.2), LT(6.5, 8.2), C1(6.5, 9.2), C2(5.5, 10.2), CT(4.5, 10.2), 
	MV(4, 10.2), C1(0.2, 10.2), C2(0.3, -0.2), CT(4, -0.2), LT(4.5, -0.2),
	C1(5.5, -0.2), C2(6.5, 0.8), CT(6.5, 2), STK(6.5, 2),
    ADJ(8.0)
};

oparg Dglyph[] = {
    MV(1, 0), LT(3, 0), C1(8, 0),
    C2(8, 10), CT(3, 10), LT(3, 10), STK(1, 10),
    MV(2, 0), STK(2, 10),
    ADJ(8.0),
};

oparg Eglyph[] = {
    MV(1, 0), MV(6.5, 0), STK(6.5, 1),
    MV(1, 10), MV(6.5, 10), STK(6.5, 9),
    MV(2, 0), STK(2, 10),
    MV(2, 5), STK(5.5, 5),
    MV(5.5, 4), STK(5.5, 6), 
    ADJ(8.0)
};

oparg Fglyph[] = {
    MV(1, 0), STK(4, 0),
    MV(1, 10), MV(6.5, 10), STK(6.5, 9),
    MV(2, 0), STK(2, 10),
    MV(2, 5), STK(5.5, 5),
    MV(5.5, 4), STK(5.5, 6), 
    ADJ(8.0)
};

oparg Gglyph[] = {
	MV(6.5, 10.2), LT(6.5, 8.2), C1(6.5, 9.2), C2(5.5, 10.2), CT(4.5, 10.2), 
	MV(4, 10.2), C1(0.2, 10.2), C2(0.3, -0.2), CT(4, -0.2), LT(4.5, -0.2),
	C1(5.5, -0.2), C2(6.5, 0.8), CT(6.5, 2), STK(6.5, 2),
	MV(4, 5), LT(6.5, 5), STK(6.5, 0.2),
    ADJ(8.0)
};

oparg Hglyph[] = {
    MV(0.5, 0), STK(2.5, 0), MV(0.5, 10), STK(2.5, 10),
    MV(1.5, 0), STK(1.5, 10), MV(5.5, 0), STK(7.5, 0),
    MV(5.5, 10), STK(7.5, 10), MV(6.5, 0), STK(6.5, 10),
    MV(1.5, 5), STK(6.5, 5), ADJ(8)
};

oparg Iglyph[] = {
    MV(2, 0), STK(4, 0),
    MV(2, 10), STK(4, 10),
    MV(3, 0), STK(3, 10), ADJ(6)
};

oparg Jglyph[] = {
	MV(4.5, 10), STK(7.5, 10),
	MV(1, 2.5), C1(1, -1.3), C2(6.5, -1.3), 
	CT(6.5, 2.5), STK(6.5, 10),
	ADJ(8.0)
};

oparg Kglyph[] = {
    MV(1, 0), STK(4, 0),
    MV(2, 0), STK(2, 10),
    MV(7.5, 10), C1(7, 10), C2(6, 9), CT(6, 8),
    C1(6, 7), C2(5, 5), CT(3, 5), STK(2, 5),
    MV(3.5, 5), C1(6, 5), C2(5, 0), CT(7, 0), STK(8, 0),
    ADJ(8.0)
};

oparg Lglyph[] = {
    MV(1, 0), MV(6.5, 0), STK(6.5, 1),
    MV(1, 10), STK(4, 10),
    MV(2, 0), STK(2, 10),
    ADJ(8.0)
};

oparg Mglyph[] = {
    MV(0.5, 0), STK(2.5, 0), 
    MV(1.5, 0), STK(1.5, 10), MV(7.5, 0), STK(9.5, 0),
    MV(8.5, 0), STK(8.5, 10),
    MV(0.5, 10), LT(1.5, 10), LT(5, 3), LT(8.5, 10), STK(9.5, 10),
    ADJ(10)
};

oparg Nglyph[] = {
    MV(0.5, 0), STK(2.5, 0), 
    MV(5.5, 10), STK(7.5, 10),
    MV(1.5, 0), STK(1.5, 10),
    MV(6.5, 0), STK(6.5, 10),
    MV(0.5, 10), LT(1.5, 10), LT(6.5, 0), STK(7.5, 0),
    ADJ(8)
};

oparg Oglyph[] = {
	MV(5, 10.2), C1(0.5, 10.2), C2(0.5, -0.2), CT(5, -0.2), 
	MV(5, -0.2), C1(9.5, -0.2), C2(9.5, 10.2), CT(5, 10.2), STK(5, 10.2),
	ADJ(10.0)
};

oparg Pglyph[] = {
    MV(1, 0), STK(4, 0),
    MV(2, 0), STK(2, 10),
    MV(1, 10), LT(3.5, 10), C1(7.5, 10), C2(7.5, 5), CT(3.5, 5), 
    STK(2, 5),
    ADJ(8.0)
};

oparg Qglyph[] = {
	MV(5, 10.2), C1(0.5, 10.2), C2(0.5, -0.2), CT(5, -0.2), 
	MV(5, -0.2), C1(9.5, -0.2), C2(9.5, 10.2), CT(5, 10.2), STK(5, 10.2),
    MV(5, -0.2), C1(3, -0.2), C2(3, 3), CT(5, 3), 
    C1(5.75, 3), C2(6.5, 2.5), CT(6.5, 0), C1(6.5, -0.5), C2(7, -1), CT(8, -1),
    STK(9.5, -1), 
	ADJ(10.0)
};

oparg Rglyph[] = {
    MV(1, 0), STK(4, 0),
    MV(2, 0), STK(2, 10),
    MV(1, 10), LT(3.5, 10), C1(7.5, 10), C2(7.5, 5), CT(3.5, 5), STK(2, 5),
    MV(3.5, 5), C1(6, 5), C2(5, 0), CT(7, 0), STK(8, 0),
    ADJ(8.0)
};

oparg Sglyph[] = {
	MV(6.5, 10), LT(6.5, 8), C1(6.5, 9), C2(5.5, 10.2), CT(4.5, 10.2),
	MV(4, 10.2), C1(0.3, 10.2), C2(0.3, 5), CT(4, 5), LT(4.5, 5), 
	C1(8, 5), C2(8, -0.3), CT(4.5, -0.3), LT(4, -0.3),
	C1(3, -0.3), C2(1.2, 0.7), CT(1.2, 1.7), STK(1.2, 2),
    ADJ(8.0)
};

oparg Tglyph[] = {
	MV(1, 9), LT(1, 10), LT(7, 10), STK(7, 9),
	MV(4, 0), STK(4, 10), MV(2, 0), STK(6, 0),
	ADJ(8.0)
};

oparg Uglyph[] = {
	MV(0.5, 10), STK(2.5, 10),
	MV(5.5, 10), STK(7.5, 10),
	MV(1.5, 10), LT(1.5, 2.5), C1(1.5, -1.3), C2(6.5, -1.3), 
	CT(6.5, 2.5), STK(6.5, 10),
	ADJ(8.0)
};

oparg Vglyph[] = {
	MV(0.5, 10), STK(2.5, 10),
	MV(5.5, 10), STK(7.5, 10),
	MV(1.5, 10), LT(4, -0.3), STK(6.5, 10),
	ADJ(8.0)
};

oparg Wglyph[] = {
	MV(0.5, 10), STK(2.5, 10),
	MV(7.5, 10), STK(9.5, 10),
	MV(1.5, 10), LT(3, -0.2), LT(5, 6), LT(7, -0.2), STK(8.5, 10),
	ADJ(10.0)
};

oparg Xglyph[] = {
    MV(0.5, 0), STK(2.5, 0), MV(0.5, 10), STK(2.5, 10),
    MV(5.5, 0), STK(7.5, 0),  MV(5.5, 10), STK(7.5, 10), 
    MV(6.5, 0), STK(1.5, 10), MV(1.5, 0), STK(6.5, 10),    
    ADJ(8)
};

oparg Yglyph[] = {
	MV(0.5, 10), STK(2.5, 10),
	MV(5.5, 10), STK(7.5, 10),
	MV(1.5, 10), LT(4, 5), STK(6.5, 10),
	MV(4, 0), STK(4, 5),
	MV(3, 0), STK(5, 0),
	ADJ(8.0)
};

oparg Zglyph[] = {
	MV(1.5, 9), LT(1.5, 10), LT(6.5, 10),
	LT(1.5, 0), LT(6.5, 0), STK(6.5, 1),
	ADJ(8.0)
};

point oparg_to_point(oparg *P) {
	point Q;
	Q.x = P->x;
	Q.y = P->y;
	return Q;
}

static void drawGlyph(oparg *l) {
	oparg *P0, *P1, *P2, *P3; /* Saving points for Cubic Bézier */
	point P[4], Q;
	int ix;
    while(1) {
        switch (l->type) {
          case MV_OP:
            glBegin(GL_LINE_STRIP);
            P0 = l;
            glVertex2fv(&l->x);
            break;
          case LT_OP:
            P0 = l;
            glVertex2fv(&l->x);
            break;
          case C1_OP:
            P1 = l;
            break;
          case C2_OP:
            P2 = l;
            break;
          case CT_OP:
            P3 = l;
            P[0] = oparg_to_point(P0);
            P[1] = oparg_to_point(P1);
            P[2] = oparg_to_point(P2);
            P[3] = oparg_to_point(P3);
            for(ix = 1; ix <= 16; ix++) {
            	float frac = ix/16.0;
            	Q = bezier(P, frac);
                glVertex2fv((const float *)&Q);
            }
            P0 = l;
            break;
          case STK_OP:
            P0 = 0;
            glVertex2fv(&l->x);
            glEnd();
            break;
          case ADJ_OP:
            glTranslatef(l->x, 0.0, 0.0);
            return;
        }
        l++;
    }
}

char glyphs[] = ""
"{U+40: mv 0 0 stk 2 0 mv 6 0 stk 8 0 "
"       mv 1 0 lt 4 10.2 stk 7 0 mv 2 3.5 stk 6 3.5 "
"       adj 8;"
"}"
;

void mklist(GLuint L, int ch, oparg *glyph) {
    glNewList(L+ch, GL_COMPILE); drawGlyph(glyph); glEndList();
}

static void init(void) {
    GLuint L;

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glLineWidth(2.0);

    L = glGenLists(65536);
    glListBase(L);
    mklist(L, 'A', Aglyph);
    mklist(L, 'B', Bglyph);
    mklist(L, 'C', Cglyph);
    mklist(L, 'D', Dglyph);
    mklist(L, 'E', Eglyph);
    mklist(L, 'F', Fglyph);
    mklist(L, 'G', Gglyph);
    mklist(L, 'H', Hglyph);
    mklist(L, 'I', Iglyph);
    mklist(L, 'J', Jglyph);
    mklist(L, 'K', Kglyph);
    mklist(L, 'L', Lglyph);
    mklist(L, 'M', Mglyph);
    mklist(L, 'N', Nglyph);
    mklist(L, 'O', Oglyph);
    mklist(L, 'P', Pglyph);
    mklist(L, 'Q', Qglyph);
    mklist(L, 'R', Rglyph);
    mklist(L, 'S', Sglyph);
    mklist(L, 'T', Tglyph);
    mklist(L, 'U', Uglyph);
    mklist(L, 'V', Vglyph);
    mklist(L, 'W', Wglyph);
    mklist(L, 'X', Xglyph);
    mklist(L, 'Y', Yglyph);
    mklist(L, 'Z', Zglyph);
    mklist(L, ' ', spaceGlyph);
}

char *str1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *str2 = "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG";

static void printString(char *s) {
    GLsizei len = strlen(s);
    glCallLists(len, GL_BYTE, (GLbyte *)s);
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);
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
        printString(str2);
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