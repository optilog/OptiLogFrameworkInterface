#include "internal.hpp"

/*------------------------------------------------------------------------*/

// This is pretty Linux specific code for reporting resource usage.
// TODO: port these functions to different OS.

extern "C" {
#ifndef _MSC_VER
#include <sys/time.h>
#include <sys/resource.h>
#else
#include <time.h>
#include <windows.h>
#include <psapi.h>
#endif

#include <sys/types.h>
#include <unistd.h>
#include <string.h>
}

namespace CaDiCaL {

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN

#include <stdint.h> // portable: uint64_t   MSVC: __int64

// MSVC defines this in winsock2.h!?
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}
#endif

double absolute_real_time () {
  struct timeval tv;
  if (gettimeofday (&tv, 0)) return 0;
  return 1e-6 * tv.tv_usec + tv.tv_sec;
}

double Internal::real_time () {
  return absolute_real_time () - stats.time.real;
}

/*------------------------------------------------------------------------*/

// We use 'getrusage' for 'process_time' and 'maximum_resident_set_size'
// which is pretty standard on Unix but probably not available on Windows
// etc.  For different variants of Unix not all fields are meaningful.

double absolute_process_time () {
#ifndef _MSC_VER
  struct rusage u;
  double res;
  if (getrusage (RUSAGE_SELF, &u)) return 0;
  res = u.ru_utime.tv_sec + 1e-6 * u.ru_utime.tv_usec;  // user time
  res += u.ru_stime.tv_sec + 1e-6 * u.ru_stime.tv_usec; // + system time
  return res;
#else
HANDLE hProcess;
FILETIME creationTime;
FILETIME exitTime;
FILETIME kernelTime;
FILETIME userTime;
ULARGE_INTEGER wrkTime;
hProcess=GetCurrentProcess();
if (GetProcessTimes(hProcess,  &creationTime, &exitTime, &kernelTime, &userTime)) {
    wrkTime.LowPart = userTime.dwLowDateTime;
    wrkTime.HighPart = userTime.dwHighDateTime;
    return wrkTime.QuadPart / 10000000.0;
    }
return 0;
#endif
}

double Internal::process_time () {
  return absolute_process_time () - stats.time.process;
}

/*------------------------------------------------------------------------*/

// This seems to work on Linux (man page says since Linux 2.6.32).

size_t maximum_resident_set_size () {
#ifndef _MSC_VER
  struct rusage u;
  if (getrusage (RUSAGE_SELF, &u)) return 0;
  return ((size_t) u.ru_maxrss) << 10;
#else
HANDLE hProcess;
PROCESS_MEMORY_COUNTERS processMemoryCounters;

hProcess=GetCurrentProcess();
if (GetProcessMemoryInfo(hProcess,  &processMemoryCounters, sizeof(processMemoryCounters))) {
    return processMemoryCounters.PeakWorkingSetSize;
    }
return 0;
#endif
}

// Unfortunately 'getrusage' on Linux does not support current resident set
// size (the field 'ru_ixrss' is there but according to the man page
// 'unused'). Thus we fall back to use the '/proc' file system instead.  So
// this is not portable at all and needs to be replaced on other systems
// The code would still compile though (assuming 'sysconf' and
// '_SC_PAGESIZE' are available).

size_t current_resident_set_size () {
#ifndef _MSC_VER
  char path[40];
  sprintf (path, "/proc/%" PRId64 "/statm", (int64_t) getpid ());
  FILE * file = fopen (path, "r");
  if (!file) return 0;
  int64_t dummy, rss;
  int scanned = fscanf (file, "%" PRId64 " %" PRId64 "", &dummy, &rss);
  fclose (file);
  return scanned == 2 ? rss * sysconf (_SC_PAGESIZE) : 0;
#else
HANDLE hProcess;
PROCESS_MEMORY_COUNTERS processMemoryCounters;

hProcess=GetCurrentProcess();
if (GetProcessMemoryInfo(hProcess,  &processMemoryCounters, sizeof(processMemoryCounters))) {
    return processMemoryCounters.WorkingSetSize;
    }
return 0;
#endif
}

}
