#ifndef DPL_SCB_HPP___
#define DPL_SCB_HPP___

#include <list>
#include <thread>

// Forward declarations
class CResource;
class CSequencer;
class CSCB;

class CSCB {
public:
   CSCB();

   CSCB(unsigned int id);
 
   virtual ~CSCB();
   
   int Start();

   int StopAndWait();

   void WaitOn(CResource *res);
   
   void Produce(CResource *res);

   static CSequencer *GetSequencer();

private:
   // SCB Identifier
   unsigned int mId;

   // SCB State
   volatile unsigned int mIsRunning;
   
   // Thread structures
   std::thread *mpT;

   // When start
   std::list<CResource *> mDecrement;
   // When done
   std::list<CResource *> mIncrement;

   // Pointer to a Sequencer object
   static CSequencer *mSequencer;

   int Wait();

   virtual void Run() = 0;

   static void EventLoop(CSCB *arg);

   void Init();

   void Clean();

};


#endif
