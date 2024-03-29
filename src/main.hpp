#pragma once

#include "Application.hpp"

namespace Diligent
{

class MyApp final : public Application
{
public:
    ~MyApp();
    virtual void ProcessCommandLine(const char* CmdLine) override final;
    virtual void Initialize(const ApplicationInitInfo& InitInfo) override final;
    virtual void Render() override final;
    virtual void Update(double CurrTime, double ElapsedTime) override final;
    virtual const Char* GetApplicationTitle() const override final { return "Diligent Application"; }
};

} // namespace Diligent
