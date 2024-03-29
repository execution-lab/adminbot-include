# adminbot-include
Assistant for administration on samp-sevrers.

Used command processes Pawn.CMD.
It is advisable to use foreach.

# Function

**Specify admin level**

```cpp
B_SetPlayerAdminLevel(playerid, const level)
```

**Add Request**

```cpp
AB_AddRequest(index, type, &time, &adminid, &targetid, &unix_time, reason[], const size = sizeof reason)
```

**Delete Request**

```cpp
AB_DeleteRequest(index)
```

**Approve request**

```cpp
AB_ApproveRequest(playerid, index)
```

**Reject Request**

```cpp
AB_RejectRequest(playerid, index)
```

**Check Availability**

```cpp
AB_CheckAvailability(playerid, params[])
```

**Get the number of requests**

```cpp
AB_GetCountRequest()
```

**Get the latest request**

```cpp
AB_GetLastRequest()
```

**Get information about the request**

```cpp
AB_GetRequestListInfo(playerid)
```

**Show menu with queries**

```cpp
AB_ShowRequestMenu(playerid)
```

# Examples
```cpp
#include <a_samp>
#include <adminbot>

main() {}

public OnPlayerConnect(playerid)
{
	/*
		If the player is an administrator, indicate his level
	*/
	AB_SetPlayerAdminLevel(playerid, level);

	return 1;
}
```