#include "../headers/macro.h"

/**
 * @brief Replaces a macro in the provided text with its content.
 *
 * This function searches for the first occurrence of the keyword "macro" in the input text,
 * extracts the macro name and its content, and then replaces all occurrences of the macro
 * name in the text with the macro content. The macro definition itself is removed from the text.
 *
 * @param text The input text containing the macro definition.
 * @return A newly allocated string with the macro replaced, or a copy of the original text if no macro is found.
 *         The caller is responsible for freeing the returned string.
 */
char *replace_macro(const char *text) {
    char *macro_start = strstr(text, "macro ");
    if (!macro_start) {
        return strndup(text, MAX_LINE_LENGTH);  // No macro found, return a copy of the original text
    }

    // Skip the "macro " part
    macro_start += strnlen("macro ", MAX_LINE_LENGTH);

    // Find the end of the macro name (assumed to be separated by a space or newline)
    char *name_end = strpbrk(macro_start, " \n");
    if (!name_end) {
        return strndup(text, MAX_LINE_LENGTH);  // Invalid macro definition
    }

    // Extract the macro name
    int name_len = name_end - macro_start;
    char *macro_name = malloc(name_len + 1);
    strncpy(macro_name, macro_start, name_len);
    macro_name[name_len] = '\0';

    // Find the macro content start (right after the macro name)
    char *content_start = name_end + 1;

    // Find the "endm" keyword
    char *macro_end = strstr(content_start, "endm");
    if (!macro_end) {
        free(macro_name);
        return strndup(text, MAX_LINE_LENGTH);  // Invalid macro definition
    }

    // Calculate the length of the macro content
    int content_len = macro_end - content_start;
    char *macro_content = malloc(content_len + 1);
    strncpy(macro_content, content_start, content_len);
    macro_content[content_len] = '\0';

    // Calculate the new text length after removing the macro
    size_t prefix_len = macro_start - text - strnlen("macro ", MAX_LINE_LENGTH);
    size_t suffix_len = strnlen(macro_end + strnlen("endm", MAX_LINE_LENGTH), MAX_LINE_LENGTH);
    size_t new_text_len = prefix_len + suffix_len;

    // Create new text without the macro definition
    char *new_text = malloc(new_text_len + 1);
    strncpy(new_text, text, prefix_len);
    size_t len = strnlen(macro_end + strnlen("endm", MAX_LINE_LENGTH), MAX_LINE_LENGTH);
    strncpy(new_text + prefix_len, macro_end + strnlen("endm", MAX_LINE_LENGTH), len);
    new_text[prefix_len + len] = '\0'; // Ensure null-termination

    // Replace the macro name with its content in the modified text
    char *result = str_replace(new_text, macro_name, macro_content);

    // Free allocated memory
    free(macro_name);
    free(macro_content);
    free(new_text);

    return result;
}

/**
 * @brief Replaces all occurrences of a substring within a string.
 *
 * This function replaces all occurrences of the substring `rep` within the string `orig`
 * with the substring `with`. The resulting string is dynamically allocated.
 *
 * @param orig The original string.
 * @param rep The substring to replace.
 * @param with The substring to replace with.
 * @return A new string with the replacements applied, or NULL if an error occurs.
 *         The caller is responsible for freeing the returned string.
 */
char *str_replace(char *orig, char *rep, char *with) {
    if (!orig || !rep || strnlen(rep, MAX_LINE_LENGTH) == 0)
        return NULL;

    int len_rep = strnlen(rep, MAX_LINE_LENGTH);
    int len_with = with ? strnlen(with, MAX_LINE_LENGTH) : 0;

    // Count the number of replacements needed
    int count = 0;
    char *tmp = orig;
    while ((tmp = strstr(tmp, rep))) {
        count++;
        tmp += len_rep;
    }

    // Allocate memory for the new string
    char *result = malloc(strnlen(orig, MAX_LINE_LENGTH) + count * (len_with - len_rep) + 1);
    if (!result)
        return NULL;

    // Replace occurrences
    char *ins = orig;
    char *res = result;
    while (count--) {
        char *pos = strstr(ins, rep);
        int len_front = pos - ins;
        res = strncpy(res, ins, len_front) + len_front;
        res = strncpy(res, with, len_with);
        res += len_with;
        ins = pos + len_rep;
    }
    res = strncpy(res, ins, strnlen(ins, MAX_LINE_LENGTH));
    res[strnlen(ins, MAX_LINE_LENGTH)] = '\0';
    return result;
}

/**
 * @brief Processes all macros in the given text.
 *
 * This function processes the given text, replacing all macros with their content.
 * The replacement continues until no more "macro" keywords are found.
 *
 * @param text The input text containing macros.
 * @return The modified text with all macros replaced.
 *         The caller is responsible for freeing the returned string.
 */
char *all_macro(char *text) {
    char *result = text;
    while (strstr(result, "macro ")) {
        result = replace_macro(result);
    }
    return result;
}

/**
 * @brief Reads the content of a file into a string.
 *
 * This function reads the content of the specified file and returns it as a dynamically allocated string.
 *
 * @param filename The name of the file to read.
 * @return A string containing the file's content, or NULL if the file cannot be opened.
 *         The caller is responsible for freeing the returned string.
 */
char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        perror("Failed to allocate buffer");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    return buffer;
}

/**
 * @brief Writes the processed text with all macros replaced to a file.
 *
 * This function writes the given text, after replacing all macros, to the specified file.
 *
 * @param filename The name of the file to write to.
 * @param text The text to write, after replacing all macros.
 */
void writeAllMacroToTextFile(const char *filename, char *text) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char *result = all_macro(text);
    fprintf(file, "%s", result);

    free(result);
    fclose(file);
}
