#pragma once

#include "Util.h"
#include "SingletonBase.h"

class Message 
{
public:
    virtual ~Message() = default;
};

class MessageEventUpdate : public Message 
{
public:
    std::wstring eventName;
    MessageEventUpdate(const std::wstring& msg) : eventName(msg) {}
};

using MessageHandler = std::function<void(std::shared_ptr<Message>)>;


class MessageDispatcher : public SingletonBase<MessageDispatcher>
{
public:

    void registerHandler(const std::string& eventType, MessageHandler handler) 
    {
        handlers[eventType].push_back(handler);
    }


    void dispatch(const std::string& eventType, std::shared_ptr<Message> event) 
    {
        if (handlers.find(eventType) != handlers.end()) 
        {
            for (auto& handler : handlers[eventType]) 
            {
                handler(event);
            }
        }
    }

private:
    friend class SingletonBase<MessageDispatcher>;
    std::unordered_map<std::string, std::vector<MessageHandler>> handlers;
};

