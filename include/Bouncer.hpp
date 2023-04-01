#pragma once

namespace bol
{
    struct Bouncer
    {
        bool hit = false;
        bool isActivated = false;
        float activatedTime = 0.f;
    };
}