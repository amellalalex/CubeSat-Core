#ifndef LORIS_MESSAGE_MESSAGE_H_
#define LORIS_MESSAGE_MESSAGE_H_

#include "KeyValuePairContainer.h"
#include <string>

//Used to represent a message that can be sent over via Messager
//Contains Key Value pairs as data, sender and recipient for routing, and Time Created for logging
class Message {
public:
    //Constructors
	Message();
    //Build message with sender and reciepient, time created will be set to the current date and time
    Message(unsigned int sender, unsigned int recipient);
    //Total constructor building message with all private members 
    Message(unsigned int sender, unsigned int recipient, long time,  KeyValuePairContainer contents);
    //Builds message from character array output of flatten method
    Message(char* flat);

    // Flattens message into a compressed character array that can be parsed by the Message(char* flat) constructor
    // msg - pointer to char array with a minimum size of 256 bytes.
    virtual void Flatten(char* msg) = 0;

    //Getters for private members
    unsigned int GetSender() const;
	unsigned int GetRecipient() const;
	long GetTimeCreated() const;
    KeyValuePairContainer GetMessageContents() const;

    //Setters for changing private members
	void SetSender(unsigned int sender_);
	void SetRecipient(unsigned int recipient_);
    void SetTimeCreated(long time_created_);

    //Methods to add various key value pairs
    void Add(const unsigned int key, int value);
    void Add(const unsigned int key, float value);
    void Add(const unsigned int key, std::string value);

    //Getters for relevant values of a given key
    int GetInt(const unsigned int key);
    float GetFloat(const unsigned int key);
    std::string GetString(const unsigned int key);

    //Getters for vector of various types of key value pair key's
    std::vector<int> GetFloatKeys();
    std::vector<int> GetIntKeys();
    std::vector<int> GetStringKeys();

    //Cleanly serializes message into nicely printable format
    virtual void ToString(char* string,int capacity);

protected:

    // Flatten method that may be used by the child class - returns new msg_size (CANNOT EXCEED 255)
    int FlattenHeader(char* msg, int msg_size);

    // Constructor helpers that allow common contents to be created in this clsss rather then childs
    // Returns new index
    int BuildHeader(char* flat, int index); //Stops running when sender, reciepient, and time are all parsed
    int BuildContents(char* flat, int index); //Stops running once end character is found

    unsigned int sender_;
    unsigned int recipient_;
    long time_created_;
    KeyValuePairContainer contents_;

};

#endif
