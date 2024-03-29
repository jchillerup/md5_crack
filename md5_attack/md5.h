#include <stdint.h>

#define TRUE 1
#define FALSE 0

extern const uint32_t h0;
extern const uint32_t h1;
extern const uint32_t h2;
extern const uint32_t h3;
extern const int m_idx[];

#ifndef MD5_STATE_STRUCT
typedef struct  {
	uint32_t a;
	uint32_t b;
	uint32_t c;
	uint32_t d;
} md5_state;
#define MD5_STATE_STRUCT
#endif

#ifndef MD5_REDUCED_STATE_STRUCT
typedef struct  {
	uint32_t b;
	uint32_t c;
} md5_state_reduced;
#define MD5_REDUCED_STATE_STRUCT
#endif

#define MAYBE_INLINE 

void      md5_round          (md5_state *state_ptr, uint32_t* m, int r);
void      md5_round_backwards(md5_state *state_ptr, uint32_t* m, int r);
md5_state md5                (char * input);
void      md5_truncated      (md5_state* state_ptr, uint32_t * m, int stop_after_round);