#include "vcore.h"

#include <vec.h>
#include <stdio.h>


void print_modelview()
{
    float mat[16] = { 0.f };
    glGetFloatv(GL_MODELVIEW_MATRIX, mat);
    for (int i=0; i<4; i++) {
        for (int j=0; j<4; j++) {
            printf("%5g  ", mat[i*4 + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void cube_face()
{
    glVertex3i(-1, -1, 0);
    glVertex3i(-1,  1, 0);
    glVertex3i( 1,  1, 0);
    glVertex3i( 1, -1, 0);
}

void default_draw_func()
{
    static float rot = 0.f;
    rot += 0.5f;
    glPushMatrix();

    glRotatef(rot, 0.f, 0.f, 1.f);
    glRotatef(sinf(rot), cosf(rot), sinf(rot), 0.f);
    glTranslatef(0.f, 0.f, -10.f);
    glMatrixMode(GL_MODELVIEW);
    glBegin(GL_QUADS);

    glColor3ub(100, 50, 50);
    cube_face();

    glEnd();

    glPopMatrix();
}

