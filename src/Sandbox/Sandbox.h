#pragma once

#include <Vengine/Vengine.h>
#include <iostream>

class Sandbox : public Vengine::Layer
{
    virtual void OnStart() const override;
    virtual void OnUpdate() const override;
};