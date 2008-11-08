#ifndef __CLASS_GAME_H_
#define __CLASS_GAME_H_

#include "base/gambi.h"
#include <gles/gl.h>
#include "event/IGameEventReceiver.h"
#include "CGameSprite.h"
#include "CGameRender.h"

#include <e32base.h>
#include <e32keys.h>

class CGameKeyEvent;
class CGameLogicalEvent;
class CGameCollisionEvent;

const TInt KErrDisplayNotCreated = -5001;
const TInt KErrDisplayNotInitialized = -5002;
const TInt KErrConfigFailed = -6001;
const TInt KErrCreateWindowSurfaceFailed = -6002;
const TInt KErrCreateContextFailed = -6003;
const TInt KErrSetContextFailed = -6004;
const TInt KErrCoeEnvNotCreated = -6005;
const TInt KErrCoeEnvNotInitialized = -6006;

const TInt timeForOneFrame = 1000000 / 25;

class CGame {

	public:
	    
	    static CGame* get_new();
	    ~CGame();
	    
	protected:
	     
	    CGame();	        
	    void init();
	    
	public:
	    
	    void init_screen( TUint aScreenWidth, TUint aScreenHeight );	        
	    TBool render( TInt64 aDurationSinceLastVisit );	    	            
	    void deinit();	    	        
	    void handle_key( TUint32 aKey, TBool aIsKeyUp );	    
	    void set_screen_size( TUint aScreenWidth, TUint aScreenHeight );
	    TInt getScreenWidth()
	    {
	    	return this->iScreenWidth;
		}
	    
	    TInt getScreenHeight()
	    {
	    	return this->iScreenHeight;
	    }
	    
	    TUint get_frame() const
	    {
	        return iFrame;
	    }
	    
	    void addKeyEventListener(IGameEventReceiver<CGameKeyEvent> r);
	    void addLogicalEventListener(IGameEventReceiver<CGameLogicalEvent> r);
	    void addCollisionEventListener(IGameEventReceiver<CGameCollisionEvent> r);
	    
	private:
	    
		CGameRender render;
	    TInt iScreenWidth;	    
	    TInt iScreenHeight;	    
	    TBool iGameOver;	    
	    TInt iZPos;
	    TBool iIsUpPressed;
	    TBool iIsDownPressed;
	    TUint iFrame;
};

#endif
