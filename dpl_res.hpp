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

class CResource;
class CSCB;

class CResourceEnvelop {
public:
   friend class CResource;

   CResourceEnvelop() {
   }
   virtual ~CResourceEnvelop() {
      __log("Deleting resource envelop");
   }

private:

   virtual void *CreateResource()        = 0;
   virtual void  DeleteResource(void *p) = 0;
   
   void Init(unsigned int number) {
      mPhysicalResources.resize(number);
      auto it = mPhysicalResources.begin();
      for (;it != mPhysicalResources.end();++it) {
	 *it = CreateResource();
      }
   }
   void Release() {
      std::cout << "Releasing physical resources" << std::endl;
      auto it = mPhysicalResources.begin();
      for (;it != mPhysicalResources.end();++it) {
	 if (*it != 0) {
	    DeleteResource(*it);
	 }
      }
   }
   
   std::vector<void *> mPhysicalResources;
};


class CResource {
public:
   friend class CSCB;

   // Used for "source" and "sink"
   // 
   CResource(unsigned int count, std::string name) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource((CResource *)0),
      mPhysicalResource((CResourceEnvelop *)0),
      mName(name) {
      // Init the resource
      Init(mName);
      
      // Call verifier
      Warn(mInitialCount, mBoundResource);
   }

   // Logical resource
   CResource(unsigned int count, std::string name, CResource *br) : 
      mSemCount(count),
      mInitialCount(br->mInitialCount),
      mCurrentIndex(0),
      mBoundResource(br),
      mPhysicalResource((CResourceEnvelop *)0),
      mName(name) {
      
      // Init the resource
      Init(mName);
      
      // Call verifier
      Warn(mInitialCount, mBoundResource);
   }   

   // Physical resource
   CResource(unsigned int count, std::string name, CResourceEnvelop *re) : 
      mSemCount(count),
      mInitialCount(count),
      mCurrentIndex(0),
      mBoundResource((CResource*)0),
      mPhysicalResource(re),
      mName(name) {
      // Init the resource
      Init(mName);

      // Create the physical resources
      re->Init(count);

      // Call verifier
      Warn(mInitialCount, mBoundResource);
   }

   ~CResource() {
      // Free the physical resources if needed
      if (mPhysicalResource) {
	 mPhysicalResource->Release();
      }
      __log("Deleting " << mName);
   }

   unsigned int GetIndex() {
      return mCurrentIndex;
   }

   void *GetData() {
      CResource *link = this;

      // While bound - go down to the physical 
      while (link->mBoundResource) {
	 link = link->mBoundResource;
      }

      unsigned int index = GetIndex();
      void *rval;

      // Protect against pure logical "source" and "sink"
      if (link->mPhysicalResource) {
	 rval = link->mPhysicalResource->mPhysicalResources[index];
      }
      else {
	 rval = 0;
      }
      return rval;
   }

private:

   // Prevent the use of the default constructor
   CResource() {}

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

   // Interface for the SCB
   // -------------------------------------------------------------------
   void UpdateIndex() {
      ++mCurrentIndex;
      if (mCurrentIndex == mInitialCount) {
	 mCurrentIndex = 0;
      }
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
   // ------------------------------------------------------------------

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
      if (mPhysicalResource) {

      }
      // We've got a logical resource
      else if (mBoundResource) {

      }
   }
};

template <class T>
class BUFFER_ENVELOP : public DDL::CResourceEnvelop {
public:
   // Inherit constructors
   using CResourceEnvelop::CResourceEnvelop;

   BUFFER_ENVELOP(unsigned int size) :
      DDL::CResourceEnvelop(),
      mSize(size) {}

   ~BUFFER_ENVELOP () {}

private:
   unsigned int mSize;

   void *CreateResource() {
      void *p = (void *) new T [mSize];
      std::cout << (unsigned long long) p << std::endl;
      return p;
   }
   void DeleteResource(void *p) {
      std::cout << (unsigned long long) p << std::endl;
      delete [] ((T *)p);
   }
};


}


#endif
