#ifndef __DPL_SEQ__
#define __DPL_SEQ__

#include <list>

class CSCB;


class CSequencer {
public:
   CSequencer();
   ~CSequencer();
   
   void RegisterSCB(CSCB *scb);
   void UnregisterSCB(CSCB *scb);

   void Start();
   void StopAndWait();

   unsigned int GetNumberOfRegisteredResources();

private:
   std::list<CSCB *> mSCBs;
};


#endif
