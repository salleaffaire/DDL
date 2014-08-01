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
#include <vector>

#include "dpl_log.hpp"
#include "semaphore.hpp"

#define DPL_RESOURCE_TIMEOUT_DEFAULT 3000 // Miliseconds

namespace DDL {

class CResourceEnvelop {
public:
   CResourceEnvelop(unsigned int number) : mPhysicalResources(number) {
   }
   ~CResourceEnvelop() {
   }

private:
   // Prevent a call to the default constructor. It shoundn't happen. 
   CResourceEnvelop();

   std::vector<void *> mPhysicalResources;
};


class CResource {
public:
   CResource() : 
      mSemCount(0), 
      mInitialCount(0),
      mCurrentIndex(0),
      mBoundResource((CResource *)0),
      mPhysicalResource((CResourceEnvelop *)0),
      mName("") {
      Init(mName);
      Warn(mInitialCount, mBoundResource);
   }

   CResource(unsigned int count) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource((CResource *)0),
      mPhysicalResource((CResourceEnvelop *)0),
      mName("") {
      Init(mName);
      Warn(mInitialCount, mBoundResource);
   }
   
   CResource(unsigned int count, std::string name) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource((CResource *)0),
      mPhysicalResource((CResourceEnvelop *)0),
      mName(name) {
      Init(mName);
      Warn(mInitialCount, mBoundResource);
   }

   CResource(unsigned int count, std::string name, CResource *br) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource(br),
      mPhysicalResource((CResourceEnvelop *)0),
      mName(name) {
      Init(mName);
      Warn(mInitialCount, mBoundResource);
   }  

   CResource(unsigned int count, std::string name, CResourceEnvelop *re) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource((CResource*)0),
      mPhysicalResource(re),
      mName(name) {
      Init(mName);
      Warn(mInitialCount, mBoundResource);
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
   
      return (mIsAcquired = mSemCount.Wait(duration));
   }

   bool IsAcquired() {
      return mIsAcquired;
   }

private:

   Semaphore    mSemCount;
   unsigned int mInitialCount;
   unsigned int mCurrentIndex;

   // Used by logical resources
   // mBoundResource = 0 for physical
   // -------------------------------------------------------------------
   CResource   *mBoundResource;

   // Used by physical resources to track physical resource object indexes
   // -------------------------------------------------------------------
   CResourceEnvelop *mPhysicalResource;

   std::string  mName;

   bool         mIsAcquired;
   
   bool         mIsSink;
   bool         mIsSource;

   void Init(std::string &name) {
      // If we have a sink
      mIsSink = false;
      mIsSource = false;
      if (name == "sink") {
	 mIsSink = true;
      }
      else if (name == "source") {
	 mIsSource = true;
      }
   }

   void Warn(unsigned int initial_count, CResource *br) {
      // We've got a physical resource
      if (initial_count > 0) {
	 if ((CResource*)0 != br)  {
	    __log("Physical resource " << mName << " shouldn't have a bound resource"); 
	 }
      }
      // We've got a logical resource
      else {
	 if ((CResource *)0 == br) { 
	    if (!mIsSink) {
	       __log("Logical resource " << mName << " should have a bound resource");
	    }
	 }
      }
   }
};

}


#endif
