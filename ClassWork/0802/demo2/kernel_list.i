# 0 "kernel_list.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "kernel_list.c"
# 11 "kernel_list.c"
# 1 "kernel_list.h" 1
# 192 "kernel_list.h"
struct list_head {
  struct list_head *prev;
  struct list_head *next;
};
# 208 "kernel_list.h"
static inline void __list_add(struct list_head *new_node,
                              struct list_head *prev, struct list_head *next) {
  next->prev = new_node;
  new_node->next = next;
  new_node->prev = prev;
  prev->next = new_node;
}
# 225 "kernel_list.h"
static inline void list_add(struct list_head *new_node,
                            struct list_head *head) {
  __list_add(new_node, head, head->next);
}
# 239 "kernel_list.h"
static inline void list_add_tail(struct list_head *new_node,
                                 struct list_head *head) {
  __list_add(new_node, head->prev, head);
}
# 253 "kernel_list.h"
static inline void __list_del(struct list_head *prev, struct list_head *next) {
  next->prev = prev;
  prev->next = next;
}
# 266 "kernel_list.h"
static inline void list_del(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
  entry->next = ((struct list_head *)0x00100100);
  entry->prev = ((struct list_head *)0x00200);
}
# 280 "kernel_list.h"
static inline void list_del_init(struct list_head *entry) {
  __list_del(entry->prev, entry->next);
  (entry)->next = (entry); (entry)->prev = (entry);
}
# 293 "kernel_list.h"
static inline int list_empty(const struct list_head *head) {
  return head->next == head;
}
# 306 "kernel_list.h"
static inline void list_move(struct list_head *list, struct list_head *head) {
  __list_del(list->prev, list->next);
  list_add(list, head);
}
# 320 "kernel_list.h"
static inline void list_move_tail(struct list_head *list,
                                  struct list_head *head) {
  __list_del(list->prev, list->next);
  list_add_tail(list, head);
}
# 12 "kernel_list.c" 2

int main(int argc, const char *argv[]) {

  struct list_head head = { &(head), &(head) }; (&head)->next = (&head); (&head)->prev = (&head);


  struct list_head node0 = { &(node0), &(node0) }; (&node0)->next = (&node0); (&node0)->prev = (&node0);
  struct list_head node1 = { &(node1), &(node1) }; (&node1)->next = (&node1); (&node1)->prev = (&node1);
  struct list_head node2 = { &(node2), &(node2) }; (&node2)->next = (&node2); (&node2)->prev = (&node2);
  struct list_head node3 = { &(node3), &(node3) }; (&node3)->next = (&node3); (&node3)->prev = (&node3);
  struct list_head node4 = { &(node4), &(node4) }; (&node4)->next = (&node4); (&node4)->prev = (&node4);
  struct list_head node5 = { &(node5), &(node5) }; (&node5)->next = (&node5); (&node5)->prev = (&node5);
  struct list_head node6 = { &(node6), &(node6) }; (&node6)->next = (&node6); (&node6)->prev = (&node6);
  struct list_head node7 = { &(node7), &(node7) }; (&node7)->next = (&node7); (&node7)->prev = (&node7);
  struct list_head node8 = { &(node8), &(node8) }; (&node8)->next = (&node8); (&node8)->prev = (&node8);
  struct list_head node9 = { &(node9), &(node9) }; (&node9)->next = (&node9); (&node9)->prev = (&node9);
  struct list_head node10 = { &(node10), &(node10) }; (&node10)->next = (&node10); (&node10)->prev = (&node10);


  list_add(&node0, &head);
  list_add(&node1, &head);
  list_add(&node2, &head);
  list_add(&node3, &head);
  list_add(&node4, &head);
  list_add(&node5, &head);
  list_add(&node6, &head);
  list_add(&node7, &head);
  list_add(&node8, &head);
  list_add(&node9, &head);
  list_add(&node10, &head);

  struct list_head *pos;
  for (pos = (&head)->next; pos != (&head); pos = pos->next) {}

  return 0;
}
