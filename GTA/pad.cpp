#include "stdafx.h"
#include "detour.h"
#include "menu.h"

CellPadData padData[7];



bool IsButtonDown(int button)
{
	const int digitals = (padData[0].button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 16) | padData[0].button[CELL_PAD_BTN_OFFSET_DIGITAL2];

	CellPadInfo2 info;
	if (cellPadGetInfo2(&info) != 0 || (info.port_status[0] & 1) == 0)
		return false;

	return (digitals >> button) & 1;
}

DetourPPC* sys_io_cellpadgetdata;
int sys_io_cellPadGetData(unsigned int port, CellPadData* data)
{
	if (port != 0)
		return sys_io_cellpadgetdata->CallOriginal<int>(port, data);

	int returnValue = sys_io_cellpadgetdata->CallOriginal<int>(port, &padData[port]);
	memcpy(data, &padData[port], sizeof(CellPadData));
	if (g_Menu->IsOpen())
	{
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~(CELL_PAD_CTRL_LEFT | CELL_PAD_CTRL_DOWN | CELL_PAD_CTRL_RIGHT | CELL_PAD_CTRL_UP);
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~(CELL_PAD_CTRL_CROSS | CELL_PAD_CTRL_CIRCLE | CELL_PAD_CTRL_R1 | CELL_PAD_CTRL_L1);
	}
	return returnValue;
}

namespace ps3_gamepad
{
	void start()
	{
		padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X] = 128;
		padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y] = 128;
		padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = 128;
		padData[0].button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] = 128;

		sys_io_cellpadgetdata = new DetourPPC(0x1807A8C, sys_io_cellPadGetData);

	}


	void stop()
	{
		delete sys_io_cellpadgetdata;
	}
}