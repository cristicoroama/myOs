@echo off
echo Organizez fisierele MyOS...

mkdir boot 2>nul
mkdir kernel 2>nul
mkdir include 2>nul

move start.S boot\ 2>nul
move irq.S boot\ 2>nul

move uart.c kernel\ 2>nul
move printf.c kernel\ 2>nul
move irq.c kernel\ 2>nul
move mem.c kernel\ 2>nul
move kernel.c kernel\ 2>nul

move uart.h include\ 2>nul
move printf.h include\ 2>nul
move irq.h include\ 2>nul
move mem.h include\ 2>nul

echo.
echo Gata! Structura finala:
echo.
tree /F
pause
