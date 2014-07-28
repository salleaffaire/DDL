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

#include <chrono>
#include <thread>
#include <vector>

#include "semaphore.hpp"
#include "dpl_seq.hpp"
#include "dpl_res.hpp"
#include "dpl_scb_dummy.hpp"
#include "dpl_scb_template.hpp"
#include "dpl_log.hpp"

void RunGo(std::list<CResource *> &res) {
   std::chrono::milliseconds duration(100);
   std::this_thread::sleep_for(duration);
   
   __log("Go ran once with Res of size " << res.size());
}

void RunDone(std::list<CResource *> &res) {
   std::chrono::milliseconds duration(200);
   std::this_thread::sleep_for(duration);
   
   __log("Done ran once with Res of size " << res.size());
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
   CSCB scb_go(0x00, "Go");
   CSCB scb_done(0x01, "Done");

   scb_go.Attach(RunGo);
   scb_done.Attach(RunDone);
      
   // Create the resources
   // -------------------------------------------------------------
   CResource go(16, "go");
   CResource done(0, "done");
   CResource go_token(1, "go_token");
   CResource done_token(0, "done_token");

   // Configure the process
   // --------------------------------------
   // [go]
   // [go_token]   -> scb_go   -> [done_token]
   // [done_token] -> scb_done -> [go_token]
   //                             [done]
   // --------------------------------------
   scb_go.WaitOn(&go);
   scb_go.WaitOn(&go_token);
   scb_go.Produce(&done_token);

   scb_done.WaitOn(&done_token);
   scb_done.Produce(&go_token);
   scb_done.Produce(&done);

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
