#pragma once

// Return size of an array, use with caution (pointer decay)
#define ARR_SIZE(a) (sizeof(a) / sizeof(0 [a]))
