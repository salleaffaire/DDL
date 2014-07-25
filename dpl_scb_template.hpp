#ifndef __LM_DPL_SCB_TEMPLATE___
#define __LM_DPL_SCB_TEMPLATE___

#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#include "dpl_seq.hpp"

template <class IN, class OUT>
class CSCB_Template : public CSCB {
public:
   typedef void (*CF)(std::vector<IN *>, std::vector<OUT *>);
   typedef std::vector<IN *> (*AIF)();
   typedef std::vector<OUT *> (*AOF)();

   CSCB_Template() : 
      CSCB(), 
      mName(""),
      mF(0),
      mAIF(0),
      mAOF(0) {
   }

   CSCB_Template(unsigned int id, std::string name) : 
      CSCB(id), 
      mName(name),
      mF(0),
      mAIF(0),
      mAOF(0) { 
   }

   CSCB_Template(unsigned int id, std::string name, CF f) : 
      CSCB(id), 
      mName(name),
      mF(f),
      mAIF(0),
      mAOF(0) { 
   }

   ~CSCB_Template() {

   }

   void SetAIF(AIF *aif) {
      mAIF = aif;
   }
   
   void SetAOF(AOF *aof) {
      mAOF = aof;
   }
   
private:
   unsigned int mSec;
   unsigned int mWaitTime; // miliseconds
   std::string  mName;
   
   // Core function
   CF           mF;
   
   // Acquire In Function
   AIF          mAIF;
   
   // Acquire Out Function
   AOF          mAOF;

   virtual void Run() {
      std::vector<IN *> vec_in  = std::vector<IN *>();
      std::vector<OUT *> vec_out  = std::vector<OUT *>();

      if (mAIF) {
	 vec_in = mAIF();
      }
      if (mAOF) {
	 vec_out = mAOF();
      }
      mF(vec_in, vec_out);
   }
   
};



#endif
