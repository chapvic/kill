CC=cl /nologo /c
LD=link /nologo
RM=del /Q

OBJ=kill.obj
APP=$(OBJ:.obj=.exe)

CFLAGS=/MT /GS /DNDEBUG /DUNICODE /D_UNICODE /Fe:$(APP)
LDFLAGS=/RELEASE /VERSION:1.0 /SUBSYSTEM:CONSOLE

LIBS=kernel32.lib

all:	$(APP)

$(APP):	$(OBJ)
	$(LD) $(LDFLAGS) $(OBJ) $(LIBS)

.c.obj::
	$(CC) $(CFLAGS) $<

clean:
	-$(RM) $(OBJ) > nul 2>&1
	-$(RM) $(APP) > nul 2>&1
