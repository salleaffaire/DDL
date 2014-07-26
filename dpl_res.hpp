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

#ifndef DPL_RES_HPP___
#define DPL_RES_HPP___

#include <list>
#include <string>

#include "dpl_log.hpp"
#include "semaphore.hpp"

#define DPL_RESOURCE_TIMEOUT_DEFAULT 3000 // Miliseconds

class CResource {
public:
   CResource() : 
      mSemCount(0), 
      mInitialCount(0),
      mName("") {
   }

   CResource(unsigned int count) : 
      mSemCount(count),
      mInitialCount(count),
      mName("") {
   }
   
   CResource(unsigned int count, std::string name) : 
      mSemCount(count),
      mInitialCount(count),
      mName(name) {
   }

   ~CResource() {
      __log("Deleting " << mName);
   }

   void Release() {
      mSemCount.Signal();
   }

   // Returns true if a resource was acquired
   // returns false if it timed out. 
   bool Acquire() {
      std::chrono::milliseconds duration(DPL_RESOURCE_TIMEOUT_DEFAULT); 
   
      mIsAcquired = mSemCount.Wait(duration);
   }

   bool IsAcquired() {
      return mIsAcquired;
   }

private:

   Semaphore    mSemCount;
   unsigned int mInitialCount;

   std::string  mName;

   bool         mIsAcquired;
};



#endif
