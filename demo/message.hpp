#ifndef MESSAGE_H
#define MESSAGE_H

enum class message_type
{
    NOT_DEFINED = 0,
    HELLO,
    GOODBYE,
    SET_PARAMETER,
    READ_DATA,
};

struct message
{
    message_type type {message_type::NOT_DEFINED};
    int parameter {0};
    bool has_handle {false};
    long handle {0};
};

#endif   // MESSAGE_H
