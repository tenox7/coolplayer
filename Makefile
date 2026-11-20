# Makefile - CoolPlayer 2.19 for Windows NT 4.0: x86, MIPS, Alpha and PowerPC,
# and ARM (PINT 4.0)
#
#   nmake          builds coolplayer.exe for the CPU of this machine
#   nmake clean    deletes what nmake made
#
# Visual C++ 4.x and the Win32 SDK of 1996. For ARM: the PINT 4.0 SDK
# (Microsoft C for ARM); it takes the -Ox branch below.
# Object files don't depend on header files here: after you change a header,
# run "nmake clean" first.

# Visual C++ 4.x for MIPS crashes and makes wrong code with -Og (its global
# optimiser), so MIPS builds use -Og-.
# Alpha traps on 0/0, NaN and denormal numbers, where x86, MIPS and PowerPC
# give the IEEE result. -QAieee1 marks the floating-point instructions for
# software completion, so that NT can give the IEEE result on Alpha too.
!IF "$(PROCESSOR_ARCHITECTURE)" == "MIPS"
OPT = -Ox -Og-
!ELSEIF "$(PROCESSOR_ARCHITECTURE)" == "ALPHA"
OPT = -Ox -QAieee1
!ELSE
OPT = -Ox
!ENDIF

CFLAGS = -nologo -c -MT $(OPT) -DWIN32 -DNDEBUG

CP = coolplayer\about.obj coolplayer\bitmap2region.obj coolplayer\CLV_ListView.obj \
	coolplayer\CompositeFile.obj coolplayer\CPI_CircleBuffer.obj \
	coolplayer\CPI_Equaliser_Basic.obj coolplayer\CPI_ID3_Genres.obj \
	coolplayer\CPI_Image.obj coolplayer\CPI_Indicators.obj coolplayer\CPI_Interface.obj \
	coolplayer\CPI_InterfacePart.obj coolplayer\CPI_InterfacePart_CommandButton.obj \
	coolplayer\CPI_InterfacePart_Indicator.obj coolplayer\CPI_Keyboard.obj \
	coolplayer\CPI_Player.obj coolplayer\CPI_Player_Callbacks.obj \
	coolplayer\CPI_Player_CoDec_MPEG.obj coolplayer\CPI_Player_CoDec_OGG.obj \
	coolplayer\CPI_Player_CoDec_WAV.obj coolplayer\CPI_Player_CoDec_WinAmpPlugin.obj \
	coolplayer\CPI_Player_Engine.obj coolplayer\CPI_Player_FileAssoc.obj \
	coolplayer\CPI_Player_Output_File.obj coolplayer\CPI_Player_Output_Wave.obj \
	coolplayer\CPI_Playlist.obj coolplayer\CPI_Playlist_Callbacks.obj \
	coolplayer\CPI_PlaylistItem.obj coolplayer\CPI_PlaylistWindow.obj \
	coolplayer\CPI_Stream.obj coolplayer\CPI_Stream_Internet.obj \
	coolplayer\CPI_Stream_LocalFile.obj coolplayer\CPI_Verbs.obj coolplayer\CPSK_Skin.obj \
	coolplayer\DLG_Find.obj coolplayer\main.obj coolplayer\options.obj \
	coolplayer\profile.obj coolplayer\RotatingIcon.obj coolplayer\shwapi.obj \
	coolplayer\skin.obj coolplayer\String.obj coolplayer\WindowsOS.obj

MAD = mad\bit.obj mad\decoder.obj mad\fixed.obj mad\frame.obj mad\huffman.obj \
	mad\layer12.obj mad\layer3.obj mad\stream.obj mad\synth.obj mad\timer.obj \
	mad\version.obj

OGG = ogg\bitwise.obj ogg\framing.obj

VORBIS = vorbis\analysis.obj vorbis\bitrate.obj vorbis\block.obj vorbis\codebook.obj \
	vorbis\envelope.obj vorbis\floor0.obj vorbis\floor1.obj vorbis\info.obj \
	vorbis\lookup.obj vorbis\lpc.obj vorbis\lsp.obj vorbis\mapping0.obj vorbis\mdct.obj \
	vorbis\psy.obj vorbis\registry.obj vorbis\res0.obj vorbis\sharedbook.obj \
	vorbis\smallft.obj vorbis\synthesis.obj vorbis\vorbisfile.obj vorbis\window.obj

# zlib: only what the skin reader needs (inflate and crc32)
ZLIB = zlib\adler32.obj zlib\crc32.obj zlib\inffast.obj zlib\inflate.obj \
	zlib\inftrees.obj zlib\zutil.obj

LIBS = kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib \
	ole32.lib uuid.lib comctl32.lib winmm.lib

all: coolplayer.exe

coolplayer.exe: $(CP) mad.lib ogg.lib vorbis.lib zlib.lib coolplayer\coolplayer.res
	link -nologo -subsystem:windows -out:coolplayer.exe $** $(LIBS)

mad.lib: $(MAD)
	lib -nologo -out:mad.lib $**

ogg.lib: $(OGG)
	lib -nologo -out:ogg.lib $**

vorbis.lib: $(VORBIS)
	lib -nologo -out:vorbis.lib $**

zlib.lib: $(ZLIB)
	lib -nologo -out:zlib.lib $**

coolplayer\coolplayer.res: coolplayer\coolplayer.rc coolplayer\resource.h
	rc -r -i coolplayer -fo coolplayer\coolplayer.res coolplayer\coolplayer.rc

{coolplayer}.c{coolplayer}.obj:
	cl $(CFLAGS) -Imad -Iogg -Ivorbis -I. -Fo$@ $<

{mad}.c{mad}.obj:
	cl $(CFLAGS) -DFPM_DEFAULT -DASO_ZEROCHECK -DHAVE_CONFIG_H -Imad\data -Fo$@ $<

{ogg}.c{ogg}.obj:
	cl $(CFLAGS) -Iogg -Fo$@ $<

{vorbis}.c{vorbis}.obj:
	cl $(CFLAGS) -Iogg -Fo$@ $<

# Visual C++ 4.2 for x86 makes wrong code for zlib's inflate.c with -Og: some
# skin images do not unpack, and CoolPlayer crashes at start. zlib only unpacks
# the skin, so -Og- costs nothing.
{zlib}.c{zlib}.obj:
	cl $(CFLAGS) -Og- -Fo$@ $<

clean:
	-del coolplayer.exe
	-del *.lib
	-del coolplayer\coolplayer.res
	-del coolplayer\*.obj
	-del mad\*.obj
	-del ogg\*.obj
	-del vorbis\*.obj
	-del zlib\*.obj
