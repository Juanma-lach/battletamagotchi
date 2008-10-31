#include "CGameSprite.h"
#include "gambi.h"
#include <gles/gl.h> 
#include <stdio.h>

static const GLbyte vertices[4 * 3] =
     {
      0,  1,  0,
      1,  1,  0,
      1,  0,  0,
      0,  0,  0,
     };

static const GLfloat tex_coord[4 * 2] =
     {
      0,  0,
      0,  1,
      1,  1,
      1,  0,
     };

static const GLubyte triangles[2 * 3] =
    { 
    1,0,3,
    1,3,2,
    };

CGameSprite::CGameSprite(CGame* game, TPtrC filename, TUint32 num)
{
	this->game = game;
	this->layer = 0;

	RFs fsSession;
	User::LeaveIfError(fsSession.Connect());
	CleanupClosePushL( fsSession );
	TFileName name, path;
	name.Append(_L("c:"));
	fsSession.PrivatePath( path );
	name.Append( path );
	name.Append( filename );
		
	CFbsBitmap *iBitmap = new (ELeave) CFbsBitmap;
	
	CGameSpriteFrame* frame;
	
	for(TUint32 i = 0; i<num; i++){
		frame = new CGameSpriteFrame(name, i, iBitmap);
		this->frames.Append(frame);
	}
		    
	delete iBitmap;

}

CGameSprite::~CGameSprite(){
	
}

void CGameSprite::draw(void){
	this->frames[0]->draw(this->size, this->layer);
}

CGameSpriteFrame::CGameSpriteFrame(const TFileName& filename, TUint32 id, CFbsBitmap* loader){

	loader->Load(filename, id, 0);
	
	TInt i,j;
	TInt h = loader->SizeInPixels().iHeight;
	TInt w = loader->SizeInPixels().iWidth;
		 	    	    
	TRgb color;
	TPoint point;
	
	this->pixels = new TUint32[SIZE*SIZE];
		    	    
	for(i=0;i<SIZE;i++){
		for(j=0;j<SIZE;j++){
			point.SetXY(i,j);
			loader->GetPixel(color,point);
			this->pixels[i*SIZE+j] = color.Value();
		}
	}
	
	glGenTextures(1, &(this->texId)); 

	glBindTexture(GL_TEXTURE_2D, this->texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLint format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, SIZE, SIZE, 0,
	format, GL_UNSIGNED_BYTE, this->pixels);
	
}

void CGameSpriteFrame::draw(const CGameRect& r, TInt layer){

	glBindTexture(GL_TEXTURE_2D, this->texId);
	
	glVertexPointer(3, GL_BYTE, 0, vertices );        
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coord);
	
	glPushMatrix();
	
	glTranslatef(r.x, r.y, layer);
	glScalef(r.w, r.h, 1);
	
	glDrawElements( GL_TRIANGLES, 2 * 3, GL_UNSIGNED_BYTE, triangles );
	
	glPopMatrix();
}

CGameSpriteFrame::~CGameSpriteFrame(){
	delete [] this->pixels;
}
