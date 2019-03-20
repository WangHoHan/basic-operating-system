#pragma once

short int blockToPhysical(short int block, short int byte)
{
	if (block > 31 || block < 0 || byte < 0 || byte > 31)
		return -1;
	else
	{
		return (block * 32) + byte;
	}
}

short int* physicalToBlock(short int byte)
{
	if (byte < 0 || byte > 1023)
		return new short int[1] {-1};
	else
	{
		short int block = byte / 32;
		short int bytes = byte % 32;
		return new short int[2] {block, bytes};
	}
}