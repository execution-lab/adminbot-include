#if defined _ADMINBOT_V2_INC_
	#endinput
#endif

#define _ADMINBOT_V2_INC_

#if __Pawn >= 0x3A && defined __PawnBuild
    #if __Pawn == 0x3A && __PawnBuild >= 2 || __Pawn > 0x3A
        #define static_enum        static enum
    #endif
#endif
#if !defined static_enum
    #define static_enum        enum
#endif

#define	ADMIN_BOT_VERSION					"2.0.1"

#if !defined ADMIN_BOT_HEX_COLOR
	#define ADMIN_BOT_HEX_COLOR					0xFF6347FF 	
#endif

#if !defined MAX_ADMIN_BOT_PLAYER_LISTITEM
	#define MAX_ADMIN_BOT_PLAYER_LISTITEM					25 			
#endif

#define	MAX_ADMIN_BOT_REQUEST 				3		
#define	MAX_ADMIN_BOT_REQUEST_LENGTH		8		
#define	MAX_ADMIN_BOT_SUPPORT_COUNT			8			
#define MIN_ADMIN_BOT_SUPPORT_COUNT			0		
#define	MAX_ADMIN_BOT_PUNISH_REASON			50  	
#define MAX_ADMIN_BOT_PUNISN_NAME_LENGTH	24		
#define ADMIN_BOT_INVALID_REQUEST_ID		MAX_ADMIN_BOT_SUPPORT_COUNT + 1		
#define ADMIN_BOT_INVALID_ADMIN_LEVEL		-1		

#define ADMIN_NAME(%0)						pAdminName[%0]
#define ADMIN_LEVEL(%0)						pAdminLevel[%0]

static_enum
{
	dAB_reqinfo,
	dAB_reqmenu,
	dAB_reqlist 
}

static_enum 
{
	CA_REQUEST_NOT_FOUND,
	CA_REQUEST_FOUND,
	CA_NO_PLACE_IN_REQUEST,
	CA_MISSING_ARGUMENT,
	CA_INVALID_TARGET_ID
}

static_enum e_ADMIN_BOT_INFO 
{
	AB_punish_type,
	AB_punish_time,
	AB_punish_reason[MAX_ADMIN_BOT_PUNISH_REASON + 1],
	AB_targetid,
	AB_adminid,
	AB_unix_time,
}


static
	AdminBotInfo[MAX_ADMIN_BOT_SUPPORT_COUNT][e_ADMIN_BOT_INFO], 
	PlayerListitem[MAX_PLAYERS][MAX_ADMIN_BOT_PLAYER_LISTITEM] 
;

static
	const ADMIN_BOT_PUNISH_NAME[MAX_ADMIN_BOT_REQUEST][MAX_ADMIN_BOT_PUNISN_NAME_LENGTH + 1] =
	{
		"Muffle",
		"Put in admin. the prison",
		"To imprison"
	};

static
	const ADMIN_BOT_SUPPORT_COMMAND[MAX_ADMIN_BOT_REQUEST][MAX_ADMIN_BOT_REQUEST_LENGTH] = 
	{  
		"/mute", 
		"/prison", 
		"/jail"
	};

static
	const ADMIN_BOT_LEVEL_USING_CMD[MAX_ADMIN_BOT_REQUEST] =  
	{
		3,
		3,
		3
	};
	
static
	gADMIN_BOT_SUPPORT_TOTAL_COUNT = 0, 
	pAdminChoseRequest[MAX_PLAYERS char],
	pAdminName[MAX_PLAYERS][MAX_PLAYER_NAME + 1],
	pAdminLevel[MAX_PLAYERS] = {ADMIN_BOT_INVALID_ADMIN_LEVEL, ...}
;

stock AB_SetPlayerAdminLevel(playerid, const level)
{
	pAdminLevel[playerid] = level;
}

stock AB_GetRequestInfo(playerid)
{
	if(pAdminChoseRequest{playerid} == ADMIN_BOT_INVALID_REQUEST_ID)
		return 0;

	new
		count = pAdminChoseRequest{playerid},
		string[MAX_CHATBUBBLE_LENGTH + 1]
	;
	
	
	format(string, sizeof string, "\n\
		{C0F08E}Requested: {FFFFFF}%s\n{ED8473}Punish: {FFFFFF}%s\n\n\
		Type of: %s\n\
		Time: %d мин\n\
		Reason: %s\n\n\n\
		%s",
		ADMIN_NAME(AdminBotInfo[count][AB_adminid]),
		ADMIN_NAME(AdminBotInfo[count][AB_targetid]),
		ADMIN_BOT_PUNISH_NAME[AdminBotInfo[count][AB_punish_type]],
		AdminBotInfo[count][AB_punish_time],
		AdminBotInfo[count][AB_punish_reason],
		ADMIN_BOT_LEVEL_USING_CMD[AdminBotInfo[count][AB_punish_type]] > ADMIN_LEVEL(playerid) 
		? ("{AFAFAF}You can not accept the request") : ("{E2BE1D}You can accept the request")
	);

	return ShowPlayerDialog(
		playerid,
		dAB_reqinfo,
		DIALOG_STYLE_MSGBOX,
		!" ",
		string,
		!"Back", !""
	);
}

