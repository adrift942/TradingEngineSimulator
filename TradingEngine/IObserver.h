#pragma once
#include "Ack.h"
#include "OrderUpdate.h"

class IObserver {
public:
    virtual ~IObserver() {};

    virtual void Notify(const Ack& ack) = 0;

    virtual void Notify(const OrderUpdate& tradeUpdate) = 0;
};