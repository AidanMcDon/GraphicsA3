// The standard headers included by all files


#ifndef HEADERS_H
#define HEADERS_H

#include "glad/include/glad/glad.h"
#include <GLFW/glfw3.h>

#ifdef LINUX
  #include <unistd.h>		// includes usleep (to sleep for some time)
  #include <float.h>           // includes FLT_MAX
  #include <sys/time.h>	        // includes gettimeofday()
  #define sprintf_s sprintf
  #define _strdup strdup
  #define sscanf_s sscanf
  #define _getcwd getcwd
#endif

#ifdef _WIN32
  #include <direct.h>
  #include <sys/timeb.h>
  #define PATH_MAX 1000
  #pragma warning(disable : 4244 4305 4996 4838 4293)
#endif

#ifdef __APPLE_CC__
  #include <unistd.h>
  #include <float.h>
  #include <sys/time.h>
  #define sprintf_s sprintf
  #define _strdup strdup
  #define sscanf_s sscanf
  #define _getcwd getcwd
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
using namespace std;

#include <cmath>

#include "linalg.h"

#define randIn01() (rand() / (float) RAND_MAX)   // random number in [0,1]

#endif
