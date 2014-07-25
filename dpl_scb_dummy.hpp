#ifndef __LM_DPL_SCB_DUMMY___
#define __LM_DPL_SCB_DUMMY___

#include <thread>
#include <chrono>
#include <iostream>
#include <string>

#include "dpl_scb.hpp"
#include "dpl_log.hpp"

class CSCB_Dummy : public CSCB {
public:
   CSCB_Dummy() : 
      CSCB(), 
      mSec(0), 
      mWaitTime(1000),
      mName("") {
   }

   CSCB_Dummy(unsigned int id) : 
      CSCB(id), 
      mSec(0),
      mWaitTime(0),
      mName("") {
   }

   CSCB_Dummy(unsigned int id, unsigned int waittime) : 
      CSCB(id), 
      mSec(0),
      mWaitTime(waittime),
      mName("") {
   }
   CSCB_Dummy(unsigned int id, unsigned int waittime, std::string name) : 
      CSCB(id), 
      mSec(0),
      mWaitTime(waittime),
      mName(name) { 
   }

   ~CSCB_Dummy() {
      __log("Ran for : " << mSec << " miliseconds");
   }
   
private:
   unsigned int mSec;
   unsigned int mWaitTime; // miliseconds
   std::string  mName;

   virtual void Run() {
      std::chrono::milliseconds duration(mWaitTime);
      std::this_thread::sleep_for(duration);
      mSec += mWaitTime;

      __log(mName << " ran once !");
   }
   
};



#endif
