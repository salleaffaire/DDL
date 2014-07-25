#ifndef DPL_RES_HPP___
#define DPL_RES_HPP___

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
