#include "Magazine.h"


Magazine::Magazine(int _ammo, const int _capacity) : 
	ammo(_ammo), capacity(_capacity)
{
}

int* Magazine::getAmmo(void)
{
	return &ammo;
}

const int* Magazine::getCapacity(void)
{
	return &capacity;
}

Magazine::~Magazine(void)
{
}
