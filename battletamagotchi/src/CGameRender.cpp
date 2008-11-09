#include "CGameRender.h"
#include <gles/gl.h>

static const GLbyte vertices[4 * 3] =
     {
    	0,  1,  0,
     	1,  1,  0,
     	1,  0,  0,
     	0,  0,  0,
     };

/*static const GLfloat tex_coord[4 * 2] =
     {
      1,  0,
      0,  0,
      0,  1,
      1,  1,      
     };*/

static const GLubyte triangles[2 * 3] =
    { 
    	1,0,3,
    	1,3,2,
    };

static void ogl_draw(const CGameRect& r, TInt texDim, TInt layer, GLuint texID){

	const GLfloat tex_coord[4 * 2] =
 	{
		0,  0,
  		r.w/((float)texDim),  0,
  		r.w/((float)texDim),  r.h/((float)texDim),
  		0,  r.h/((float)texDim),            
 	};

	glBindTexture(GL_TEXTURE_2D, texID);

	glVertexPointer(3, GL_BYTE, 0, vertices );        
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);

	glPushMatrix();

	glTranslatef(r.x, r.y, layer);
	glScalef(r.w, r.h, 1);

	glDrawElements(GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, triangles);

	glPopMatrix();
}

void CGameRender::addGameObject(CGameObject* o){
	if(this->objs.Find(o) == KErrNotFound) this->objs.Append(o);
}

CGameObject* CGameRender::removeGameObject(CGameObject* o){
	TInt i = this->objs.Find(o);
	if(i == KErrNotFound) return NULL;
	o = this->objs[i];
	this->objs.Remove(i);
	return o;
}
	
void CGameRender::draw(){
	
	TInt i, size = this->objs.Count();
	
	for(i=0; i<size;i++){
		CGameSpriteFrame* sf = objs[i]->getCurrentSpriteFrame();
		ogl_draw(objs[i]->rect, sf->getTexDim(), objs[i]->getLayer(), sf->getTexID());
	}
}