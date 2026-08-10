#include "movement.h"

void HorseMovement::updateInputPosition()
{
	direction->x = 0;
	direction->y = 0;
	direction->z = 0;

	if (Feel::GetInstance()->mappedKeys.forwardKey->enabled)
	{
		direction->x = 1;
	}
	if (Feel::GetInstance()->mappedKeys.backwardKey->enabled)
	{
		direction->x = -1;
	}
	if (Feel::GetInstance()->mappedKeys.leftKey->enabled)
	{
		direction->z = -1;
	}
	if (Feel::GetInstance()->mappedKeys.rightKey->enabled)
	{
		direction->z = 1;
	}
	
}
