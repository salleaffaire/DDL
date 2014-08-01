//    =======================================================================
//    DDL is the Data Driven Language and provides a simple framework
//    to build asynchonous systems
//    ======================================================================= 
//    Copyright (C) 2014  Luc Martel
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//    ======================================================================= 

#include "dpl_seq.hpp"
#include "dpl_scb.hpp"
#include "dpl_res.hpp"
#include "dpl_log.hpp"

using namespace DDL;

CSCB::CSCB() : 
   mId(0),
   CoreFunction((CF_t)0),
   mName(""),
   mIsRunning(0),
   mpT((std::thread *)0) {
   Init();
}

CSCB::CSCB(unsigned int id) : 
   mId(id),
   CoreFunction((CF_t)0),
   mName(""),
   mIsRunning(0),
   mpT((std::thread *)0) {
   Init();
}

CSCB::CSCB(unsigned int id, std::string name) : 
   mId(id),
   CoreFunction((CF_t)0),
   mName(name),
   mIsRunning(0),
   mpT((std::thread *)0) {
   Init();
}

CSCB::~CSCB() {
   __log("Deleting SCB id " << mId);
   
   if (mpT) {
      delete mpT;
   }
   
   Clean();
}

int CSCB::Start() {
   mIsRunning = 1;
   mpT = new std::thread(CSCB::EventLoop, this);
}

int CSCB::StopAndWait() {
   mIsRunning = 0;
   Wait();
}

void CSCB::WaitOn(CResource *res) {
   mDecrement.push_back(res);
}

void CSCB::Produce(CResource *res)  {
   mIncrement.push_back(res);
}

CSequencer *CSCB::GetSequencer() {
   return mSequencer;
}

void CSCB::Attach(CF_t f) {
   CoreFunction = f;
}

int CSCB::Wait() {
   mpT->join();
}

void CSCB::EventLoop(CSCB *arg) {
   __log("Starting thread CSCB ID " << arg->mId);

   while (arg->mIsRunning) {
      // Are all resources acquired
      bool have_all_resources = true;
      
      // Acquire (Decrement) all resources
      // This will block if one of the resource is not available
      // This implementation works since only 1 SCB can consume a given
      // resource
      {
	 auto it = arg->mDecrement.begin();
	 for (;it != arg->mDecrement.end(); 
	      ++it) {
	    have_all_resources &= (*it)->Acquire();
	 }
      }
      
      if (have_all_resources) {
	 // Execute SCB's core routine
	 if (arg->CoreFunction) arg->CoreFunction(arg->mDecrement);
      
	 // Release (Increment) all resources
	 {
	    auto it =  arg->mIncrement.begin();
	    for (;it != arg->mIncrement.end(); 
		 ++it) {
	       (*it)->Release();
	    }
	 }
      }
      else {
	 __log("CSCB ID " << arg->mId << " timed out");
	 // Release the resources that were properly acquired
	 {
	    auto it = arg->mDecrement.begin();
	    for (;it != arg->mDecrement.end();
		 ++it) {
	       if ((*it)->IsAcquired()) (*it)->Release();
	    }
	 }
      }
   }
}

void CSCB::Init() {
   if (mSequencer == (CSequencer *)0) {
      mSequencer = new CSequencer;
   }
   mSequencer->RegisterSCB(this); 
}

void CSCB::Clean() {
   mSequencer->UnregisterSCB(this);
   
   if (mSequencer->GetNumberOfRegisteredResources() == 0) {
      delete mSequencer;
      mSequencer = (CSequencer *)0;
   }
}
