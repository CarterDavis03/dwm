#!/bin/bash

xset -dpms
xset s off

# Fix for Android Studio
export _JAVA_AWT_WM_NONREPARENTING=1

exec dbus-update-activation-environment DISPLAY &
exec unclutter -idle 5 &
exec clipit -d -n &
exec ~/.fehbg &
exec changekeys &
exec slstatus &
exec flameshot &
exec dunst &
exec solaar -d -w hide &
exec pactl load-module module-switch-on-connect &
exec blueman-applet &
exec /usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &
exec xidlehook --not-when-fullscreen --not-when-audio --timer 300 blurlock '' &
exec setxkbmap us &
exec picom &
