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
#include <list>

#include "test/dpl_test.hpp"


int 
main(int argc, char *argv[])
{
   // Test Sequencer
   // -------------------------------------------------------------
   // Dummy Test
#if 0
   DPL_TEST_DUMMY();

#endif

   // Low Pass Image
#if 0
   DPL_TEST_LOWPASSIMAGE();

#endif
   
   // AES Tree
#if 1
   DPL_TEST_AESTREE();

#endif
   return 0;
}
