#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define LCD_RS 573  // GPIO pin number for RS
#define LCD_RW 591  // GPIO pin number for RW
#define LCD_E  574  // GPIO pin number for E
#define LCD_DATA_PINS { 588, 598, 593, 576 }  // GPIO pins for D4-D7

// LCD commands
#define LCD_CMD_DISPLAY_ON  0x0C  // Command to turn on the display
#define LCD_CMD_CLEAR       0x01  // Command to clear the display
#define LCD_CMD_HOME        0x02  // Command to return home

static int lcd_pins[] = LCD_DATA_PINS; // Data pins array

// Function prototypes
static void lcd_send_command(uint8_t cmd);
static void lcd_enable_pulse(void);
static void lcd_gpio_setup(void);
static void lcd_display_on(void);
static void lcd_clear_display(void);
static void lcd_home(void);
static int lcd_read_busy_flag(void);

static int __init lcd_init(void) {
    pr_err("LCD Driver: Initializing...\n");

    // Set up GPIO pins for the LCD
    lcd_gpio_setup();
    
    // Initialize the LCD
    mdelay(20); // Wait >15ms after power-up
    pr_err("LCD Driver: Sending function set command (4-bit mode, 2 lines, 5x8 dots)\n");
    lcd_send_command(0x28); // Function set: 4-bit mode, 2 lines, 5x8 dots
    
    lcd_display_on(); // Turn on the display
    lcd_clear_display(); // Clear display
    lcd_home(); // Return to home

    pr_err("LCD Driver: Initialization complete. LCD turned ON.\n");

    // Read the busy flag to confirm the last command was received
    if (lcd_read_busy_flag() == 0) {
        pr_err("LCD Driver: Command successfully processed.\n");
    } else {
        pr_err("LCD Driver: LCD is still busy, command might not have been processed.\n");
    }

    return 0;
}

static void __exit lcd_exit(void) {
    pr_err("LCD Driver: Exiting and releasing resources...\n");

    // Release GPIOs
    gpio_free(LCD_RS);
    gpio_free(LCD_RW);
    gpio_free(LCD_E);
    for (int i = 0; i < 4; i++) {
        gpio_free(lcd_pins[i]);
    }

    pr_err("LCD Driver: Resources released.\n");
}

// Setup GPIOs for LCD
static void lcd_gpio_setup(void) {
    pr_err("LCD Driver: Setting up GPIOs for RS, RW, E, and data pins (D4-D7)\n");

    // Request GPIOs for RS, RW, E
    if (gpio_request_one(LCD_RS, GPIOF_OUT_INIT_LOW, "LCD_RS")) {
        pr_err("LCD Driver: Failed to request GPIO for RS\n");
    }
    if (gpio_request_one(LCD_RW, GPIOF_OUT_INIT_LOW, "LCD_RW")) {
        pr_err("LCD Driver: Failed to request GPIO for RW\n");
    }
    if (gpio_request_one(LCD_E, GPIOF_OUT_INIT_LOW, "LCD_E")) {
        pr_err("LCD Driver: Failed to request GPIO for E\n");
    }

    // Request GPIOs for D4-D7 (data pins)
    for (int i = 0; i < 4; i++) {
        if (gpio_request_one(lcd_pins[i], GPIOF_OUT_INIT_LOW, "LCD_D")) {
            pr_err("LCD Driver: Failed to request GPIO for data pin D%d\n", i + 4);
        }
    }

    pr_err("LCD Driver: GPIO setup completed.\n");
}

// Send command to the LCD
static void lcd_send_command(uint8_t cmd) {
    pr_err("LCD Driver: Sending command 0x%x\n", cmd);

    // RS = 0 for command mode
    gpio_set_value(LCD_RS, 0);
    // RW = 0 for write mode
    gpio_set_value(LCD_RW, 0);

    // Send higher nibble (D4-D7)
    for (int i = 0; i < 4; i++) {
        gpio_set_value(lcd_pins[i], (cmd >> (i + 4)) & 0x01);
    }
    lcd_enable_pulse();

    // Send lower nibble (D4-D7)
    for (int i = 0; i < 4; i++) {
        gpio_set_value(lcd_pins[i], (cmd >> i) & 0x01);
    }
    lcd_enable_pulse();

    pr_err("LCD Driver: Command 0x%x sent.\n", cmd);

    // Small delay to allow the LCD to process the command
    mdelay(2);
}

// Read busy flag and address counter
static int lcd_read_busy_flag(void) {
    uint8_t busy_flag;

    pr_err("LCD Driver: Reading busy flag.\n");

    // Set data pins as input for reading
    for (int i = 0; i < 4; i++) {
        gpio_direction_input(lcd_pins[i]);
    }

    // RS = 0 for command mode, RW = 1 for read mode
    gpio_set_value(LCD_RS, 0);
    gpio_set_value(LCD_RW, 1);

    // Read higher nibble (D7 is the busy flag)
    lcd_enable_pulse();
    busy_flag = gpio_get_value(lcd_pins[3]); // D7 (MSB)

    // Restore data pins to output mode after reading
    for (int i = 0; i < 4; i++) {
        gpio_direction_output(lcd_pins[i], 0);
    }

    pr_err("LCD Driver: Busy flag read, value: %d\n", busy_flag);

    return busy_flag; // Return the busy flag status (0 if ready, 1 if busy)
}

// Create an enable pulse to latch data into the LCD
static void lcd_enable_pulse(void) {
    gpio_set_value(LCD_E, 1);
    udelay(1);  // Small delay
    gpio_set_value(LCD_E, 0);
    udelay(1);
    pr_err("LCD Driver: Enable pulse sent.\n");
}

// Turn the display ON (display on, cursor off, blink off)
static void lcd_display_on(void) {
    pr_err("LCD Driver: Turning display ON.\n");
    lcd_send_command(LCD_CMD_DISPLAY_ON);
}

// Clear the LCD display
static void lcd_clear_display(void) {
    pr_err("LCD Driver: Clearing display.\n");
    lcd_send_command(LCD_CMD_CLEAR);
    mdelay(2);  // This command needs more delay
}

// Return the cursor to the home position
static void lcd_home(void) {
    pr_err("LCD Driver: Returning cursor to home position.\n");
    lcd_send_command(LCD_CMD_HOME);
    mdelay(2);  // This command needs more delay
}

// Module entry and exit points
module_init(lcd_init);
module_exit(lcd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A Linux kernel module to control an LCD display with read capability and debug logs.");
MODULE_VERSION("1.0");