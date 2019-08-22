//
// Created by Spencer Axford on 2019-05-16.
//

#include "Repository.h"
#include <string>

Repository::Repository(std::string socket_path)
        : UnixDomainStreamSocketServer(socket_path) {this->identifier_=0;}

int Repository::HandleMessage(char *buffer,int client_file_descriptor){
    cout << "Handling message: " << buffer << endl;

    //Convert the buffer contents into a DataMessage object
    DataMessage message = DataMessage(buffer);

    //Extract any new, watched data from the message
    ExtractDataFromReceivedMessage(message);

    //Perform any additional, optional processing for the message
    ProcessMessage(message);

    //Build an empty reply message
 	DataMessage reply_message(this->GetIdentifier(),message.GetSender());

 	//Append any requested data to the reply_message, if the repository has it
    if(message.HasRequests()){
      	BuildReturnDataMessage(message,reply_message);
    }

    //Reply to the client
   	ReplyToConnectedClient(reply_message);

    return 0;
}



int Repository::ExtractDataFromReceivedMessage(DataMessage received_message){
    //TODO remove the cout statements when deemed "too annoying and unnecessary"
	cout << "Getting message key value pairs" << endl;
    KeyValuePairContainer data = received_message.GetMessageContents();

    //Delete this-----
    std::vector<int> floatKeys = data.GetFloatKeys();
    std::vector<int> intKeys = data.GetIntKeys();
    std::vector<int> stringKeys = data.GetStringKeys();
    cout << "key value pairs:" << endl;

    //Logging statements
    for(int i = 0; i < intKeys.size(); i++){
    	cout << intKeys.at(i) << " : " << data.GetInt(intKeys.at(i)) << endl;
    }

    for(int i = 0; i < floatKeys.size(); i++){
    	cout << floatKeys.at(i) << " : " << data.GetFloat(floatKeys.at(i)) << endl;
    }

    for(int i = 0; i < stringKeys.size(); i++){
    	cout << stringKeys.at(i) << " : " << data.GetString(stringKeys.at(i)) << endl;
    }
    //Delete this ^^^^

    cout << "Adding data" << endl;
    //Get all keys for each primitive type
	std::vector<int> float_keys = received_message.GetFloatKeys();
	std::vector<int> int_keys = received_message.GetIntKeys();
	std::vector<int> string_keys = received_message.GetStringKeys();

	//Iterate through all key float value pairs and add any that are watched
	for(int i=0;i<float_keys.size();i++){
		unsigned int current_key = float_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetFloat(current_key));
		}
	}

	//Iterate through all key int value pairs and add any that are watched
	for(int i=0;i<int_keys.size();i++){
		unsigned int current_key = int_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetInt(current_key));
		}
	}

	//Iterate through all key string value pairs and add any that are watched
	for(int i=0;i<string_keys.size();i++){
		unsigned int current_key = string_keys[i];
		cout << "Current Key: " << current_key << endl;
		if(WatchListContainsKey(current_key)){
			cout << "Updating value for: "<<current_key << endl;
			repository_data_.AddKeyValuePair(current_key,received_message.
					GetString(current_key));
		}
	}

	return 0;
}

int Repository::ReplyToConnectedClient(DataMessage& message){
	//TODO Spencer, can I just arbitrarily set the length of the message? and send it? - Andrew
    char msg[255] = "";
	message.Flatten(msg);
	int client_file_descriptor=this->current_client_file_descriptor();
	cout << "Replying to client at " << client_file_descriptor << " with message: " << msg << endl;
	WriteToSocket(msg,client_file_descriptor);
	return 1;
}


bool Repository::WatchListContainsKey(unsigned int key){

	//TODO there is the potential for optimizing this search by
	//sorting the array and using a search algorithm
	for(int i=0;i<this->watch_list_.size();i++){
		if(watch_list_.at(i).key()==key){
			return true;
		}
	}
	return false;
}

int Repository::BuildReturnDataMessage(DataMessage request_message,DataMessage& return_message){
	std::vector<int> requests = request_message.GetRequests();
	int number_of_reqs=requests.size();
	if(number_of_reqs>0){
		cout << "Iterating through request keys" << endl;
		bool no_keys_watched_and_requested=true;
		//Iterate through all requested keys
		for(int i=0;i<number_of_reqs;i++){
			//Check if the requested key is watched by this repository
			unsigned int requested_key = requests[i];
			cout << "uint requested_key " << requested_key << endl;
			cout << "requested_key " << requests[i] << endl;

			if(WatchListContainsKey(requested_key)){
				no_keys_watched_and_requested=false;
				//Check if the repository contains the requested key
				if(repository_data_.ContainsIntKey(requested_key)){
					return_message.Add(requested_key,
						repository_data_.GetInt(requested_key));
				}
				if(repository_data_.ContainsFloatKey(requested_key)){
					return_message.Add(requested_key,
						repository_data_.GetFloat(requested_key));
				}

			}
		}
		if(no_keys_watched_and_requested){
			cout << "The message contained requests which the repository does not contain data for." << endl;
		}
	}
	else{
		cout << "The data message contained no requests." << endl;
	}
	return 1;
}


unsigned int Repository::GetIdentifier(){
	return this->identifier_;
}
