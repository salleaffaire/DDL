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

// Include DDL
#include "dpl.hpp"

#define IMAGE_WIDTH  640
#define IMAGE_HEIGHT 480
#define SLICE_HEIGHT 8

// SCB core functions

DDL_COREF(CFRead, res) {
   void *data0 = res[0]->GetData();
   std::cout << "CF Read" << std::endl;
}

DDL_COREF(CFLowPass, res) {
   void *data0 = res[0]->GetData();
   std::cout << "CF LP" << std::endl;
}

DDL_COREF(CFWrite, res) {
   void *data0 = res[0]->GetData();
   std::cout << "CF Write" << std::endl;
}

void DPL_TEST_LOWPASSIMAGE() {

   // Alloocate input memory
   unsigned char *image_data = new unsigned char [IMAGE_WIDTH*IMAGE_HEIGHT];
   
   // Read ther image from disk
   
   // Setup a sequencer graph

   // Create wrapper for GO_TOKEN physical allocator
   // -------------------------------------------------------------
   DDL::BUFFER_ENVELOP<unsigned char> 
      INPUT_SLICE_BUFFER(IMAGE_WIDTH * (SLICE_HEIGHT+2));
   DDL::BUFFER_ENVELOP<unsigned char> 
      OUTPUT_SLICE_BUFFER(IMAGE_WIDTH * SLICE_HEIGHT);

   // Create the Sequencer Controlled Blocks 
   // -------------------------------------------------------------
   DDL::CSCB scb_read(0x00, "ReadSlice");
   DDL::CSCB scb_lp(0x01, "LPSlice");
   DDL::CSCB scb_write(0x02, "WriteSlice");

   scb_read.Attach(CFRead);
   scb_lp.Attach(CFLowPass);
   scb_write.Attach(CFWrite);
      
   // Create the resources
   // -------------------------------------------------------------
   DDL::CResource go((IMAGE_HEIGHT/SLICE_HEIGHT), "source"); // Source
   DDL::CResource done(0, "sink");                           // Sink

   DDL::CResource input_buffer(5, "input_buffer", &INPUT_SLICE_BUFFER);
   DDL::CResource input_ready(0, "input_ready", &input_buffer);

   DDL::CResource output_buffer(4, "output_buffer", &OUTPUT_SLICE_BUFFER);
   DDL::CResource output_ready(0, "output_ready", &output_buffer);

   // Configure the process
   // --------------------------------------------------------------
   // [go]
   // [input_buffer]  -> scb_read ->  [input_ready]
   //
   // [input_ready] 
   // [output_buffer] -> scb_lp   ->  [output_ready]
   //                                 [input_buffer]
   //
   // [output_ready] -> scb_write ->  [output_buffer]
   //                                 [done]
   // --------------------------------------------------------------

   scb_read.WaitOn(&go);
   scb_read.WaitOn(&input_buffer);
   scb_read.Produce(&input_ready);

   scb_lp.WaitOn(&input_ready);
   scb_lp.WaitOn(&output_buffer);
   scb_lp.Produce(&output_ready);
   scb_lp.Produce(&input_buffer);

   scb_write.WaitOn(&output_ready);
   scb_write.Produce(&output_buffer);
   scb_write.Produce(&done);

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

   // Write the image to disk

   // Delete the memory
   delete [] image_data;
}
