#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

const std::string listLink = "https://raw.githubusercontent.com/DeceptiveGD/Geometry-Dash-Challenge-Progression/refs/heads/main/data/";
const int levelsPerPage = 10;

enum Role {
    Owner = 0,
    Dev = 1,
    Admin = 2,
    Helper = 3,
    Other = 4
};

const std::unordered_map<std::string, Role> roleMap = {
    {"owner", Role::Owner},
    {"dev", Role::Dev},
    {"admin", Role::Admin},
    {"helper", Role::Helper}
};

const std::unordered_map<Role, std::string> roleColors = {
    {Role::Owner, "r"},
    {Role::Dev, "g"},
    {Role::Admin, "y"},
    {Role::Helper, "j"},
    {Role::Other, "b"}
};

// lamo