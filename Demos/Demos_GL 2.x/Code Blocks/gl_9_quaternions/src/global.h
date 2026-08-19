#include "Quaternion.h"
#include "Matrix.h"

//angulos de Euler
extern int roll, pitch, yaw;
extern int old_yaw, old_roll, old_pitch;
extern int d_yaw, d_roll, d_pitch;   //delta da variacao de angulo entre quadros

extern Quaternion g_quat_1, g_quat_2, g_quat_3, g_quat_4;
extern float g_angle;
extern Matrix g_mat;

void eixos();
void eixos_2();
void eixos_3(Vector3 eixo);
void text(int x, int y, char *t);

void euler();
void quaternion_1();
void quaternion_2();
void quaternion_3();
void eixo_arbitrario_quat();
void eixo_arbitrario_euler();

void cria_matriz_rotacao_eixo(Vector3 axis, float angle, float m[16]);
