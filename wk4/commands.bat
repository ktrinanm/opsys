rem echo off
rem '$*' has to be appended to show something comes after it

doskey ls=dir
doskey rm=del $*
doskey ifconfig=ipconfig
doskey ps=tasklist