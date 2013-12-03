/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake III Arena source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
//
/*
=======================================================================

MAIN MENU

=======================================================================
*/


#include "ui_local.h"

#define ID_MULTIPLAYER			11
#define ID_SETUP				12
#define ID_EXIT					17

#define	ID_MODEL				18
#define MAX_NAMELENGTH			20

int	multiXpos = 28;
int multiYpos = 119;

int setupXpos = 28;
int setupYpos = 157;

int exitXpos = 28;
int exitYpos = 195;
typedef struct {
	menuframework_s	menu;

	menutext_s		multiplayer;
	menutext_s		setup;
	menufield_s		name;
	menutext_s		exit;
	menubitmap_s	player;
	playerInfo_t	playerinfo;
	char			playerModel[MAX_QPATH];
} mainmenu_t;

static mainmenu_t s_main;
void MainMenu_Save(void);

typedef struct {
	menuframework_s menu;	
	char errorMessage[4096];
} errorMessage_t;

static errorMessage_t s_errorMessage;

/*
=================
MainMenu_DrawPlayer
=================
*/
static void MainMenu_DrawPlayer( void *self ) {
	menubitmap_s	*b;
	vec3_t			viewangles;
	char			buf[MAX_QPATH];

	trap_Cvar_VariableStringBuffer( "model", buf, sizeof( buf ) );
	if ( strcmp( buf, s_main.playerModel ) != 0 ) {
		UI_PlayerInfo_SetModel( &s_main.playerinfo, buf );
		strcpy( s_main.playerModel, buf );

		viewangles[YAW]   = 180 - 30;
		viewangles[PITCH] = 0;
		viewangles[ROLL]  = 0;
		UI_PlayerInfo_SetInfo( &s_main.playerinfo, ANIM_IDLE, ANIM_IDLE, viewangles, vec3_origin, WP_NONE, qfalse );
	}

	b = (menubitmap_s*) self;
	UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, &s_main.playerinfo, uis.realtime / PLAYER_MODEL_SPEED );
}
static void MainMenu_DrawName( void *self ) {
	menufield_s		*f;
	qboolean		focus;
	int				style;
	char			*txt;
	char			c;
	float			*color;
	int				n;
	int				basex, x, y;
	char			name[32];

	f = (menufield_s*)self;
	txt = f->field.buffer;
	basex = (f->generic.right - 8) - (Q_PrintStrlen(txt) + 1) * 6;
	y = f->generic.y;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);
	style = UI_LEFT|UI_SMALLFONT|UI_DROPSHADOW;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = text_color_highlight;
	}
	//basex += 64;
	y += PROP_HEIGHT;
	color = g_color_table[ColorIndex(COLOR_WHITE)];
	x = basex;
	while ( (c = *txt) != 0 ) {
		if ( !focus && Q_IsColorString( txt ) ) {
			n = ColorIndex( *(txt+1) );
			if( n == 0 ) {
				n = 7;
			}
			color = g_color_table[n];
			txt += 2;
			continue;
		}
		UI_DrawChar( x, y, c, style, color );
		txt++;
		x += 6;
	}
	if( focus ) {
		if ( trap_Key_GetOverstrikeMode() ) {
			c = 11;
		} else {
			c = 10;
		}
		UI_DrawChar( basex + f->field.cursor * 6, y, c, style, color_white );
	}
}

/*
=================
MainMenu_ExitAction
=================
*/
static void MainMenu_ExitAction( qboolean result ) {
	if( !result ) {
		return;
	}
	UI_PopMenu();
	UI_CreditMenu();
}



/*
=================
Main_MenuEvent
=================
*/
void Main_MenuEvent (void* ptr, int event) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_MULTIPLAYER:
		MainMenu_Save();
		UI_ArenaServersMenu();
		break;
	case ID_SETUP:
		MainMenu_Save();
		UI_SystemSettingsMenu();
		break;
	case ID_EXIT:
		MainMenu_Save();
		UI_ConfirmMenu( "Quit?", 0, MainMenu_ExitAction );
		break;
	case ID_MODEL:
		MainMenu_Save();
		UI_PlayerModelMenu();
		break;
	}
}


/*
===============
MainMenu_Cache
===============
*/
void MainMenu_Cache( void ) {}
void MainMenu_Save ( void ) {
	trap_Cvar_Set("name", s_main.name.field.buffer );
}



sfxHandle_t ErrorMessage_Key(int key)
{
	trap_Cvar_Set( "com_errorMessage", "" );
	UI_MainMenu();
	return (menu_null_sound);
}

/*
===============
Main_MenuDraw
TTimo: this function is common to the main menu and errorMessage menu
===============
*/

static void Main_MenuDraw( void ) {
	char *result;
	char segment[6400];
	if(uis.menuamount != 3){
		//trap_HTTPGet("http://zeq2.com/lite/forums/");		
		//trap_Cvar_VariableStringBuffer("httpContents",segment,sizeof(segment));
	}
	uis.menuamount = 3;
	uis.hideEarth = qfalse;
	uis.showFrame = qfalse;
	UI_DrawHandlePic(464,390,149,49,uis.blobShadow);
	UI_DrawHandlePic(474,430,132,21,uis.nameArea);
	Menu_Draw(&s_main.menu);
}


