#pragma once

struct dynamic::empty
{
};

bool operator==(dynamic::empty, dynamic::empty);
bool operator<(dynamic::empty, dynamic::empty);
std::ostream &operator<<(std::ostream &os, dynamic::empty);
