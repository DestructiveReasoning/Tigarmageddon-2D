#pragma once
class Magazine
{
public:
	Magazine(int ammo, const int capacity);
	~Magazine(void);

	int* getAmmo(void);
	void setAmmo(int _ammo);
	const int* getCapacity(void);

private:
	int ammo;
	const int capacity;
	//bool full;
};

