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

#ifndef LM_SEMAPHORE_HPP___
#define LM_SEMAPHORE_HPP___

#include <mutex>               // std::mutex, std::unique_lock
#include <condition_variable>  // std::condition_variable
#include <chrono>
 
class Semaphore
{   
public:
   inline Semaphore()
      : mCount(0) { }

   inline Semaphore(unsigned int count)
      : mCount(count) { }
   
   inline void Wait()
   {
      std::unique_lock<std::mutex> lock(mMutex);
      mCondition.wait(lock, [&]()->bool{ return mCount>0; });
      --mCount;
   }
   
   template< typename R,typename P >
   bool Wait(const std::chrono::duration<R,P>& crRelTime)
   {
      std::unique_lock<std::mutex> lock(mMutex);
      if (!mCondition.wait_for(lock, crRelTime, [&]()->bool{ return mCount>0; }))
	 return false;
      --mCount;
      return true;
   }
   
   inline void Signal()
   {
      std::unique_lock<std::mutex> lock(mMutex);
      ++mCount;
      mCondition.notify_one();
   }

private:
   unsigned int            mCount;
   std::mutex              mMutex;
   std::condition_variable mCondition;
};

#endif



