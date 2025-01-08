#pragma once

#include <Azimuth/Azimuth.h>
#include <iostream>

using namespace Azimuth;

class Sandbox : public Azimuth::Layer
{
public:
    virtual void OnStart() override;
    virtual void OnUpdate() override;
};