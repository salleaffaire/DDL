#include "semaphore.hpp"
#include "dpl_seq.hpp"
#include "dpl_res.hpp"
#include "dpl_scb_dummy.hpp"
#include "dpl_scb_template.hpp"

void f(int *a, int *b) {

}

int 
main(int argc, char *argv[])
{
   // Test Semaphore
#if 1
   Semaphore sem(1); 
#endif

   // Test Sequencer
   // -------------------------------------------------------------
#if 1

   // Create the Sequencer Controlled Blocks 
   CSCB_Dummy scb0(0xAAAAAAAA, 100, "Go");
   CSCB_Dummy scb1(0xBBBBBBBB, 200, "Done");

   //CSCB_Template<int, int> temp1(0x11111111,
   //                              "Template 1",
   //                              [](int *, int *)->void {});

   // Create the resources
   // -------------------------------------------------------------
   CResource go(16, "go");
   CResource done(0, "done");
   CResource go_token(1, "go_token");
   CResource done_token(0, "done_token");

   // Configure the process
   // --------------------------------------
   // [go]
   // [go_token]   -> scb0 -> [done_token]
   // [done_token] -> scb1 -> [go_token]
   //                         [done]
   // --------------------------------------
   scb0.WaitOn(&go);
   scb0.WaitOn(&go_token);
   scb0.Produce(&done_token);

   scb1.WaitOn(&done_token);
   scb1.Produce(&go_token);
   scb1.Produce(&done);

   // Get access to the sequencer object
   CSequencer *seq = CSCB::GetSequencer();

   if (seq) seq->Start();

   char a = 't';
   while (a != 'f') {
      std::cin >> a;
   }

   if (seq) seq->StopAndWait();

#endif

   return 0;
}
