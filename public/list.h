/******************************************************************************
Copyright (c) HrSet Tech. Co., All rights reserved.
-------------------------------------------------------------------------------
Description: 
-------------------------------------------------------------------------------
Version : V1.00.01
Modification Hisitory:
Date:          Author:            Description:
-------------------------------------------------------------------------------
2013-04-09

******************************************************************************/

#ifndef ILST_H
#define ILST_H

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/********************************************
*return the ptr of the data , by the ptr of it's member
    param:  prt: the prt of member
                type: the type of data
                member: the member in the data of type
********************************************/
#define container_of(ptr, type, member) ({			\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
        (type *)( (char *)__mptr - offsetof(type,member) );})


/*double direction list element, include netx and prev element ptr*/
struct list_head {
	struct list_head *next, *prev;
};


#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/*add new element between prev and next elements*/
static inline void __list_add(struct list_head *new_node,
			      struct list_head *prev,
			      struct list_head *next)
{
	next->prev = new_node;
	new_node->next = next;
	new_node->prev = prev;
	prev->next = new_node;
}

/*add new element after the head*/
static inline void list_add(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head, head->next);
}


/*add new element before the tail*/
static inline void list_add_tail(struct list_head *new_node, struct list_head *head)
{
	__list_add(new_node, head->prev, head);
}



/*delete element between prev and next elements*/
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}


/*delete the entry element*/
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}


static inline void list_replace(struct list_head *old,
				struct list_head *new_node)
{
	new_node->next = old->next;
	new_node->next->prev = new_node;
	new_node->prev = old->prev;
	new_node->prev->next = new_node;
}

static inline void list_replace_init(struct list_head *old,
					struct list_head *new_node)
{
	list_replace(old, new_node);
	INIT_LIST_HEAD(old);
}



static inline void list_del_init(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	INIT_LIST_HEAD(entry);
}


static inline void list_move(struct list_head *list, struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add(list, head);
}


static inline void list_move_tail(struct list_head *list,
				  struct list_head *head)
{
        __list_del(list->prev, list->next);
        list_add_tail(list, head);
}



static inline struct list_head *list_get_tail(struct list_head *head)
{
        return head->prev;
}



static inline int list_is_last(const struct list_head *list,
				const struct list_head *head)
{
	return list->next == head;
}


/*if the list is empty, return ture, else false*/
static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}


static inline int list_empty_careful(const struct list_head *head)
{
	struct list_head *next = head->next;
	return (next == head) && (next == head->prev);
}

static inline void __list_splice(struct list_head *list,
				 struct list_head *head)
{
	struct list_head *first = list->next;
	struct list_head *last = list->prev;
	struct list_head *at = head->next;

	first->prev = head;
	head->next = first;

	last->next = at;
	at->prev = last;
}



static inline void list_splice(struct list_head *list, struct list_head *head)
{
	if (!list_empty(list))
		__list_splice(list, head);
}



static inline void list_splice_init(struct list_head *list,
				    struct list_head *head)
{
	if (!list_empty(list)) {
		__list_splice(list, head);
		INIT_LIST_HEAD(list);
	}
}




/*the same of container_of()*/
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

/*get the ptr of first element's content*/
#define list_get_entry(pos, head, member) \
	pos = list_entry((head)->next, typeof(*pos), member)


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); \
        	pos = pos->next)



#define __list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)



#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev;  pos != (head); \
        	pos = pos->prev)


#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/*loop, to get every ptr of the element's content*/
#define list_for_each_entry(pos, head, member)				\
	for (pos = list_entry((head)->next, typeof(*pos), member);	\
	     &pos->member != (head); 	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))


#define list_for_each_entry_reverse(pos, head, member)			\
	for (pos = list_entry((head)->prev, typeof(*pos), member);	\
	      &pos->member != (head); 	\
	     pos = list_entry(pos->member.prev, typeof(*pos), member))


#define list_prepare_entry(pos, head, member) \
	((pos) ? : list_entry(head, typeof(*pos), member))


#define list_for_each_entry_continue(pos, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))


#define list_for_each_entry_from(pos, head, member) 			\
	for (; &pos->member != (head);	\
	     pos = list_entry(pos->member.next, typeof(*pos), member))

#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_entry((head)->next, typeof(*pos), member),	\
		n = list_entry(pos->member.next, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))


#define list_for_each_entry_safe_continue(pos, n, head, member) 		\
	for (pos = list_entry(pos->member.next, typeof(*pos), member), 		\
		n = list_entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))


#define list_for_each_entry_safe_from(pos, n, head, member) 			\
	for (n = list_entry(pos->member.next, typeof(*pos), member);		\
	     &pos->member != (head);						\
	     pos = n, n = list_entry(n->member.next, typeof(*n), member))


#define list_for_each_entry_safe_reverse(pos, n, head, member)		\
	for (pos = list_entry((head)->prev, typeof(*pos), member),	\
		n = list_entry(pos->member.prev, typeof(*pos), member);	\
	     &pos->member != (head); 					\
	     pos = n, n = list_entry(n->member.prev, typeof(*n), member))

#endif
