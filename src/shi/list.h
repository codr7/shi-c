#ifndef SHI_LIST
#define SHI_LIST

#include <stdbool.h>
#include "utility.h"

#define _sh_list_do(l, i, _list, _next)				\
  __auto_type _list = l;					\
  for (struct sh_list *i = _list->next, *_next = i->next;	\
       i != _list;						\
       i = _next, _next = i->next)

#define sh_list_do(l, i)				\
  _sh_list_do(l, i, sh_unique(list), sh_unique(next))

struct sh_list {
  struct sh_list *prev, *next;
};

void sh_list_init(struct sh_list *l);
bool sh_list_nil(const struct sh_list *l);
size_t sh_list_length(struct sh_list *l);
struct sh_list *sh_list_delete(struct sh_list *l);

void sh_list_push_front(struct sh_list *l, struct sh_list *it);
struct sh_list *sh_list_pop_front(struct sh_list *l);
struct sh_list *sh_list_peek_front(struct sh_list *l);

void sh_list_push_back(struct sh_list *l, struct sh_list *it);
struct sh_list *sh_list_pop_back(struct sh_list *l);
struct sh_list *sh_list_peek_back(struct sh_list *l);
void sh_list_shift_back(struct sh_list *l);

#endif
