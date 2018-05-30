#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/slab.h>

#define GPIO 27
#define DEV_NAME "ledtest_dev"
#define DEV_NUM 240

static char *msg = NULL;

MODULE_LICENSE("GPL");

int ledtest_open(struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "OPEN ledtest_dev\n");
  gpio_request(GPIO, "GPIO");
  gpio_direction_output(GPIO, 1);

  return 0;
}

int ledtest_close(struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "RELEASE ledtest_dev\n");
  return 0;
}

ssize_t ledtest_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset){
  copy_from_user(msg, buffer, length);

  if(strcmp(msg, "led_off") == 0){
    printk("led off\n");
    gpio_set_value(GPIO, 0);
  }else if(strcmp(msg, "led_on") == 0){
    printk("led on\n");
    gpio_set_value(GPIO, 1);
  }

  return length;
}

struct file_operations fop = {
  .owner = THIS_MODULE,
  .open = ledtest_open,
  .release = ledtest_close,
  .write = ledtest_write,
};

int __init ledtest_init(void){
  printk(KERN_ALERT "INIT ledtest\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);

  msg = (char*)kmalloc(32, GFP_KERNEL);

  if(msg != NULL){
    printk("malloc allocator address: 0x%p\n", msg);
  }

  return 0;
}

void __exit ledtest_exit(void){
  printk(KERN_ALERT "EXIT ledtest_dev\n");
  if(msg) kfree(msg);
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(ledtest_init);
module_exit(ledtest_exit);
