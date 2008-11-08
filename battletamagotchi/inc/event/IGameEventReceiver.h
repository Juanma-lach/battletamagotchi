#ifndef IGAMEEVENTRECEIVER_H_
#define IGAMEEVENTRECEIVER_H_

template <class EventType> class IGameEventReceiver {

	public:
		
		virtual receiveEvent(EventType e) = 0;
	
};

#endif /*IGAMEEVENTRECEIVER_H_*/
