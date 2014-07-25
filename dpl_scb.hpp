#ifndef DPL_SCB_HPP___
#define DPL_SCB_HPP___

#include <list>
#include <thread>
#include <string>

// Forward declarations
class CResource;
class CSequencer;
class CSCB;

class CSCB {
public:
   typedef void (*run_t)(std::list<CResource *> &);

   CSCB();

   CSCB(unsigned int id);
 
   CSCB(unsigned int id, std::string name); 

   virtual ~CSCB();
   
   int Start();

   int StopAndWait();

   void WaitOn(CResource *res);
   
   void Produce(CResource *res);

   static CSequencer *GetSequencer();

   void Attach(run_t f);

private:
   // SCB Identifier
   unsigned int mId;

   run_t Run;

   std::string mName;

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

   static void EventLoop(CSCB *arg);

   void Init();

   void Clean();

};


#endif
