#ifndef __CLASS_GAME_SPRITE_H_
#define __CLASS_GAME_SPRITE_H_

//#define SIZE 256

#include <fbs.h>
#include <gles/gl.h>
//#include "CGame.h"
#include "base/CGameRect.h" 
#include "base/CReferenceCounter.h"

//class CGameSpriteFrame;

class CGameSpriteFrame {

		TUint32* pixels;
		TInt texDim;
		GLuint texId;
	
	public:
	
		CGameSpriteFrame(const TFileName& filename, TUint32 id, const CGameRect& r, CFbsBitmap* loader);
		~CGameSpriteFrame();
		//void draw(const CGameRect& r, TInt layer);
		//TUint32* getPixels(){return this->pixels;}
		GLuint getTexID(void){return this->texId;}
		TInt getTexDim(void){return this->texDim;}
		
};

class CGame;

class CGameSprite /*: public CReferenceCounter*/ {

		RArray<CGameSpriteFrame*> frames;
		CGameRect size;
		CGame* game;
		TInt layer;

	public:
			
		CGameSprite(CGame* game, TPtrC filename, const CGameRect& r, TUint32 num);
		//~CGameSprite();
		
		//void setSize(const CGameRect& size){this->size = size;}
		//CGameRect getSize(){return this->size;}
		void setLayer(TInt l){this->layer = l;}
		TInt getLayer(void){return this->layer;}
		//void draw();
};

#endif
