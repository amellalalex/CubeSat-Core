#include "../header/Message.h"

Message::Message() {

}

Message::Message(std::string contents) {
    this->contents_=contents;
}

std::string Message::getContents() {
    return this->contents_;
}