stock AB_ShowRequestMenu(playerid)
{
	if(pAdminChoseRequest{playerid} == ADMIN_BOT_INVALID_REQUEST_ID)
		return 0;

	return ShowPlayerDialog(
		playerid,
		dAB_reqmenu,
		DIALOG_STYLE_LIST,
		ADMIN_NAME(AdminBotInfo[pAdminChoseRequest{playerid}][AB_targetid]),
		!"{C0F08E}Punish\n{FFFFFF}Information\Reject", 
		!"Next", !"Back"
	);
}

stock AB_GetRequestListInfo(playerid)
{
	new
		string[(4 + ((MAX_PLAYER_NAME + 1 - 2) * 2) + (- 2 + MAX_ADMIN_BOT_PUNISH_REASON) + (- 2 + MAX_ADMIN_BOT_PUNISN_NAME_LENGTH) 
		+ (- 2 + 4)) * MAX_ADMIN_BOT_SUPPORT_COUNT + 46] = 
		{
			"Requested\Act(time)\Punish\tReason\n"
		},

		count = 0
	;

	for(new iter = 0; iter < MAX_ADMIN_BOT_SUPPORT_COUNT; iter ++)
	{
		if(AB_IsEmptyRequest(iter))
				continue;
			
		format(string, sizeof string, "%s%s\t%s(%d min)\t%s\t%s\n",
			string,
			ADMIN_NAME(AdminBotInfo[iter][AB_adminid]),
			ADMIN_BOT_PUNISH_NAME[AdminBotInfo[iter][AB_punish_type]],
			AdminBotInfo[iter][AB_punish_time],
			ADMIN_NAME(AdminBotInfo[iter][AB_targetid]),
			AdminBotInfo[iter][AB_punish_reason]
		);

		PlayerListitem[playerid][count++] = iter;

	}

	return ShowPlayerDialog(
		playerid,
		dAB_reqlist ,
		DIALOG_STYLE_TABLIST_HEADERS,
		!"Requests admin bot",
		string,
		!"Next", !""
	);
}

stock AB_GetLastRequest()
{
	new
		tmp_ = -1;
	
	if(AB_GetCountRequest() == MIN_ADMIN_BOT_SUPPORT_COUNT)
		return tmp_;

	for(new iter = 0; iter < MAX_ADMIN_BOT_SUPPORT_COUNT; iter ++)
	{
		if(AB_IsEmptyRequest(iter))
				continue;
		
		if((tmp_ == -1) || (tmp_ != -1 && AdminBotInfo[iter][AB_unix_time] > AdminBotInfo[tmp_][AB_unix_time]))
		{
			tmp_ = iter;
		}
	}
	
	return tmp_;
}

stock AB_GetCountRequest()
{
	return gADMIN_BOT_SUPPORT_TOTAL_COUNT;
}

stock AB_CheckAvailability(playerid, params[])
{
	new
		bool:IsFinedCommand = false,

        strFindPos,

		supportCommand[MAX_ADMIN_BOT_REQUEST_LENGTH + 1],
		supportTargetid,
		supportTime,
		supportReason[MAX_ADMIN_BOT_PUNISH_REASON + 1],
		supportUnixtTime = gettime(),

		string[MAX_CHATBUBBLE_LENGTH + 1]
	;

	for(new i = 0; i < MAX_ADMIN_BOT_REQUEST; ++i)
	{
        if((strFindPos = strfind(params, ADMIN_BOT_SUPPORT_COMMAND[i], true)) != -1)
        {
			if(AB_GetCountRequest() >= MAX_ADMIN_BOT_SUPPORT_COUNT)
				return CA_NO_PLACE_IN_REQUEST;

            strdel(params, 0, strFindPos);

			if(sscanf(params, "s[8]uds[50]",
				supportCommand, supportTargetid, supportTime, supportReason))
				return CA_MISSING_ARGUMENT;
			
			if(supportTargetid == INVALID_PLAYER_ID)
				return CA_INVALID_TARGET_ID;
		

			for(new j = 0; j < MAX_ADMIN_BOT_SUPPORT_COUNT; j ++)
			{
				if(!AB_IsEmptyRequest(j))
					continue;
				
				format(string, sizeof string, "[ADMIN-BOT] {FFFFFF}%s requested %s player %s for %d min, reason: %s",
					ADMIN_NAME(playerid), 
					ADMIN_BOT_PUNISH_NAME[i], 
					ADMIN_NAME(supportTargetid),
					supportTime,
					supportReason
				);
				SendAdminMessage(ADMIN_BOT_HEX_COLOR, string);

				SendAdminMessage(ADMIN_BOT_HEX_COLOR, !"Press Y to approve, N to refuse. (/viewreq to view all requests)");

				AB_AddRequest(
					j, 
					i, 
					supportTime, 
					playerid, 
					supportTargetid, 
					supportUnixtTime, 
					supportReason
				);

				return CA_REQUEST_FOUND;
			}
			
			return CA_REQUEST_NOT_FOUND;
		}
	}
}

