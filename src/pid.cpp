#include "pid.h"

wap_pid *wap_pid_create() { return new (std::nothrow) wap_pid; }

void wap_pid_free(wap_pid *pid) { delete pid; }
