#include <GL/glut.h>

void matrizGL()
{
    double v[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, v);
    printf("\n");

    for(int l=0; l<4; l++)
    {
        printf("\n");
        for(int c=0; c<4; c++)
        {
            printf("  %.2lf", v[l*4+c]);
        }
    }
    getchar();/**/
}
