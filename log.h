#include <stdio.h>
#include <stdlib.h>

#define LOGI(fmt, ...) fprintf(stdout, "I::" fmt "\n", ##__VA_ARGS__)
#define LOGE(fmt, ...) fprintf(stderr, "E::" fmt "\n", ##__VA_ARGS__)

#ifdef NDEBUG
#define LOGD(fmt, ...)
#else
#define LOGD(fmt, ...) fprintf(stdout, "I::" fmt "\n", ##__VA_ARGS__)
#endif
