#ifndef __CLASS_GAME_SPRITE_H_
#define __CLASS_GAME_SPRITE_H_

#define SIZE 256

#include <fbs.h>
#include "CGameRect.h" 

#include "Textureutils.h"
#include "CReferenceCounter.h"

class CGameSpriteFrame {

		TUint32* pixels;
		int ms;
	
	public:
	
		
};

class CGameSprite : public CReferenceCounter, public MTextureLoadingListener {

		//CFbsBitmap* iBitmap;		
		CGameSpriteFrame* frames;

	public:
	
		TUint32* data;
		CGameSprite(TPtrC filename, TUint8 * colorKey, CGameRect* clips, TUint32* times, TUint32 num);
		~CGameSprite();
		
		/** Called by the CTextureManager when it starts to load textures. */
		virtual void OnStartLoadingTexturesL(){}
		/** Called by the CTextureManager when it has loaded all the textures in the queue. */
		virtual void OnEndLoadingTexturesL(){}
		
		TUint32* getData(void){return this->data;}
};

#endif
