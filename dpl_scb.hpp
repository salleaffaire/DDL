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


#endif
