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

#include <random>
#include <iostream>

// Include DDL
#include "dpl.hpp"

#define KEY_SIZE              128 // In bits
#define INPUT_KEY_BUFFER_SIZE 16  // In keys


void AES128(unsigned char *in, unsigned char *key, unsigned char *out) {
   // Does nothing 
}

void random_wait() {
   std::random_device rd;

   std::mt19937 gen(rd());
   std::uniform_int_distribution<> dis(1,10);

   // Wait for a random amount of time
   std::chrono::milliseconds duration(dis(gen)*10);
   std::this_thread::sleep_for(duration);
}

// SCB core functions

namespace AESTREE {
   DDL_COREF(CFRead, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF Read" << std::endl;
   }

   DDL_COREF(CFAES0, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF AES 0" << std::endl;
   }

   DDL_COREF(CFAES1, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF AES 1" << std::endl;
   }

   DDL_COREF(CFKEY0, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF KEY 0" << std::endl;
   }

   DDL_COREF(CFKEY1, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF KEY 1" << std::endl;
   }

   DDL_COREF(CFWrite, res) {
      void *data0 = res[0]->GetData();

      random_wait();

      std::cout << "CF Write" << std::endl;
   }
}

void DPL_TEST_AESTREE() {

   // Calculate key size in bytes
   unsigned int key_size_in_byte = KEY_SIZE >> 8;

   // Allocate input memory
   unsigned char *input_key_buffer = 
      new unsigned char [key_size_in_byte * INPUT_KEY_BUFFER_SIZE];
   
   // Just test data
   // -------------------------------------------------------------   
   
   // Setup a sequencer graph

   // Create wrapper for GO_TOKEN physical allocator
   // -------------------------------------------------------------
   DDL::BUFFER_ENVELOP<unsigned char> 
      INPUT_BUFFER(2*key_size_in_byte);

   DDL::BUFFER_ENVELOP<unsigned char> 
      KEY0_BUFFER(key_size_in_byte);
   DDL::BUFFER_ENVELOP<unsigned char>
      KEY1_BUFFER(key_size_in_byte);

   DDL::BUFFER_ENVELOP<unsigned char> 
      AES0_BUFFER(key_size_in_byte);
   DDL::BUFFER_ENVELOP<unsigned char>
      AES1_BUFFER(key_size_in_byte);

   // Create the Sequencer Controlled Blocks 
   // -------------------------------------------------------------
   DDL::CSCB scb_read(0x00, "Read");
   DDL::CSCB scb_aes0(0x01, "AES0");
   DDL::CSCB scb_aes1(0x02, "AES1");
   DDL::CSCB scb_key0(0x03, "KEY0");
   DDL::CSCB scb_key1(0x04, "KEY1");
   DDL::CSCB scb_write(0x05, "Write");

   scb_read.Attach(AESTREE::CFRead);
   scb_aes0.Attach(AESTREE::CFAES0);
   scb_aes1.Attach(AESTREE::CFAES1);
   scb_key0.Attach(AESTREE::CFKEY0);
   scb_key1.Attach(AESTREE::CFKEY1);
   scb_write.Attach(AESTREE::CFWrite);
      
   // Create the resources
   // -------------------------------------------------------------
   DDL::CResource go(64, "source");
   DDL::CResource done(0, "sink");

   DDL::CResource input_buffer(8, "input_buffer", &INPUT_BUFFER);
   DDL::CResource aes0_buffer(8, "aes0_buffer", &AES0_BUFFER);
   DDL::CResource aes1_buffer(8, "aes1_buffer", &AES1_BUFFER);
   DDL::CResource key0_buffer(8, "key0_buffer", &KEY0_BUFFER);
   DDL::CResource key1_buffer(8, "key1_buffer", &KEY1_BUFFER);
   
   DDL::CResource input_ready(0, "input_ready", &input_buffer);
   DDL::CResource aes0_ready(0, "aes0_ready", &aes0_buffer);
   DDL::CResource aes1_ready(0, "aes1_ready", &aes1_buffer);
   DDL::CResource key0_ready(0, "key0_ready", &key0_buffer);
   DDL::CResource key1_ready(0, "key1_ready", &key1_buffer);
 

   // Configure the process
   // --------------------------------------------------------------
   // --------------------------------------------------------------

   scb_read.WaitOn(&go);
   scb_read.WaitOn(&input_buffer);
   scb_read.Produce(&input_ready);

   scb_key0.WaitOn(&key0_buffer);
   scb_key0.Produce(&key0_ready);

   scb_key1.WaitOn(&key1_buffer);
   scb_key1.Produce(&key1_ready);

   scb_aes0.WaitOn(&input_ready);
   scb_aes0.WaitOn(&key0_ready);
   scb_aes0.WaitOn(&aes0_buffer);
   scb_aes0.Produce(&key0_buffer);
   scb_aes0.Produce(&aes0_ready);
   scb_aes0.Produce(&input_buffer);

   scb_aes1.WaitOn(&aes0_ready);
   scb_aes1.WaitOn(&key1_ready);
   scb_aes1.WaitOn(&aes1_buffer);
   scb_aes1.Produce(&key1_buffer);
   scb_aes1.Produce(&aes1_ready);
   scb_aes1.Produce(&aes0_buffer);

   scb_write.WaitOn(&aes1_ready);
   scb_write.Produce(&aes1_buffer);
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

}
