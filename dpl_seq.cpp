#include "dpl_seq.hpp"
#include "dpl_scb.hpp"
#include "dpl_log.hpp"

CSequencer *CSCB::mSequencer = 0;

CSequencer::CSequencer() {}

CSequencer::~CSequencer() {
   __log("Deleting the sequencer ...");
}

void CSequencer::RegisterSCB(CSCB *scb) {
   mSCBs.push_back(scb);
}

void CSequencer::UnregisterSCB(CSCB *scb) {
   mSCBs.remove(scb);
}

unsigned int CSequencer::GetNumberOfRegisteredResources() {
   return mSCBs.size();
}

void CSequencer::Start() {
   // Start all SCBs
   auto it = mSCBs.begin();
   for (;it != mSCBs.end();++it) {
      (*it)->Start();
   }
}

void CSequencer::StopAndWait() {
   // Stop and Wait on all SCBs
   auto it = mSCBs.begin();
   for (;it != mSCBs.end();++it) {
      (*it)->StopAndWait();
   }
}
