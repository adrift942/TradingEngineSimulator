#pragma once
#include "Ack.h"
#include "OrderUpdate.h"

class IObserver {
public:
    virtual ~IObserver() {};

    virtual void Notify(Ack& ack) = 0;

    virtual void Notify(OrderUpdate& tradeUpdate) = 0;
};