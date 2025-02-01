#version 460 core
layout (location = 0) out int EntityID; 

in flat int Entity;

void main() {
     EntityID = int(Entity);
}