#include "Entity.h"
#include "Geom.h"

Entity::Entity(Memory *memory_p, uint32_t address_p) {
	memory = memory_p;
	address = address_p;
	name = memory->ReadString(address + offsets::entity_name, 16);
	team = memory->Read<int>(address + offsets::entity_team, 4);
}

Vec3 Entity::getHeadPos() {
	return memory->ReadVec3(address + offsets::entity_head);
}

int Entity::getHealth() {
	return memory->Read<uint32_t>(address + offsets::entity_health);
}


Vec3 Entity::getFeetPos() {
	return memory->ReadVec3(address + offsets::entity_feet);
}

void Entity::setYaw(float yaw) {
	memory->Write(address + offsets::entity_yaw, yaw);
}

void Entity::setPitch(float pitch) {
	memory->Write(address + offsets::entity_pitch, pitch);
}

