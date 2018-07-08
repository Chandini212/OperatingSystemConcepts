#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday {
  int day;
  int month;
  int year;
  struct list_head list;
};

static LIST_HEAD(birthday_list);

int birthdayList_init (void)
{
  printk(KERN_INFO "Creating birthday list\n");

  struct birthday *person1;
  person1 = kmalloc(sizeof(*person1), GFP_KERNEL);
  person1->day = 1;
  person1->month = 1;
  person1->year = 1991;
  INIT_LIST_HEAD(&person1->list);
  list_add_tail(&person1->list, &birthday_list);

  struct birthday *person2;
  person2 = kmalloc(sizeof(*person2), GFP_KERNEL);
  person2->day = 1;
  person2->month = 2;
  person2->year = 1992;
  INIT_LIST_HEAD(&person2->list);
  list_add_tail(&person2->list, &birthday_list);

  struct birthday *person3;
  person3 = kmalloc(sizeof(*person3), GFP_KERNEL);
  person3->day = 1;
  person3->month = 3;
  person3->year = 1993;
  INIT_LIST_HEAD(&person3->list);
  list_add_tail(&person3->list, &birthday_list);

  struct birthday *person4;
  person4 = kmalloc(sizeof(*person4), GFP_KERNEL);
  person4->day = 1;
  person4->month = 4;
  person4->year = 1994;
  INIT_LIST_HEAD(&person4->list);
  list_add_tail(&person4->list, &birthday_list);

  struct birthday *person5;
  person5 = kmalloc(sizeof(*person5), GFP_KERNEL);
  person5->day = 1;
  person5->month = 5;
  person5->year = 1995;
  INIT_LIST_HEAD(&person5->list);
  list_add_tail(&person5->list, &birthday_list);

  printk(KERN_INFO "Traversing birthday list\n");

  struct birthday *bday_ptr;

  list_for_each_entry(bday_ptr, &birthday_list, list)
  {
    printk(KERN_INFO "BIRTHDAY: Day: %d Month %d Year: %d\n", bday_ptr->day, bday_ptr->month, bday_ptr->year);
  }

  return 0;
}


void birthdayList_exit (void)
{
  struct birthday *bday_ptr, *bday_next;

  list_for_each_entry_safe(bday_ptr, bday_next, &birthday_list, list)
  {
    list_del(&bday_ptr->list);
    kfree(bday_ptr);
  }
}

module_init(birthdayList_init);
module_exit(birthdayList_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("BIRTHDAY LIST");
MODULE_AUTHOR("ME");
