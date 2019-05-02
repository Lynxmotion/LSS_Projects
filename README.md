# LSS-Projects
LSS based small projects (Arduino, Python, etc.)

# Project list

## MD900 [[link]](https://github.com/Lynxmotion/LSS_Projects/blob/master/MD900/MD900.ino)
USes broadcasts (ID # 254) for all commands.
(1) Boots; (2) waits 1 s; (3) sends D0; (4) waits 1 s; (5) sends 12x MD900 (delay=300 ms); (6) sends 12x MD-900 (delay=300 ms).
It repeats steps 5/6 infinitely.
