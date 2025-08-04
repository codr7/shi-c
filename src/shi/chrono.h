#ifndef SHI_CHRONO
#define SHI_CHRONO

#include <stdint.h>
#include <time.h>

#define SH_TIME_FORMAT "%Y-%m-%dT%H:%M:%S"

struct sh_stream;

struct sh_time {
  struct timespec value;
};

typedef struct sh_time sh_time_t;

sh_time_t sh_now();

sh_time_t sh_time(int year,
		  int month,
		  int day,
		  int hour,
		  int minute,
		  int second);

uint64_t sh_time_ns(const sh_time_t *t);
void sh_time_print(const sh_time_t *t, const char *m);
char *sh_time_sprintf(const sh_time_t *t, const char *spec);

void sh_time_printf(const sh_time_t *t,
		    const char *spec,
		    struct sh_stream *out);

uint64_t sh_sleep(uint64_t ns);

#endif
