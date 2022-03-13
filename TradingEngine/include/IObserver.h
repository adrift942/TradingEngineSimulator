#pragma once
#include "Ack.h"
#include "OrderUpdate.h"
#include "Trade.h"

class IObserver {
public:
    virtual ~IObserver() {};

    virtual void Notify(const Ack& ack) = 0;

    virtual void Notify(const OrderUpdate& tradeUpdate) = 0;

    virtual void Notify(const Trade& tradeUpdate) = 0;
};