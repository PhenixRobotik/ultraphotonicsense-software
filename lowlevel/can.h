#pragma once

#include <stdbool.h>
#include <stdint.h>

void can_setup();
// TODO !
void* can_get_message();
void can_send(void* msg);
