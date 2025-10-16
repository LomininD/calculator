# ifndef DEBUG_H
# define DEBUG_H

enum db_mode
{
    off,
    on
};

// error messages in console always, but in debug mode also add in log file,
// msg only in debug mode in console and log file
// general info in console always in debug also in log

// REFACTOR in one define with VA_ARGS
// reorder error message

// messages types:
// - error messages with red ERROR (done)
// - warning messages (done)
// - messages of shutting down due to error (done)
// - debug messages (done)
// - general messages (done)
// - note messages (done)
// - empty line (done)

#define printf_empty_line(mode) do{  \
    printf("\n");                    \
}                                    \
while(0)

#define printf_gen_info(mode, text, ...) do{     \
        printf(text , __VA_ARGS__);              \
}                                                \
while(0)

#define printf_log_msg(mode, text, ...) do{     \
    if (mode == 1)                              \
    {                                           \
        printf(text, __VA_ARGS__);              \
    }                                           \
}                                               \
while(0)


#define printf_err(mode, called_place, err_text, ...) do{                     \
    printf_empty_line(mode);                                                  \
    printf(called_place MAKE_BOLD_RED(" ERROR: ") err_text, __VA_ARGS__);     \
}                                                                             \
while(0)

#define printf_warn(mode, called_place, warn_text, ...) do{                 \
    printf_empty_line(mode);                                                \
    printf(called_place MAKE_BOLD(" WARNING: ") warn_text, __VA_ARGS__);    \
}                                                                           \
while(0)

#define printf_note(mode, text, ...) do{           \
    printf(MAKE_GREY(text), __VA_ARGS__);          \
}                                                  \
while(0)

#define printf_abortion(mode, called_place, err_text) do{   \
    printf_empty_line(mode);                                \
    printf(called_place " " MAKE_BOLD_RED(err_text));       \
}                                                           \
while(0)

#endif
