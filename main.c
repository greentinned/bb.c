#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Tape {
  int count;
  struct Cell *first;
  struct Cell *last;
};

struct Cell {
  int data;
  struct Cell *next;
  struct Cell *prev;
};

struct Cell *tape_new(int elem) {
  struct Cell *result = malloc(sizeof(struct Cell));
  *result = (struct Cell){elem, 0, 0};
  return result;
}

/* struct Tape *tape_new(int elem) { */
/*   struct Tape *result = malloc(sizeof(struct Tape)); */
/*   struct Cell *cell = malloc(sizeof(struct Cell)); */

/*   *cell = (struct Cell){elem, 0, 0}; */
/*   *result = (struct Tape){1, cell, 0}; */

/*   return result; */
/* } */

struct Cell *tape_get_first(struct Cell *tape) {
  struct Cell *result = tape;

  while (result->prev) {
    result = result->prev;
  }

  return result;
}

struct Cell *tape_get_last(struct Cell *tape) {
  struct Cell *result = tape;

  while (result->next) {
    result = result->next;
  }

  return result;
}

struct Cell *tape_get_at(int index, struct Cell *tape) {
  int count = 0;
  struct Cell *result = tape_get_first(tape);

  while (count < index && result->next) {
    result = result->next;
    ++count;
  }

  return result;
}

void tape_push_front(int elem, struct Cell **tape) {
  struct Cell *first_elem = tape_get_first(*tape);
  struct Cell *new_elem = tape_new(elem);
  new_elem->next = first_elem;
  first_elem->prev = new_elem;
  *tape = new_elem;
}

void tape_push_back(int elem, struct Cell **tape) {
  struct Cell *last_elem = tape_get_last(*tape);
  struct Cell *new_elem = tape_new(elem);
  new_elem->prev = last_elem;
  last_elem->next = new_elem;
}

void tape_delete_at(int index, struct Cell **tape) {
  struct Cell *elem = tape_get_at(index, *tape);
  // TODO: check if about to remove head element
  // and then change the pointer too
  if (elem->prev) {
    elem->prev->next = elem->next;
  }

  if (elem->next) {
    elem->next->prev = elem->prev;
  }

  if (index == 0 && elem == *tape) {
    *tape = elem->next;
  }

  free(elem);
  elem = NULL;
}

void tape_free(struct Cell *tape) {
  struct Cell *prev_elem = tape_get_last(tape);

  do {
    prev_elem = prev_elem->prev;
    free(prev_elem->next);
    prev_elem->next = NULL;
  } while (prev_elem->prev);

  free(prev_elem);
  prev_elem = NULL;
  tape = NULL;
}

int main(void) {
  struct Cell *tape = tape_new(1);

  tape_push_back(2, &tape);
  tape_push_back(3, &tape);
  tape_push_back(4, &tape);
  tape_push_front(0, &tape);

  tape_delete_at(0, &tape);
  tape_delete_at(3, &tape);
  tape_delete_at(1, &tape);

  int a = tape_get_at(0, tape)->data;
  printf("a: %d\n", a);

  int b = tape_get_at(4, tape)->data;
  printf("b: %d\n", b);

  tape_free(tape);

  return 0;
}