stock AB_RejectRequest(playerid, index)
{
	new
		string[MAX_CHATBUBBLE_LENGTH + 1];

	format(string, sizeof string, "[ADMIN-BOT] {FFFFFF}%s rejected the request №%d",
		ADMIN_NAME(playerid), index + 1);
	SendAdminMessage(ADMIN_BOT_HEX_COLOR, string);

	format(string, sizeof string, "Sender: %s | Request: %s | Punishable: %s | Reason: %s)",
		ADMIN_NAME(AdminBotInfo[index][AB_targetid]),
		ADMIN_BOT_PUNISH_NAME[AdminBotInfo[index][AB_punish_type]],
		ADMIN_NAME(AdminBotInfo[index][AB_adminid]),
		AdminBotInfo[index][AB_punish_reason]
	);
	SendAdminMessage(ADMIN_BOT_HEX_COLOR, string);

	AB_DeleteRequest(index);
}

stock AB_ApproveRequest(playerid, index)
{
	new
		string[MAX_CHATBUBBLE_LENGTH + 1];

	format(string, MAX_CHATBUBBLE_LENGTH + 1, "%s %d %d %s // %s",
			ADMIN_BOT_SUPPORT_COMMAND[AdminBotInfo[index][AB_punish_type]],
			AdminBotInfo[index][AB_targetid],
			AdminBotInfo[index][AB_punish_time],
			AdminBotInfo[index][AB_punish_reason],
			ADMIN_NAME(AdminBotInfo[index][AB_adminid])
	);
	PC_EmulateCommand(playerid, string);

	AB_DeleteRequest(index);
}

stock AB_IsEmptyRequest(index)
{
	return ((AdminBotInfo[index][AB_punish_reason][0] == '\0') ? (1) : (0));
}

stock AB_DeleteRequest(index)
{
	if(!AB_IsEmptyRequest(index))
	{
		AdminBotInfo[index][AB_targetid] = INVALID_PLAYER_ID;
		AdminBotInfo[index][AB_punish_reason][0] = '\0';

		gADMIN_BOT_SUPPORT_TOTAL_COUNT --;

		return 1;
	}
	else
	{
		return 0;
	}
}

stock AB_AddRequest(index, type, &time, &adminid, &targetid, &unix_time, reason[], const size = sizeof reason)
{
	AdminBotInfo[index][AB_punish_type] = type;
	AdminBotInfo[index][AB_punish_time] = time;

	strmid(
		AdminBotInfo[index][AB_punish_reason],
		reason,
		0,
		MAX_ADMIN_BOT_PUNISH_REASON,
		size
	);
	
	AdminBotInfo[index][AB_targetid] = targetid;
	AdminBotInfo[index][AB_adminid] = adminid;
	AdminBotInfo[index][AB_unix_time] = unix_time;

	gADMIN_BOT_SUPPORT_TOTAL_COUNT ++;
}

public OnPlayerConnect(playerid)
{
	pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID;

	GetPlayerName(playerid, pAdminName[playerid], MAX_PLAYER_NAME);

	#if defined AB_OnPlayerConnect
		return AB_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch(dialogid)
	{
		case dAB_reqinfo: 
		{ 
			return AB_GetRequestMenu(playerid); 
		} 

		case dAB_reqmenu: 
		{ 
			if(!response) 
			{ 
				pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID; 

				return 1; 
			} 

			if(AB_IsEmptyRequest(pAdminChoseRequest{playerid})) 
				return SendClientMessage(playerid, -1, !"This request has been deleted."); 
			
			new 
				count = pAdminChoseRequest{playerid}; 

			switch(listitem) 
			{ 
				case 0: 
				{ 
					if(ADMIN_BOT_LEVEL_USING_CMD[AdminBotInfo[count][AB_punish_type]] > ADMIN_LEVEL(playerid)) 
					{ 
						pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID; 

						return SendClientMessage(playerid, -1, !"You are not authorized to accept this request."); 
					} 
					
					pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID; 

					AB_ApproveRequest(playerid, count); 

					return 1; 
				} 
				case 1: 
				{ 
					return AB_GetRequestInfo(playerid); 
				} 
				case 2: 
				{ 

					if((AdminBotInfo[count][AB_adminid] != playerid)  
					&& (ADMIN_BOT_LEVEL_USING_CMD[AdminBotInfo[count][AB_punish_type]] > ADMIN_LEVEL(playerid))) 
					{ 
						pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID; 

						return SendClientMessage(playerid, -1, !"You are not authorized to reject this request."); 
					} 
					
					pAdminChoseRequest{playerid} = ADMIN_BOT_INVALID_REQUEST_ID; 

					AB_RejectRequest(playerid, count); 

					return 1; 
				} 
			} 
			return 1; 
		} 

		case dAB_reqlist : 
		{ 
			if(!response) 
				return 1; 
			
			new 
				count = PlayerListitem[playerid][listitem]; 

			if(AB_IsEmptyRequest(count)) 
			{ 
				SendClientMessage(playerid, -1, !"This request has been deleted."); 
				return AB_GetRequestList(playerid); 
			} 

			pAdminChoseRequest{playerid} = count; 
			
			return AB_GetRequestMenu(playerid); 
		}  
	}

	#if defined AB_OnDialogResponse
		return AB_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
	#else
		return 1;
	#endif
}

