#include "common/profiler/profiler.h"

#include <string>

#ifdef PROFILER_AVAILABLE

#include "gperftools/heap-profiler.h"
#include "gperftools/profiler.h"

namespace Envoy {
namespace Profiler {

bool Cpu::profilerEnabled() { return ProfilingIsEnabledForAllThreads(); }

bool Cpu::startProfiler(const std::string& output_path) {
  return ProfilerStart(output_path.c_str());
}

void Cpu::stopProfiler() { ProfilerStop(); }

bool Heap::profilerEnabled() {
  // determined by PROFILER_AVAILABLE
  return true;
}

bool Heap::isProfilerStarted() { return IsHeapProfilerRunning(); }
bool Heap::startProfiler(const std::string& output_file_name_prefix) {
  HeapProfilerStart(output_file_name_prefix.c_str());
  return true;
}

bool Heap::stopProfiler() {
  if (!IsHeapProfilerRunning()) {
    return false;
  }
  HeapProfilerDump("stop and dump");
  HeapProfilerStop();
  return true;
}

void Heap::forceLink() {
  // Currently this is here to force the inclusion of the heap profiler during static linking.
  // Without this call the heap profiler will not be included and cannot be started via env
  // variable. In the future we can add admin support.
  HeapProfilerDump("");
}

} // namespace Profiler
} // namespace Envoy

#else

namespace Envoy {
namespace Profiler {

bool Cpu::profilerEnabled() { return false; }
bool Cpu::startProfiler(const std::string&) { return false; }
void Cpu::stopProfiler() {}

bool Heap::profilerEnabled() { return false; }
bool Heap::isProfilerStarted() { return false; }
bool Heap::startProfiler(const std::string&) { return false; }
bool Heap::stopProfiler() { return false; }
} // namespace Profiler
} // namespace Envoy

#endif // #ifdef TCMALLOC
