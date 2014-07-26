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
