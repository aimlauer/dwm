/* See LICENSE file for copyright and license details. */
#include "tcl.c"

/* appearance */
static const char *fonts[] = {
	"fixed:regular:size=10" /* menu font */
};
static const char dmenufont[]       = "fixed:size=12"; /* dmenu_run font*/
/* GOLD COLORS*/
/*static const char normbordercolor[] = "#a48623";*/
/*static const char selbordercolor[]  = "#e8b923";*/
/*fgcolor[] = #d4af37*/
// static const char normbordercolor[] = "#323232";
// static const char selbordercolor[]  = "#a95f70";
// static const char normbgcolor[]     = "#181818";
// static const char normfgcolor[]     = "#8f8f8f";
// static const char selbgcolor[]      = "#434343";
// static const char selfgcolor[]      = "#ffffff";
// static const char normmarkcolor[]   = "#000000";
static const char selmarkcolor[]    = "#000000";
static const char normbordercolor[] = "#323232";
static const char normmarkcolor[]   = "#000000";
static const char normbgcolor[]     = "#181818";
static const char normfgcolor[]     = "#8f8f8f";
static const char selbordercolor[]  = "#3380c2";
static const char selbgcolor[]      = "#434343";
static const char selfgcolor[]      = "#ffffff";
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 0;        /* 0 means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title      tags mask     isfloating   monitor */
	{ "thunderbird",  NULL,       NULL,  1 << 2,       0,           -1 },
	{ "Gimp",         NULL,       NULL,  0,            1,           -1 },
	{ "Firefox",      NULL,       NULL,  1 << 0,       0,           -1 },
	{ "Chromium",     NULL,       NULL,  1 << 1,       0,           -1 },
	{ "Thunar",       NULL,       NULL,  0,            1,           -1 },
	{ "Ristretto",    NULL,       NULL,  0,            0,           -1 },
	{ "Tor",          NULL,       NULL,  0,            0,           -1 },
	{ "Pcmanfm",      NULL,       NULL,  0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */



/* commands */
// static const char *mail[] = { "thunderbird", NULL };
static const char *upvol[] = { "amixer", "-q", "set", "Master", "5%+", NULL };
static const char *downvol[] = { "amixer", "-q", "set", "Master", "5%-", NULL };
/* for muting/unmuting */
static const char *mute[] = { "amixer", "-q", "set", "Master", "toggle", NULL };
/* brightness up/down */
static const char *brightness_up[] =   { "xbacklight" ,"-inc" ,"10", NULL }; 
static const char *brightness_down[]  = { "xbacklight", "-dec", "10", NULL };
/* xscreensaver lock */
static const char *screensaver[] = { "xscreensaver-command","-lock",NULL };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
  { "|||",      tcl },

};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "xfce4-terminal", NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_w,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Up,     spawn,          {.v = upvol } },
	{ MODKEY|ShiftMask,             XK_Down,   spawn,          {.v = downvol } },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          {.v = mute } },
	{ MODKEY|ShiftMask,             XK_l,   spawn, {.v = screensaver } },
	{ MODKEY|ShiftMask,             XK_b,   spawn, {.v = brightness_up } },
	{ MODKEY|ShiftMask,             XK_v, spawn, {.v = brightness_down } },
	//{ 0,                            XF86XK_Mail,    spawn, {.v = mail } },
	{ MODKEY,                       XK_Return, swapclient,     {0} },
	{ MODKEY,                       XK_semicolon, togglemark,  {0} }, 
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

