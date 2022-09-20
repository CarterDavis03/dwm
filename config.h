/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx = 1; /* border pixel of windows */
static const unsigned int systraypinning = 0; /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1; /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2; /* systray spacing */
static const int systraypinningfailfirst = 1; /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray = 1;    /* 0 means no systray */
static const unsigned int snap = 32; /* snap pixel */
static const int swallowfloating = 0; /* 1 means swallow floating windows by default */
static const int showbar = 1; /* 0 means no bar */
static const int topbar = 1;  /* 0 means bottom bar */
static const char *fonts[] = {"JetBrains Mono Nerd Font:size=16", "Noto Color Emoji:pixelsize=16:antialias=true:autohint=true", "FreeSans:size=16", "IPAGothic:size=16"}; /*Size 16 for 1440p*/
static const char dmenufont[] = "JetBrains Mono Nerd Font:size=16";
static const char col_gray1[] = "#2b2022";
static const char col_gray2[] = "#444444";
static const char col_gray3[] = "#E6CDB3";
static const char col_gray4[] = "#E6CDB3";
/* static const char col_cyan[]        = "#005577"; */
static const char col_cyan[] = "#BA7988";
static const char *colors[][3] = {
    /*               fg         bg         border   */
    [SchemeNorm] = {col_gray3, col_gray1, col_gray2},
    [SchemeSel] = {col_gray4, col_cyan, col_cyan},
};

/* tagging */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8"};

