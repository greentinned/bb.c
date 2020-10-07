#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define check_bounds_error(index)                                              \
  fprintf(stderr, "Error: Index %d is out of bounds\n", (index));              \
  exit(EXIT_FAILURE)

struct Cell {
  int data;
  struct Cell *next;
  struct Cell *prev;
};

struct Cell *cell_new(int elem) {
  struct Cell *result = malloc(sizeof(struct Cell));
  *result = (struct Cell){elem, 0, 0};
  return result;
}

struct Cell *cell_get_first(struct Cell *cell) {
  struct Cell *result = cell;

  while (result->prev) {
    result = result->prev;
  }

  return result;
}

struct Cell *cell_get_last(struct Cell *cell) {
  struct Cell *result = cell;

  while (result->next) {
    result = result->next;
  }

  return result;
}

struct Cell *cell_get_at(int index, struct Cell *cell) {
  // Left bounds: index is less than zero
  if (index < 0) {
    check_bounds_error(index);
  }

  int count = 0;
  struct Cell *result = cell_get_first(cell);

  while (count < index) {
    // Right bounds:
    // We do not have next value, but still indexing
    if (result->next == NULL && count < index) {
      check_bounds_error(index);
    }

    result = result->next;
    ++count;
  }

  return result;
}

void cell_push_front(int elem, struct Cell **cell) {
  struct Cell *first_elem = cell_get_first(*cell);
  struct Cell *new_elem = cell_new(elem);
  new_elem->next = first_elem;
  first_elem->prev = new_elem;
  *cell = new_elem;
}

void cell_push_back(int elem, struct Cell **cell) {
  struct Cell *last_elem = cell_get_last(*cell);
  struct Cell *new_elem = cell_new(elem);
  new_elem->prev = last_elem;
  last_elem->next = new_elem;
}

void cell_delete_at(int index, struct Cell **cell) {
  struct Cell *elem = cell_get_at(index, *cell);

  if (elem->prev) {
    elem->prev->next = elem->next;
  }

  if (elem->next) {
    elem->next->prev = elem->prev;
  }

  if (index == 0 && elem == *cell) {
    *cell = elem->next;
  }

  free(elem);
  elem = NULL;
}

void cell_free_all(struct Cell *cell) {
  struct Cell *prev_elem = cell_get_last(cell);

  do {
    prev_elem = prev_elem->prev;
    free(prev_elem->next);
    prev_elem->next = NULL;
  } while (prev_elem->prev);

  free(prev_elem);
  prev_elem = NULL;
  cell = NULL;
}

int main(void) {
  struct Cell *cell = cell_new(1);

  cell_push_back(2, &cell);
  cell_push_back(3, &cell);
  cell_push_back(4, &cell);
  cell_push_front(0, &cell);

  cell_delete_at(0, &cell);
  cell_delete_at(3, &cell);
  cell_delete_at(1, &cell);

  int a = cell_get_at(0, cell)->data;
  printf("a: %d\n", a);

  int b = cell_get_at(1, cell)->data;
  printf("b: %d\n", b);

  cell_free_all(cell);

  return 0;
}
