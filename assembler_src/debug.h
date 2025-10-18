# ifndef DEBUG_H
# define DEBUG_H

#include <stdio.h>

extern FILE* log_ptr;

enum db_mode
{
    off,
    on
};

// error messages in console always, but in debug mode also add in log file,
// msg only in debug mode in console and log file
// general info only in console

#define printf_empty_line(mode) do{  \
    printf("\n");                    \
    fprintf(log_ptr, "\n");          \
}                                    \
while(0)

#define printf_log_msg(mode, ...) do{           \
    if (mode == on)                             \
    {                                           \
        printf(__VA_ARGS__);                    \
        fprintf(log_ptr, __VA_ARGS__);          \
    }                                           \
}                                               \
while(0)


#define printf_err(mode, ...) do{                                             \
    printf_empty_line(mode);                                                  \
    printf(MAKE_BOLD_RED("ERROR: ") __VA_ARGS__);                             \
    fprintf(log_ptr, "ERROR: " __VA_ARGS__);                   \
}                                                                             \
while(0)

#define printf_warn(mode, ...) do{                                          \
    printf_empty_line(mode);                                                \
    printf(MAKE_BOLD("WARNING: ") __VA_ARGS__);                             \
    fprintf(log_ptr, "WARNING: " __VA_ARGS__);                   \
}                                                                           \
while(0)

#define printf_note(mode, text, ...) do{           \
    printf(MAKE_GREY(text), __VA_ARGS__);          \
}                                                  \
while(0)

#define printf_abortion(mode, ...) do{                                      \
    printf_empty_line(mode);                                                \
    printf(MAKE_BOLD_RED("aborting due to error: ") __VA_ARGS__);           \
    fprintf(log_ptr, "aborting due to error: " __VA_ARGS__); \
}                                                                           \
while(0)

#endif
