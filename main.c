#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define panic_abort(fmt, ...)                                                  \
  fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__,            \
          __VA_ARGS__);                                                        \
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
    panic_abort("Index %d out of bounds\n", index);
  }

  int count = 0;
  struct Cell *result = cell_get_first(cell);

  while (count < index) {
    // Right bounds:
    // We do not have next value, but still indexing
    if (result->next == NULL && count < index) {
      panic_abort("Index %d out of bounds\n", index);
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
  struct Cell *cell = cell_new(0);

  for (int i = 1; i < 10000; ++i) {
    if (i % 2 == 0) {
      cell_push_back(i, &cell);
    } else {
      cell_push_front(i, &cell);
    }
  }

  for (int i = 1; i < 1000; ++i) {
    if (i % 2 == 0) {
      cell_delete_at(i, &cell);
    }
  }

  struct Cell *first_cell = cell_get_first(cell);
  while ((first_cell = first_cell->next)) {
    printf("data: %d\n", first_cell->data);
  }

  cell_free_all(cell);

  return 0;
}
