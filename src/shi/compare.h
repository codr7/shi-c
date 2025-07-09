#ifndef SHI_COMPARE
#define SHI_COMPARE

#define sh_compare(x, y) ({				\
      __auto_type _x = x;				\
      __auto_type _y = y;				\
      (_x < _y) ? SH_LT : ((_x > _y) ? SH_GT : SH_EQ);	\
    })

enum sh_order {SH_LT = -1, SH_EQ = 0, SH_GT = 1};

typedef enum sh_order (*sh_compare_t)(const void *, const void *);

#endif
