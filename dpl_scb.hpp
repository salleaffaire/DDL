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

#ifndef DPL_SCB_HPP___
#define DPL_SCB_HPP___

#include <list>
#include <thread>
#include <string>

namespace DDL {

// Forward declarations
class CResource;
class CSequencer;
class CSCB;

class CSCB {
public:
   // Sequencer will be the only one allow to directly control an SCB
   // through the SCB Control API
   // Start
   // Wait
   // StopAndWait
   friend class CSequencer;

   // Core Function Type
   typedef void (*CF_t)(std::list<CResource *> &);

   CSCB();

   CSCB(unsigned int id);
 
   CSCB(unsigned int id, std::string name); 

   virtual ~CSCB();

   void WaitOn(CResource *res);
   
   void Produce(CResource *res);

   void Attach(CF_t f);

   static CSequencer *GetSequencer();

private:
   // SCB Identifier
   unsigned int mId;

   std::string mName;

   // SCB State variables
   // -------------------------------------------------------
   volatile unsigned int mIsRunning;
   
   // Thread structures
   std::thread *mpT;

   // When start
   std::list<CResource *> mDecrement;
   // When done
   std::list<CResource *> mIncrement;

   // Custom Core Function
   // -------------------------------------------------------
   CF_t CoreFunction;

   // Pointer to a Sequencer object
   // -------------------------------------------------------
   static CSequencer *mSequencer;

   // SBC Control API
   // -------------------------------------------------------
   int Start();

   int StopAndWait();

   int Wait();

   // SBC Initialization and Release API 
   // These functions are called by the constructors and
   // Destructor respectively
   // -------------------------------------------------------
   void Init();

   void Clean();

   // Common execution loop
   // -------------------------------------------------------
   static void EventLoop(CSCB *arg);

};

}

#endif
