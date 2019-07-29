#pragma once

class Bar final
{
public:
    bool IsReady() const noexcept
    {
        return ready_;
    }
private:
    bool ready_ = false;
};