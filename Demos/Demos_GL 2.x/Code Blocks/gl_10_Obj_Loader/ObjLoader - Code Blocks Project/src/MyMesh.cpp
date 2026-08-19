#include "MyMesh.h"

MyMesh::MyMesh() : _ObjMesh() 
{}

void MyMesh::draw(void){
	if(loaded){
		GLfloat mat[3];
		int vsize = v.size();
		int nsize = n.size();
		int uvsize = uv.size();
		int fsize = f.size();
		for(int i = 0; i < fsize; i++){
			if(f[i].material != NULL){
				if(f[i].material->textured){
					glEnable(GL_TEXTURE_2D);
					glBindTexture(GL_TEXTURE_2D, f[i].material->texture);
					glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				}
				mat[0] = f[i].material->ambient.r;
				mat[1] = f[i].material->ambient.g;
				mat[2] = f[i].material->ambient.b;
				glMaterialfv(GL_FRONT, GL_AMBIENT, mat);

				mat[0] = f[i].material->diffuse.r;
				mat[1] = f[i].material->diffuse.g;
				mat[2] = f[i].material->diffuse.b;
				glMaterialfv(GL_FRONT, GL_DIFFUSE, mat);

				mat[0] = f[i].material->specular.r;
				mat[1] = f[i].material->specular.g;
				mat[2] = f[i].material->specular.b;
				glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
			}
			glBegin(GL_TRIANGLES);
			for(int j = 0; j < 3; j++){
				if((nsize > 0) && (f[i].n[j] < nsize)){
					glNormal3f(  n[f[i].n[j]].x    ,  n[f[i].n[j]].y    ,  n[f[i].n[j]].z  );
				}
				if((uvsize > 0) && (f[i].uv[j] < uvsize) && (f[i].material->texture > 0)){
					glTexCoord2f(	uv[f[i].uv[j]].u,	uv[f[i].uv[j]].v	);
				}
				if((vsize > 0) && (f[i].v[j] < vsize)){
					glVertex3f(  v[f[i].v[j]].x    ,  v[f[i].v[j]].y    ,  v[f[i].v[j]].z  );
				}
			}
			glEnd();
			if(f[i].material != NULL){
				if(f[i].material->textured){
					glDisable(GL_TEXTURE_2D);
				}
			}
		}
	}
}