/*
===============
UI_MainMenu

The main menu only comes up when not in a game,
so make sure that the attract loop server is down
and that local cinematics are killed
===============
*/
void UI_MainMenu( void ) {
	qboolean teamArena = qfalse;
	int		style = UI_LEFT | UI_DROPSHADOW | UI_TINYFONT;

	trap_S_StopBackgroundTrack();
	trap_S_StartBackgroundTrack("music/menu.ogg", "music/menu.ogg");
	trap_Cvar_Set( "sv_killserver", "1" );

	// set menu cursor to a nice location
	uis.cursorx = 320;
	uis.cursory = 240;

#if MAPLENSFLARES	// JUHOX: reset g_editmode
	trap_Cvar_Set("g_editmode", "0");
#endif

	memset( &s_main, 0 ,sizeof(mainmenu_t) );
	memset( &s_errorMessage, 0 ,sizeof(errorMessage_t) );

	// com_errorMessage would need that too
	MainMenu_Cache();
	
	trap_Cvar_VariableStringBuffer( "com_errorMessage", s_errorMessage.errorMessage, sizeof(s_errorMessage.errorMessage) );
	if (strlen(s_errorMessage.errorMessage))
	{	
		s_errorMessage.menu.draw = Main_MenuDraw;
		s_errorMessage.menu.key = ErrorMessage_Key;
		s_errorMessage.menu.fullscreen = qtrue;
		s_errorMessage.menu.wrapAround = qtrue;
		s_errorMessage.menu.showlogo = qtrue;		

		trap_Key_SetCatcher( KEYCATCH_UI );
		uis.menusp = 0;
		UI_PushMenu ( &s_errorMessage.menu );
		
		return;
	}

	s_main.menu.draw = Main_MenuDraw;
	s_main.menu.fullscreen = qtrue;
	s_main.menu.wrapAround = qtrue;
	s_main.menu.showlogo = qtrue;

	s_main.multiplayer.generic.type			= MTYPE_PTEXT;
	s_main.multiplayer.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.multiplayer.generic.x			= multiXpos;
	s_main.multiplayer.generic.y			= multiYpos;
	s_main.multiplayer.generic.id			= ID_MULTIPLAYER;
	s_main.multiplayer.generic.callback		= Main_MenuEvent; 
	s_main.multiplayer.string				= "PLAY";
	s_main.multiplayer.color				= color_white;
	s_main.multiplayer.style				= style;

	s_main.setup.generic.type				= MTYPE_PTEXT;
	s_main.setup.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.setup.generic.x					= setupXpos;
	s_main.setup.generic.y					= setupYpos;
	s_main.setup.generic.id					= ID_SETUP;
	s_main.setup.generic.callback			= Main_MenuEvent; 
	s_main.setup.string						= "SETUP";
	s_main.setup.color						= color_white;
	s_main.setup.style						= style;

	s_main.exit.generic.type				= MTYPE_PTEXT;
	s_main.exit.generic.flags				= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_main.exit.generic.x					= exitXpos;
	s_main.exit.generic.y					= exitYpos;
	s_main.exit.generic.id					= ID_EXIT;
	s_main.exit.generic.callback			= Main_MenuEvent; 
	s_main.exit.string						= "EXIT";
	s_main.exit.color						= color_white;
	s_main.exit.style						= style;

	s_main.name.generic.type				= MTYPE_FIELD;
	s_main.name.generic.flags				= QMF_NODEFAULTINIT;
	s_main.name.generic.ownerdraw			= MainMenu_DrawName;
	s_main.name.field.widthInChars			= 14;
	s_main.name.field.maxchars				= 14;
	s_main.name.generic.x					= 454;
	s_main.name.generic.y					= 405;
	s_main.name.generic.left				= 474;
	s_main.name.generic.top					= 430;
	s_main.name.generic.right				= 610;
	s_main.name.generic.bottom				= 455;

	s_main.player.generic.type				= MTYPE_BITMAP;
	s_main.player.generic.flags				= QMF_SILENT;
	s_main.player.generic.ownerdraw			= MainMenu_DrawPlayer;
	s_main.player.generic.id				= ID_MODEL;
	s_main.player.generic.callback			= Main_MenuEvent;
	s_main.player.generic.x					= 380;
	s_main.player.generic.y					= -170;
	s_main.player.width						= 320;
	s_main.player.height					= 810;


	Menu_AddItem( &s_main.menu,	&s_main.multiplayer );
	Menu_AddItem( &s_main.menu,	&s_main.setup );
	Menu_AddItem( &s_main.menu,	&s_main.exit );
	Menu_AddItem( &s_main.menu, &s_main.name );
	Q_strncpyz(s_main.name.field.buffer,UI_Cvar_VariableString("name"),sizeof(s_main.name.field.buffer) );
	Menu_AddItem( &s_main.menu, &s_main.player );
	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;
	UI_PushMenu ( &s_main.menu );
}

