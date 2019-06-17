# adminbot-include
Assistant for administration on samp-sevrers

# Function

**Add Request**

`AB_AddRequest(index, type, &time, &adminid, &targetid, &unix_time, reason[], const size = sizeof reason)`

**Delete Request**

`AB_DeleteRequest(index)`

**Approve request**

`AB_ApproveRequest(playerid, index)`

**Reject Request**

`AB_RejectRequest(playerid, index)`

**Check Availability**

`AB_CheckAvailability(playerid, params[])`

**Get the number of requests**

`AB_GetCountRequest()`

**Get the latest request**

`AB_GetLastRequest()`

**Get information about the request**

`AB_GetRequestListInfo(playerid)`

**Show menu with queries**

`AB_ShowRequestMenu(playerid)`