#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <asm/irq.h>

#define GPIO 27
#define BUTTON 22
#define DEV_NAME "ledtest_dev"
#define DEV_NUM 240

static char *msg = NULL;
int button_state = 0;

MODULE_LICENSE("GPL");

/*irqreturn_t interrupt_handler(int irq, void *dev_id){
  if(gpio_get_value(BUTTON)){
    while(gpio_get_value(BUTTON)){}
    button_state = 1;
    printk("button on\n");
  }
  printk("This is interrupt routine\n");
  return IRQ_HANDLED;
}
*/
int ledtest_open(struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "OPEN ledtest_dev\n");

  if(gpio_request(GPIO, "GPIO") < 0)
	  printk(KERN_ALERT "Led gpio allocation error!\n");
  if(gpio_direction_output(GPIO, 1) < 0)
	  printk(KERN_ALERT "Led setting error!\n");

  return 0;
}

int ledtest_close(struct inode *pinode, struct file *pfile){
  printk(KERN_ALERT "RELEASE ledtest_dev\n");

  return 0;
}

ssize_t button_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset){
  if(gpio_get_value(BUTTON) == 1)
  	copy_to_user(buffer,"btn_toggle", length);
  else
    copy_to_user(buffer, "no_signal", length);

  printk("btn test");
  return 0;
  /*while(gpio_get_value(BUTTON)==0){}
  if(gpio_get_value(BUTTON)==1)
  {
    printk("button input occured\n");

  if(button_state == 1){
    button_state = 0;
    printk("button interrupt occured!\n");

    copy_to_user(buffer, "btn_toggle", length);
  }else{
    copy_to_user(buffer, "no_signal", length);
  }
  printk("testtest!");
  return 0;*/

 // printk(KERN_ALERT "OPEN ledtest_dev\n");
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
  .read = button_read,
};

int __init ledtest_init(void){
  printk(KERN_ALERT "INIT ledtest\n");
  register_chrdev(DEV_NUM, DEV_NAME, &fop);

  msg = (char*)kmalloc(32, GFP_KERNEL);

  if(msg != NULL){
    printk("malloc allocator address: 0x%p\n", msg);
  }

  if(gpio_request(BUTTON, "BUTTON") < 0)
	  printk(KERN_ALERT "Button gpio allocation error\n");
  if(gpio_direction_input(BUTTON) < 0)
	  printk(KERN_ALERT "Button setting error\n");
  gpio_set_value(BUTTON, 0);
  //if(request_irq(gpio_to_irq(BUTTON), interrupt_handler, IRQF_TRIGGER_RISING, "Button", NULL) < 0) printk("Not request interrupt\n");
  return 0;
}

void __exit ledtest_exit(void){
  printk(KERN_ALERT "EXIT ledtest_dev\n");
  if(msg) kfree(msg);
  //free_irq(gpio_to_irq(BUTTON), interrupt_handler);
  unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(ledtest_init);
module_exit(ledtest_exit);
