VERSION = 53
REVISION = 1

.macro DATE
.ascii "3.7.2022"
.endm

.macro VERS
.ascii "powerpacker.library 53.1"
.endm

.macro VSTRING
.ascii "powerpacker.library 53.1 (3.7.2022)"
.byte 13,10,0
.endm

.macro VERSTAG
.byte 0
.ascii "$VER: powerpacker.library 53.1 (3.7.2022)"
.byte 0
.endm
