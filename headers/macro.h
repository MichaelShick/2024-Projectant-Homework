/**
 * @file macro.h
 * @brief Header file containing function declarations for macro operations.
 *
 * This file includes function declarations for operations such as string replacement,
 * macro replacement, and reading/writing files related to macros.
 *
 * Created by aviv on 15/08/2024.
 */

#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024 ///< Maximum length of a line in the text file.

/**
 * @brief Replaces occurrences of a substring within a string.
 *
 * This function replaces all occurrences of the substring `rep` within the string `orig`
 * with the substring `with`.
 *
 * @param orig The original string.
 * @param rep The substring to replace.
 * @param with The substring to replace with.
 * @return A new string with the replacements applied. The caller is responsible for freeing the returned string.
 */
char *str_replace(char *orig, char *rep, char *with);

/**
 * @brief Replaces macros in a given text.
 *
 * This function replaces macros defined within the provided text with their corresponding values.
 *
 * @param text The input text containing macros.
 * @return A new string with macros replaced. The caller is responsible for freeing the returned string.
 */
char *replace_macro(const char *text);

/**
 * @brief Applies all macro replacements in a given text.
 *
 * This function processes the provided text and replaces all macros with their corresponding values.
 *
 * @param text The input text containing macros.
 * @return A new string with all macros replaced. The caller is responsible for freeing the returned string.
 */
char *all_macro(char *text);

/**
 * @brief Reads the content of a file into a string.
 *
 * This function reads the content of the specified file and returns it as a dynamically allocated string.
 *
 * @param filename The name of the file to read.
 * @return A string containing the file's content. The caller is responsible for freeing the returned string.
 */
char *read_file(const char *filename);

/**
 * @brief Writes text with all macros replaced to a file.
 *
 * This function writes the provided text, with all macros replaced, to the specified file.
 *
 * @param filename The name of the file to write to.
 * @param text The text to write, after replacing all macros.
 */
void writeAllMacroToTextFile(const char *filename, char *text);

#endif // MACRO_H
