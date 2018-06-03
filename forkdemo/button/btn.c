#include<linux/gpio.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/delay.h>
#include<linux/fs.h>
#include<asm/gpio.h>
#define GPIO_NUMBER 18 
int gpio_nreset = GPIO_NUMBER;
#define DEV_NAME "btn_dev"
#define DEV_NUM 240

MODULE_LICENSE("GPL");

/*int btn_open(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "OPEN btn_dev\n");
	cs4271_probe();
}*/

int btn_close(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "RELEASE btn_dev\n");
	return 0;
}



int btn_open(struct inode *pinode, struct file *pfile){
	printk(KERN_ALERT "OPEN btn_dev\n");
	int check=gpio_request(GPIO_NUMBER, "GPIO SET");
	while(check!=0){
	gpio_free(GPIO_NUMBER);check=gpio_request(GPIO_NUMBER,"GPIO SET");
}
	printk(KERN_ALERT "gpio can be used");

	if(gpio_direction_input(GPIO_NUMBER)!=0)
	printk(KERN_ALERT "gpio dir input error");
	
	udelay(1);

	int pushCount=0;

//	while(pushCount<3){
	if(gpio_get_value(GPIO_NUMBER)==1)
	printk(KERN_ALERT "Button is Pressed!");

/*	gpio_request(GPIO_NUMBER, "GPIO SET");
	gpio_direction_output(GPIO_NUMBER, 0);
	gpio_set_value(GPIO_NUMBER,0);
	gpio_request(GPIO_NUMBER, "GPIO SET");
	gpio_direction_input(GPIO_NUMBER);*/
//	pushCount++;
//	}
	

	
	udelay(1);
	gpio_free(GPIO_NUMBER);	
	return 0;
}

struct file_operations fop={
        .owner=THIS_MODULE,
        .open=btn_open,
        .release=btn_close,
};
    

int __init btntest_init(void){
	printk(KERN_ALERT "INIT btntest\n");
	register_chrdev(DEV_NUM, DEV_NAME, &fop);
	return 0;
}

void __exit btntest_exit(void){
	printk(KERN_ALERT "EXIT btntest_dev\n");
	unregister_chrdev(DEV_NUM, DEV_NAME);
}

module_init(btntest_init);
module_exit(btntest_exit);
