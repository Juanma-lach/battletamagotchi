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

TInt minor2Pot(TInt val){
	
	if(val>128) return 256;
	if(val>64) return 128;
	if(val>32) return 64;
	return 32;
}

CGameSprite::CGameSprite(CGame* game, TPtrC filename, const CGameRect& r, TUint32 num)
{
	this->game = game;
	this->layer = 0;
	this->size = r;
	
	//printf("Largura e altura: %d %d\n", size.w, size.h);

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
		frame = new CGameSpriteFrame(name, i, r, iBitmap);
		this->frames.Append(frame);
	}
		    
	delete iBitmap;

}

CGameSprite::~CGameSprite(){
	
}

void CGameSprite::draw(void){
	this->frames[0]->draw(this->size, this->layer);
}

CGameSpriteFrame::CGameSpriteFrame(const TFileName& filename, TUint32 id, const CGameRect& r, CFbsBitmap* loader){

	loader->Load(filename, id, 0);
	
	TInt i,j;
	TInt h = loader->SizeInPixels().iHeight;
	TInt w = loader->SizeInPixels().iWidth;
	
	h = (h<r.h)?h:r.h;
	w = (w<r.w)?w:r.w;
	
	TInt H = minor2Pot(h);
	TInt W = minor2Pot(w);
	
	if(H>W)W=H;
	else H=W;
	
	this->texDim = H;
			 	    	    
	TRgb color;
	TPoint point;
	
	this->pixels = new TUint32[W*H];
		    	    
	for(i=0;i<w;i++){
		for(j=0;j<h;j++){
			point.SetXY(i,j);
			loader->GetPixel(color,point);
			this->pixels[i+j*W] = color.Value();
		}
	}
	
	glGenTextures(1, &(this->texId)); 

	glBindTexture(GL_TEXTURE_2D, this->texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLint format = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, format, W, H, 0,
	format, GL_UNSIGNED_BYTE, this->pixels);
	
}

void CGameSpriteFrame::draw(const CGameRect& r, TInt layer){

	//printf("Largura e altura: %d %d\n", r.w, r.h);

	const GLfloat tex_coord[4 * 2] =
     {
      0,  0,
      r.w/((float)this->texDim),  0,
      r.w/((float)this->texDim),  r.h/((float)this->texDim),
      0,  r.h/((float)this->texDim),            
     };
	
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
