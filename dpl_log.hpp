#ifndef __DPL_LOG__
#define __DPL_LOG__

#include <iostream>

#define __log0 1

#if (__log0 == 1)
#define __log(x) std::cout << x << std::endl;
#else
#define __log(x)
#endif

#endif
