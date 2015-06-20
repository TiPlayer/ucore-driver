//
//  irq.h
//  uCore with linux kmodule support
//
//  Created by Yue Shichao on 6/12/15.
//  Copyright (c) 2015 Yue Shichao. All rights reserved.
//

struct trapframe;

typedef int(*interrupt_func)(int phy_vector, void* data);

#define INTR_MINE_AND_FAIL 0
#define INTR_MINE_AND_SUCCESS 1
#define INTR_RETURN_NOT_MINE 2

struct interrupt_handler {
  interrupt_func func;
  struct interrupt_handler* next;
  void* data;
};

#define MAX_LOGIC_INTR_NUMBER 256
struct irq_manager {
  struct interrupt_handler* handler_vector[MAX_LOGIC_INTR_NUMBER];
  int (*p2l_irq)(int);
  int (*get_logic_irq)(int);
  void (*eoi)();
};

typedef struct irq_manager irq_m_t;
typedef struct interrupt_handler ih_t;
typedef interrupt_func if_t;

#define TIMER_INDEX 0
#define COM1_INDEX 1
#define KBD_INDEX 2
#define IDE1_INDEX 3
#define IDE2_INDEX 4

int default_get_logic_irq(int me);
int register_intr_handler(int logic_no, if_t func, void* data, int solo);
void irq_manager_init(int(*p2l_irq)(int), int (*get_logic_irq)(int), void (*eoi)());
void external_irq_handler(int irq);
