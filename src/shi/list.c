#include <stddef.h>
#include "list.h"

void sh_list_init(struct sh_list *l) {
  l->prev = l->next = l;
}

bool sh_list_nil(const struct sh_list *l) {
  return l->prev == l && l->next == l;
}

struct sh_list *sh_list_delete(struct sh_list *l) {
  l->prev->next = l->next;
  l->next->prev = l->prev;
  return l;
}

void sh_list_push_front(struct sh_list *l, struct sh_list *it) {
  sh_list_push_back(l->next, it);
}

struct sh_list *sh_list_pop_front(struct sh_list *l) {
  struct sh_list *it = l->next;
  return (it == l) ? NULL : sh_list_delete(it);
}

struct sh_list *sh_list_peek_front(struct sh_list *l) {
  struct sh_list *it = l->next;
  return (it == l) ? NULL : it;
}

void sh_list_push_back(struct sh_list *l, struct sh_list *it) {
  it->prev = l->prev;
  l->prev->next = it;
  it->next = l;
  l->prev = it;
}

struct sh_list *sh_list_pop_back(struct sh_list *l) {
  struct sh_list *it = l->prev;
  return (it == l) ? NULL : sh_list_delete(it);
}

struct sh_list *sh_list_peek_back(struct sh_list *l) {
  struct sh_list *it = l->prev;
  return (it == l) ? NULL : it;
}

void sh_list_shift_back(struct sh_list *l) {
  l->next->prev = l->prev;
  l->prev->next = l->next;
  l->prev = l->next;
  l->next = l->next->next;
  l->prev->next = l;
  l->next->prev = l;
}
