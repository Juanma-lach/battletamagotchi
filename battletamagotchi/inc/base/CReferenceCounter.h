#ifndef __CLASS_REFERENCE_COUNTER_H_
#define __CLASS_REFERENCE_COUNTER_H_

class CReferenceCounter {

		int counter;

	protected:
		
		CReferenceCounter(){this->counter  = 1; }
		
	public:
		
		void grab(void){
			this->counter++;
		}
		
		void drop(void){

			if(--this->counter == 0) delete this;
		}
};

#endif
