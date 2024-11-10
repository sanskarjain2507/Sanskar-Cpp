#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/delay.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sanskar Jain");
MODULE_DESCRIPTION("A driver to write to a LCD text display");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "lcd"
#define DRIVER_CLASS "MyModuleClass"

/* LCD char buffer */
static char lcd_buffer[17];
#define LCD_RW 591 
/* Pinout for LCD Display */
unsigned int gpios[] = {
	574, /* Enable Pin */
	573, /* Register Select Pin */
	594, /* Data Pin 0*/
	595, /* Data Pin 1*/
	596, /* Data Pin 2*/
	587, /* Data Pin 3*/
	588, /* Data Pin 4*/
	598, /* Data Pin 5*/
	593, /* Data Pin 6*/
	576, /* Data Pin 7*/
};

#define REGISTER_SELECT gpios[1]

/**
 * @brief generates a pulse on the enable signal
 */

void lcd_enable(void) {
	gpio_set_value(gpios[0], 1);
	msleep(40);
	gpio_set_value(gpios[0], 0);
    msleep(40);
}


static int lcd_read_busy_flag(void) {
    uint8_t busy_flag;

    pr_err("LCD Driver: Reading busy flag.\n");

    // Set data pins as input for reading
    for (int i = 2; i < 10; i++) {
        gpio_direction_input(gpios[i]);
    }

    // RS = 0 for command mode, RW = 1 for read mode
    gpio_set_value(REGISTER_SELECT, 0);
    gpio_set_value(LCD_RW, 1);

    // Read higher nibble (D7 is the busy flag)
    lcd_enable();
    busy_flag = gpio_get_value(gpios[9]); // D7 (MSB)

    // Restore data pins to output mode after reading
    for (int i = 2; i < 10; i++) {
        gpio_direction_output(gpios[i], 0);
    }

    pr_err("LCD Driver: Busy flag read, value: %d\n", busy_flag);

    return busy_flag; // Return the busy flag status (0 if ready, 1 if busy)
}


/**
 * @brief set the 8 bit data bus
 * @param data: Data to set
 */
void lcd_send_byte(char data) {
	int i;
	for(i=0; i<8; i++)
		gpio_set_value(gpios[i+2], ((data) & (1<<i)) >> i);
	lcd_enable();
	msleep(40);
}

/**
 * @brief send a command to the LCD
 *
 * @param data: command to send
 */
void lcd_command(uint8_t data) {
    gpio_set_value(LCD_RW, 0);
 	gpio_set_value(REGISTER_SELECT, 0);	/* RS to Instruction */
	lcd_send_byte(data);
}

/**
 * @brief send a data to the LCD
 *
 * @param data: command to send
 */
void lcd_data(uint8_t data) {
 	gpio_set_value(REGISTER_SELECT, 1);	/* RS to data */
    gpio_set_value(LCD_RW, 0);
	lcd_send_byte(data);
}


/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs) {
	int to_copy, not_copied, delta, i;

	/* Get amount of data to copy */
	to_copy = min(count, sizeof(lcd_buffer));

	/* Copy data to user */
	not_copied = copy_from_user(lcd_buffer, user_buffer, to_copy);

	/* Calculate data */
	delta = to_copy - not_copied;

	/* Set the new data to the display */
	lcd_command(0x1);
    printk("Writing...");
	for(i=0; i<to_copy; i++)
		lcd_data(lcd_buffer[i]);

	return delta;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.write = driver_write
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	int i;
	char *names[] = {"ENABLE_PIN", "REGISTER_SELECT", "DATA_PIN0", "DATA_PIN1", "DATA_PIN2", "DATA_PIN3", "DATA_PIN4", "DATA_PIN5", "DATA_PIN6", "DATA_PIN7"};
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create device class */
	if((my_class = class_create(DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */
	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("lcd-driver - Registering of device to kernel failed!\n");
		goto AddError;
	}

	/* Initialize GPIOs */
	printk("lcd-driver - GPIO Init\n");
	for(i=0; i<10; i++) {
		if(gpio_request(gpios[i], names[i])) {
			printk("lcd-driver - Error Init GPIO %d\n", gpios[i]);
			goto GpioInitError;
		}
	}

	printk("lcd-driver - Set GPIOs to output\n");
	for(i=0; i<10; i++) {
		if(gpio_direction_output(gpios[i], 0)) {
			printk("lcd-driver - Error setting GPIO %d to output\n", i);
			goto GpioDirectionError;
		}
	}

	/* Init the display */
while(lcd_read_busy_flag())
{
lcd_command(0x30); // Function set: 8-bit mode
msleep(40);      // Wait for more than 40ms
printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
}
lcd_command(0x38); // Function set: 8-bit, 2 lines, 5x8 dots
msleep(40); 
printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
lcd_command(0x0C); // Display on, cursor off, blink off
msleep(40); 
printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
lcd_command(0x06); // Entry mode set: increment, no shift
msleep(40); 
printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
lcd_command(0x01); // Clear display
msleep(40);      // Wait for more than 1.52ms
printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
	char text[] = "Hello World!";
	for(i=0; i<sizeof(text)-1;i++)
		lcd_data(text[i]);
    // msleep(100000);
    printk("lcd-driver -busy flag %d to output\n",lcd_read_busy_flag() );
	return 0;
GpioDirectionError:
	i=9;
GpioInitError:
	for(;i>=0; i--)
		gpio_free(gpios[i]);
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	int i;
	lcd_command(0x1);	/* Clear the display */
	for(i=0; i<10; i++){
		gpio_set_value(gpios[i], 0);
		gpio_free(gpios[i]);
	}
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);

