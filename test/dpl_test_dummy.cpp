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

#include <iostream>

#include "dpl.hpp"

// Core functions 
// ---------------------------------------------------------------
DDL_COREF(RunGo, res) {
   std::chrono::milliseconds duration(100);
   std::this_thread::sleep_for(duration);
   
   void *data0 = res[0]->GetData();
   void *data1 = res[1]->GetData();

   std::cout << "Go Index = " << res[0]->GetIndex() 
	     << " Data = " << (long long) data0 << std::endl;
   std::cout << "Go Index = " << res[1]->GetIndex() 
	     << " Data = " << (long long) data1 << std::endl;  
 
   __log("Go ran once with Res of size " << res.size());
}

DDL_COREF(RunDone, res) {
   std::chrono::milliseconds duration(200);
   std::this_thread::sleep_for(duration);

   void *data0 = res[0]->GetData();

   std::cout << "Done Index = " << res[0]->GetIndex() 
	     << " Data = " << (long long) data0 << std::endl;


   __log("Done ran once with Res of size " << res.size());
}
// ---------------------------------------------------------------

void DPL_TEST_DUMMY() {

   // Create wrapper for GO_TOKEN physical allocator
   // -------------------------------------------------------------
   DDL::BUFFER_ENVELOP<unsigned char> GO_TOKEN_BUFFER(1024); 

   // Create the Sequencer Controlled Blocks 
   // -------------------------------------------------------------
   DDL::CSCB scb_go(0x00, "Go");
   DDL::CSCB scb_done(0x01, "Done");

   scb_go.Attach(RunGo);
   scb_done.Attach(RunDone);
      
   // Create the resources
   // -------------------------------------------------------------
   DDL::CResource go(8, "source");
   DDL::CResource done(0, "sink");

   DDL::CResource go_token(8, "go_token", &GO_TOKEN_BUFFER);
   DDL::CResource done_token(0, "done_token", &go_token);

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
   // -------------------------------------------------------------
   DDL::CSequencer *seq = DDL::CSCB::GetSequencer();

   if (seq) seq->Start();

   #if 1
   char a = 't';
   while (a != 'f') {
      std::cin >> a;
   }
   #else
   std::chrono::milliseconds duration(2000);
   std::this_thread::sleep_for(duration);
   #endif

   if (seq) seq->StopAndWait();
}