static const Rule rules[] = {
    /* xprop(1):
     *	WM_CLASS(STRING) = instance, class
     *	WM_NAME(STRING) = title
     */
    /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
    {"Gimp", NULL, NULL, 0, 1, 0, 0, -1},
    {"st", NULL, NULL, 0, 0, 1, 0, -1},
    {"zathura", NULL, NULL, 0, 0, 0, 1, -1},
    {NULL, "zathura", NULL, 0, 0, 0, 1, -1},
    {NULL, NULL, "Event Tester", 0, 0, 0, 1, -1}, /* xev */
    {"Teams", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {"Steam", NULL, NULL, 1 << 6, 0, 0, 0, -1},
    {"Microsoft Teams", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {"Discord", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {"discord", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {NULL, NULL, "Authenticate", 0, 1, 0, 0, -1},
    {"Spotify", NULL, NULL, 1 << 7, 0, 0, 0, -1},
    {NULL, NULL, "PyPCSleepTool", 0, 1, 0, 1, -1},

};

/* layout(s) */
static const float mfact = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster = 1;    /* number of clients in master area */
static const int resizehints =
    0; /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "grid.c"
static const Layout layouts[] = {
    /* symbol     arrange function */
    {"[]=", dwindle}, /* first entry is default */
    {"><>", NULL},    /* no layout function means floating behavior */
    {"[M]", monocle}, {"[@]", spiral}, {"[\\]", tile},
	{ "HHH",      grid },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd)                                                             \
  {                                                                            \
    .v = (const char *[]) { "usr/bin/st", "-c", cmd, NULL }                    \
  }

/* commands */
static char dmenumon[2] =
    "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL};
static const char *termcmd[] = {"st", NULL};
static const char *openchromium[] = {"firefox", NULL};
static const char *openranger[] = {"st", "ranger", NULL};
static const char *opensuranger[] = {"st", "sudo", "ranger", "/home/carter/",
                                     NULL};
static const char *blurlock[] = {"blurlock", NULL};
static const char *screenoff[] = {"screenoff", NULL};
static const char *htopopen[] = {"st", "htop", NULL};
static const char *suspend[] = {"suspendpc", NULL};
static const char *rofirun[] = {"rofi", "-show", "drun", "-show-icons", NULL};
static const char *opennvim[] = {"st", "nvim", NULL};
static const char *openpulsemixer[] = {"st", "pulsemixer", NULL};
static const char *kblock[] = {"xtrlock", NULL};
static const char *audiomute[] = {"pactl", "set-sink-mute", "@DEFAULT_SINK@",
                                  "toggle", NULL};
static const char *audioup[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@",
                                "+1%", NULL};
static const char *audiodown[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK@",
                                  "-1%", NULL};
static const char *screenshot[] = {"flameshot", "full", "-p", "/home/carter/Pictures/Screenshots/", NULL};
static const char *screenshotregion[] = {"flameshot", "gui", NULL};
static const char *connectairpods[] = {"bluetoothctl", "connect", "D4:68:AA:88:A7:63", NULL};
static const char *disconnectairpods[] = {"bluetoothctl", "disconnect", "D4:68:AA:88:A7:63", NULL};
static const char *emojiselector[] = {"dmenuunicode", NULL};
static const char *picom[] = {"picom", NULL};
static const char *killpicom[] = {"pkill", "picom", NULL};
static const char *setenglish[] = {"setxkbmap", "gb", NULL};
static const char *sethebrew[] = {"setxkbmap", "il", NULL};


static Key keys[] = {
    /* modifier                     key        function        argument */
    {MODKEY, XK_p, spawn, {.v = picom}},
    {MODKEY | ShiftMask, XK_F7, spawn, {.v = sethebrew}},
    {MODKEY | ShiftMask, XK_F6, spawn, {.v = setenglish}},
    {MODKEY | ShiftMask, XK_p, spawn, {.v = killpicom}},
    {MODKEY | ShiftMask, XK_e, spawn, {.v = emojiselector}},
    {NULL, XK_F9, spawn, {.v = connectairpods}},
    {ShiftMask, XK_F9, spawn, {.v = disconnectairpods}},
    {MODKEY, XK_Print, spawn, {.v = screenshot}},
    {MODKEY | ShiftMask, XK_Print, spawn, {.v = screenshotregion}},
    {NULL, XF86XK_AudioMute, spawn, {.v = audiomute}},
    {NULL, XF86XK_AudioRaiseVolume, spawn, {.v = audioup}},
    {NULL, XF86XK_AudioLowerVolume, spawn, {.v = audiodown}},
    {MODKEY, XK_minus, spawn, {.v = audiodown}},
    {MODKEY, XK_equal, spawn, {.v = audioup}},
    {MODKEY, XK_F12, spawn, {.v = kblock}},
    {MODKEY, XK_F1, spawn, {.v = opennvim}},
    {MODKEY, XK_F4, spawn, {.v = openpulsemixer}},
    {MODKEY, XK_9, spawn, {.v = blurlock}},
    {MODKEY, XK_0, spawn, {.v = suspend}},
    {MODKEY | ShiftMask, XK_0, spawn, {.v = screenoff}},
    {ControlMask | ShiftMask, XK_Escape, spawn, {.v = htopopen}},
    {MODKEY | ShiftMask, XK_F3, spawn, {.v = opensuranger}},
    {MODKEY, XK_F3, spawn, {.v = openranger}},
    {MODKEY, XK_e, spawn, {.v = openranger}},
    {MODKEY, XK_F2, spawn, {.v = openchromium}},
    {MODKEY,  XK_d,  spawn,  {.v = dmenucmd } },
    {MODKEY, XK_Return, spawn, {.v = termcmd}},
    {MODKEY, XK_b, togglebar, {0}},
    {MODKEY, XK_j, focusstack, {.i = +1}},
    {MODKEY, XK_k, focusstack, {.i = -1}},
    {MODKEY, XK_i, incnmaster, {.i = +1}},
    {MODKEY, XK_o, incnmaster, {.i = -1}},
    {MODKEY, XK_h, setmfact, {.f = -0.05}},
    {MODKEY, XK_l, setmfact, {.f = +0.05}},
    {MODKEY | ShiftMask, XK_Return, zoom, {0}},
    {MODKEY, XK_Tab, view, {0}},
    {MODKEY, XK_c, killclient, {0}},
    {MODKEY, XK_t, setlayout, {.v = &layouts[0]}},
    {MODKEY, XK_f, setlayout, {.v = &layouts[1]}},
    {MODKEY, XK_m, setlayout, {.v = &layouts[2]}},
    {MODKEY, XK_r, setlayout, {.v = &layouts[3]}},
    {MODKEY, XK_g, setlayout, {.v = &layouts[5]}},
    {MODKEY | ShiftMask, XK_r, setlayout, {.v = &layouts[4]}},
    {MODKEY, XK_space, setlayout, {0}},
    {MODKEY | ShiftMask, XK_space, togglefloating, {0}},
    {MODKEY | ShiftMask, XK_f, togglefullscr, {0}},
    {MODKEY, XK_comma, focusmon, {.i = -1}},
    {MODKEY, XK_period, focusmon, {.i = +1}},
    {MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},
    {MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6)
            TAGKEYS(XK_8, 7){MODKEY | ShiftMask, XK_q, quit, {0}},
    {MODKEY | ControlMask | ShiftMask, XK_q, quit, {1}},
};


    /*{MODKEY, XK_d, spawn, {.v = rofirun}},*/
    /*{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
    { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },*/

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function argument
     */
    {ClkLtSymbol, 0, Button1, setlayout, {0}},
    {ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
    {ClkWinTitle, 0, Button2, zoom, {0}},
    {ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
    {ClkClientWin, MODKEY, Button1, movemouse, {0}},
    {ClkClientWin, MODKEY, Button2, togglefloating, {0}},
    {ClkClientWin, MODKEY, Button3, resizemouse, {0}},
    { ClkTagBar,            0,              Button1,        view, {0} },
    {ClkTagBar, 0, Button3, toggleview, {0}},
    {ClkTagBar, MODKEY, Button1, tag, {0}},
    {ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

