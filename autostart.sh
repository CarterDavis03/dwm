#!/bin/bash

xset -dpms
xset s off
sudo timedatectl set-ntp true

# Fix for Android Studio
export _JAVA_AWT_WM_NONREPARENTING=1

exec unclutter -idle 5 &
exec clipit -d -n &
exec wallset -S -a ~/Pictures/Backgrounds/oregairu6.png
exec changekeys &
exec slstatus &
exec flameshot &
exec dunst &
exec playjazz &
