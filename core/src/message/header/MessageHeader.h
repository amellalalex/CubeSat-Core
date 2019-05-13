
#ifndef LORIS_MESSAGE_MESSAGEHEADER_H_
#define LORIS_MESSAGE_MESSAGEHEADER_H_


class MessageHeader {
public:
    MessageHeader(unsigned int recipient, unsigned int sender, long time_created);
    friend message_serialize (Message *message, float *data);

private:
    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
};

#endif
