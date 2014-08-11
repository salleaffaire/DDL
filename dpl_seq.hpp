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

#ifndef __DPL_SEQ__
#define __DPL_SEQ__

#include <list>
#include <map>

namespace DDL {

class CSCB;
class CResource;

class CSequencer {
public:
   CSequencer();
   ~CSequencer();
   
   void RegisterSCB(CSCB *scb);
   void UnregisterSCB(CSCB *scb);

   void RegisterResourceOnWait(CResource *res, CSCB *scb);
   void RegisterResourceOnProduce(CResource *res, CSCB *scb);

   void Start();
   void StopAndWait();

   unsigned int GetNumberOfRegisteredResources();

private:
   // SCB list
   std::list<CSCB *>      mSCBs;

   // Resource Maps
   std::map<CResource *, std::list<CSCB *>> mIsWaitedOnBy;
   std::map<CResource *, std::list<CSCB *>> mIsProducedBy;
};

}

#endif
