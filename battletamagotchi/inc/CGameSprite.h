#ifndef __CLASS_GAME_SPRITE_H_
#define __CLASS_GAME_SPRITE_H_

//#define SIZE 256

#include <fbs.h>
#include "CGame.h"
#include "CGameRect.h" 
#include "CReferenceCounter.h"

class CGame;

class CGameSpriteFrame {

		TUint32* pixels;
		TInt texDim;
		GLuint texId;
	
	public:
	
		CGameSpriteFrame(const TFileName& filename, TUint32 id, const CGameRect& r, CFbsBitmap* loader);
		~CGameSpriteFrame();
		void draw(const CGameRect& r, TInt layer);
		
};

class CGameSprite : public CReferenceCounter {

		RArray<CGameSpriteFrame*> frames;
		CGameRect size;
		CGame* game;
		TInt layer;

	public:
			
		CGameSprite(CGame* game, TPtrC filename, const CGameRect& r, TUint32 num);
		~CGameSprite();
		
		void setSize(const CGameRect& size){this->size = size;}
		CGameRect getSize(){return this->size;}
		void setLayer(TInt l){this->layer = l;}
		void draw();
};

#endif
