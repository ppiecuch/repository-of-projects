
#ifndef __MONITOR_H__
#define __MONITOR_H__


static HMONITOR hMonitor[MAX_MONITORS];

void GetMonitorRect( LPRECT lprc, HMONITOR hMonitor)
{
	MONITORINFO mi;
	RECT        rc;

	mi.cbSize = sizeof( mi );
	::GetMonitorInfo( hMonitor, &mi );
	rc = mi.rcMonitor;

	::SetRect( lprc, rc.left, rc.top, rc.right, rc.bottom );
}

int GetMonitorCount()
{ 
	return ::GetSystemMetrics(SM_CMONITORS);
}

BOOL CALLBACK AddMonitorsCallBack( HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	static int imonitors = 0;

	HMONITOR* pHM = (HMONITOR*)dwData;
	pHM[imonitors++] = (HMONITOR)hMonitor;

	return TRUE;
}

HMONITOR GetMonitor( const int index )
{
	return hMonitor[index];
}

// returns the primary monitor
HMONITOR GetPrimaryMonitor()
{
	//the primary monitor always has its origin at 0,0
	HMONITOR hMonitor = GetMonitor(0);// = ::MonitorFromPoint( CPoint( 0,0 ), MONITOR_DEFAULTTOPRIMARY );

	return hMonitor;
}

void InitMonitorInfo()
{
	::EnumDisplayMonitors( NULL, NULL, AddMonitorsCallBack, (LPARAM)&hMonitor );
}

#endif // __MONITOR_H__

