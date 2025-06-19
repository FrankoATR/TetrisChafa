@echo off
cd /d "%~dp0"
powershell -ExecutionPolicy Bypass -NoExit -Command ".\.venv\Scripts\Activate.ps1"