public OnPlayerDisconnect(playerid, reason)
{
	pAdminLevel[playerid] = ADMIN_BOT_INVALID_ADMIN_LEVEL;

	if(AB_GetCountRequest() != MIN_ADMIN_BOT_SUPPORT_COUNT) 
    { 
        for(new iter = 0; iter < MAX_ADMIN_BOT_SUPPORT_COUNT; iter ++) 
        { 
            if(AdminBotInfo[iter][AB_adminid] != playerid && AdminBotInfo[iter][AB_targetid] != playerid) 
                continue; 
             
            AB_DeleteRequest(iter); 
        } 
    }  

	#if defined AB_OnPlayerDisconnect
		return AB_OnPlayerDisconnect(playerid, reason);
	#else
		return 1;
	#endif
}

public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(newkeys & KEY_NO) 
	{ 
		if(AB_GetCountRequest() != MIN_ADMIN_BOT_SUPPORT_COUNT) 
		{ 
			if(ADMIN_LEVEL(playerid)) 
			{ 
				new 
					iter = AB_GetLastRequest(); 

				if(iter != -1) 
				{ 
					if(ADMIN_BOT_LEVEL_USING_CMD[AdminBotInfo[iter][AB_punish_type]] > ADMIN_LEVEL(playerid)) 
					{ 
						SendClientMessage(playerid, -1, !"You are not authorized to reject this request."); 

						return 1; 
					} 
					
					AB_RejectRequest(playerid, iter); 

					return 1; 
				} 
			} 
		} 
	} 

	if(newkeys & KEY_YES) 
	{ 
		if(AB_GetCountRequest() != MIN_ADMIN_BOT_SUPPORT_COUNT) 
		{ 
			if(ADMIN_LEVEL(playerid)) 
			{ 
				new 
					iter = AB_GetLastRequest(); 

				if(iter != -1) 
				{     
					if(ADMIN_BOT_LEVEL_USING_CMD[AdminBotInfo[iter][AB_punish_type]] > ADMIN_LEVEL(playerid)) 
					{ 
						SendClientMessage(playerid, -1, !"You are not authorized to accept this request."); 

						return 1; 
					} 
						
					AB_ApproveRequest(playerid, iter); 
				} 
			} 
		} 
	}

	#if defined AB_OnPlayerKeyStateChange
		return AB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
	#else
		return 1;
	#endif
}

#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif
#define OnPlayerConnect AB_OnPlayerConnect
#if defined AB_OnPlayerConnect
	forward AB_OnPlayerConnect(playerid);
#endif


#if defined _ALS_OnDialogResponse
	#undef OnDialogResponse
#else
	#define _ALS_OnDialogResponse
#endif
#define OnDialogResponse AB_OnDialogResponse
#if defined AB_OnDialogResponse
	forward AB_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
#endif


#if defined _ALS_OnPlayerDisconnect
	#undef OnPlayerDisconnect
#else
	#define _ALS_OnPlayerDisconnect
#endif
#define OnPlayerDisconnect AB_OnPlayerDisconnect
#if defined AB_OnPlayerDisconnect
	forward AB_OnPlayerDisconnect(playerid, reason);
#endif


#if defined _ALS_OnPlayerKeyStateChange
	#undef OnPlayerKeyStateChange
#else
	#define _ALS_OnPlayerKeyStateChange
#endif
#define OnPlayerKeyStateChange AB_OnPlayerKeyStateChange
#if defined AB_OnPlayerKeyStateChange
	forward AB_OnPlayerKeyStateChange(playerid, newkeys, oldkeys);
#endif