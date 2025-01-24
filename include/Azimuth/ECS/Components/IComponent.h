#pragma once

enum GEOMETRY_TYPE
{
    None = 0,
    GEOMETRY_POINT = 1,
    GEOMETRY_LINE = 2,
    GEOMETRY_TRIANGLE = 3,
    GEOMETRY_PLANE = 4,
    GEOMETRY_CUBE = 5,
    GEOMETRY_SPHERE = 6
};

struct IComponent
{
};