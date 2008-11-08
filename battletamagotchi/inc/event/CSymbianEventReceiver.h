#ifndef __CLASS_GAME_EVENT_RECEIVER_H_
#define __CLASS_GAME_EVENT_RECEIVER_H_

#include "CGameController.h"
#include "base/gambi.h"

class CSymbianEventReceiver : public CActive {

	public: 
	    /**
	     * Recieves events from the Windows Server.
	     *
	     * This function gets called in the 
	     * CGameController::ProcessBackgroundTasks() function,
	     * when the CActiveScheduler gets a chance to run.
	     */
	    virtual void RunL();
	    
	    /**
	     * Cancels event request from the Windows Server.
	     */    
	    virtual void DoCancel();
	
	    /**
	     * Static Constructor.
	     *
	     * @param aParent The Game Controller requesting for the events.
	     */    
	    static CSymbianEventReceiver* get_new( CGameController& aParent );
	    
	    /**
	     * Destructor.
	     */    
	    ~CSymbianEventReceiver();
	
	private: // Constructors
	    /**
	     * Constructor.
	     */
		CSymbianEventReceiver();
	
	    /**
	     * Two phase Constructor.
	     *
	     * @param aParent The Game controller which will be notified of
	     *                Windows Server events .
	     */    
	    void init( CGameController& aParent );                
	
	private: // Data.
	    /** The Windows Server Session */
	    RWsSession iWsSession;
	    
	    /** The Game controller */
	    CGameController* iParent;
};

#endif
