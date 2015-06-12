//
//  irq.c
//  uCore with linux kmodule support
//
//  Created by Yue Shichao on 6/12/15.
//  Copyright (c) 2015 Yue Shichao. All rights reserved.
//


#include <kio.h>
#include <slab.h>
#include <trap.h>
#include <irq.h>


static irq_m_t default_irq_manager;


static bool try_handle_irq(ih_t* handler, int phy_vector) {
  while (handler != NULL && handler->func != NULL) {
    if(handler->func(phy_vector, handler->data) == INTR_MINE_AND_SUCCESS) {
      return 1;
    }
    handler = handler->next;
  }
  return 0;
}


int register_intr_handler(int logic_no, if_t func, void* data, int solo) {
  ih_t* end = default_irq_manager.handler_vector[logic_no];
  if (solo && end) return -1; //Error Here
  ih_t* handler = kmalloc(sizeof(ih_t));
  handler->func = func;
  handler->next = NULL;
  handler->data = data;
  if (end == NULL) {
    default_irq_manager.handler_vector[logic_no] = handler;
  } else {
    while (end->next != NULL) end = end->next;
    end->next = handler;
  }
  return 0;
}

void irq_manager_init(int(*p2l_irq)(int), int (*get_logic_irq)(int), void (*eoi)()) {
  int i = 0;
  for (i = 0; i < MAX_LOGIC_INTR_NUMBER; i++) {
    default_irq_manager.handler_vector[i] = NULL;
  }
  default_irq_manager.p2l_irq = p2l_irq;
  default_irq_manager.get_logic_irq = get_logic_irq;
  default_irq_manager.eoi = eoi;
}

void external_irq_handler(int phy_vector) {
  int logic_irq = default_irq_manager.p2l_irq(phy_vector);
  if (!try_handle_irq(default_irq_manager.handler_vector[logic_irq], phy_vector)) {
    kprintf("Discard Irq: %d\n", logic_irq);
  }
}

int default_get_logic_irq(int me) {
  switch (me) {
    case TIMER_INDEX:
      return IRQ_TIMER;
    case COM1_INDEX:
      return IRQ_COM1;
    case KBD_INDEX:
      return IRQ_KBD;
    case IDE1_INDEX:
      return IRQ_IDE1;
    case IDE2_INDEX:
      return IRQ_IDE2;
    default:
      return -1;
  }
}
