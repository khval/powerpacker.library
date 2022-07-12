# powerpacker.library (Project resurrection)

Based on the following work:
  PowerPack 1.0 compression routine based on PowerPacker.library 36.10 (28.9.93) written by Nico François (POWER PEAK) decompiled by Dr. MefistO in 2020
  pplib 1.2: a simple PowerPacker decompression and decryption library (placed in the Public Domain on 25-Nov-2010 by Stuart Caie).
  ppcrack 1.0a 

Powerpacker.library was popular compression library used on images, Amiga music modules and readme and other documents as well executable files, back in the late 80’s and early 90’s people used to put things on floppy disks, with only space for 880k of data, from time to time come across some old files, in particular on Amiga cover CD, and amiga magazine disks. Its popularity declined as storage space increased and as well alternative compression routines become popular.

This is a re-creation of powerpacker.library is written by Kjetil Hvalstrand in C code, the code I contributed is mostly the code defines a AmigaOS4.1 library and how it works, the compression/decompression routines is based on different sources, that was released under public domain license, and changed to work as part of the library. 

The aim of this project is to create native version of this Amiga library, in order to route-out any problems, and to allow native AmigaOS4.1/PowerPC programs to be built using the library, as well as give an extra speed boost, to any programs using the library.

# History

RELEASE 1.8 PPC (Library Version 38.01)

    (Skipping 1.7 as there exists 68k version with that number.)

    12.07.2022 Compression and decompression are working. all bits that need to be there should be.
    upgraded from pplib 1.0 to pplib 1.2 found in ppcrack.
    added support for reaction password requestor.

    03.07.2022: Changed done by LiveForit (Kjetil Hvalstrand) 
    Generated all AmigaOS4 header files for the library.
    Started the work of converting it back into a library.
    Started looking for any copyright notice, and trying to figure out status of this thing.
    I pretty much assume this is okay, as PP was included with so many things, 
    the decompression routines can be found all over the place. 
    See LICENCE for more info.

    09.08.2020: Decompiled by Dr. MefistO in 2020, Version: v1.0
    decompiled the original library and restored source code of the original compression algo (including the encryption).

RELEASE 1.6 (Library Version 36.10)

    Library header decrunching functions now allocate memory with the
      MEMF_PUBLIC attribute set.  This should ensure compatibility with
      GigaMem.
    Added new function: ppCrunchBufferDest().  Allows you to crunch from
      one buffer to another.
    Crunching routine slightly optimized.
    Release 2.1f of reqtools.library included.
    
RELEASE 1.5a (Library Version 35.347)

    Another (harmless) bug in buffer overflow detection fixed: it was
      possible a buffer overflow was detected too soon.  This especially
      happened with files which would crunch extremely well.  Fixed.
    Release 2.0a of reqtools.library included.

RELEASE 1.5 (Library Version 35.344)

    DECRUNCHING SPEED GREATLY IMPROVED!  Thanks to Alain Penders for his
      great ideas to speed up decrunching.  Decrunching is almost twice as
      fast.  Together with the 20% speedup achieved in release 1.4 of
      powerpacker.library this means decrunching is double the speed it was
      originally (version 1.3 and before)!  For the Imploder fans out there,
      PowerPacker decrunches faster than Imploder now.
      Also note that it is now MUCH better to use the Library Header feature
      of PowerPacker 4.0a as the resulting executable will decrunch twice as
      fast!
    Bug fixed:  buffer overflow wasn't always detected correctly.  This could
      result in files crunching incorrectly.  Luckily the chances of this
      happening were very small, but it is fixed now anyway.
    Small changes to autodoc regarding generating encrypted files.
    Bug fixed in Dice library, ppCalcChecksum() was not included.

RELEASE 1.4 (Library Version 35.274)

    Command files using PowerPacker 3.0b's or 4.0a's library header will
      decrunch faster.
    Decrunching code optimized.  About 20% faster.
    LoadPPLib will no longer return an error code when run on Kickstart 2.0
      or higher.  It just won't load powerpacker.library and reqtools.library
      in memory.  It can now safely be used in startup-sequences of disks for
      both Kickstart 1.2/1.3 and 2.0.
    Edited autodoc for ppDecrunchBuffer() a bit.  Hope this helps Michael
      Berg :-)  (Michael, if you are reading this, your optimized decrunch
      code in PowerPackerPatcher 1.4b turned out to be just as fast as the
      original code.  A 'lsr.l d1,d0' takes just as many clock cycles as a
      'lsr.l #1,d0'!  I suppose you compared your "optimized" decruncher to
      an old PowerPacker decruncher since I optimized the decruncher by 15 to
      20% when I released version 3.0a.  I suggest you use ppDecrunchBuffer()
      for your next release of PowerPackerPatcher since it is now faster.)
    No longer uses partial FreeMem()s as it seems these are illegal.
    Latest revision of Oberon interface included.
    Aztec C interface to the library included.
    Release 1.0d of reqtools.library included.
    
RELEASE 1.3c (Library Version 35.256)

    Libraries (powerpackers.lib and powerpackersr.lib) for Dice C included.
    Release 1.0c of reqtools.library included.
    
RELEASE 1.3b (Library Version 35.256)

    Updated Oberon interface by Kai Bolay included.
    Release 1.0b of reqtools.library included.
    
RELEASE 1.3a (Library Version 35.256)

    Fixed serious bug: in low memory situations ppAllocCrunchInfo crashed!!
    Small changes and enhancements to autodocs.
    
RELEASE 1.3 (Library Version 35.255)

    Fixed serious bug: PPLoadSeg files were incorrectly decrunched!
    Now distributed with release 1.0a of reqtools.library.
    Improved installation script.
    
RELEASE 1.2 (Library Version 35.254)

    PowerPacker 4.0's library decrunch header needs this version to work.
      Some things have changed to crunch files even better :-)
    After relocation of loaded executable the instruction and data caches
      are cleared.  This should insure compatibility with the 68040.
      Note that this only applies to Amigas running Kickstart 2.0 (V37).
    Crunching code added to library (4 new functions):
      ppAllocCrunchInfo, ppFreeCrunchInfo, ppCrunchBuffer, ppWriteDataHeader.
    One new requester function: ppEnterPassword.
    The requester functions ppGetString and ppGetLong are now declared
      obsolete!  They can now be found (much improved) in 'reqtools.library'.
    'reqtools.library' _MUST_ be installed in your system for the the old
      (obsolete) requester functions to work!  ppGetPassword and
      ppEnterPassword also call functions in 'reqtools.library'.
    ppErrorMessage function added to get default error messages.
    Support for crunched LoadSeg files added for use with PPLoadSeg 1.1.
    Stricter check for unknown data headers.
    
RELEASE 1.1a (Library Version 34.2)

    Bug fixed: A6 was not preserved in ppLoadData!
    Oberon interface to the library included.
    
RELEASE 1.1 (Library Version 34.1)

    Added 'ppOverlayDecrHdr' for use of library from overlay decrunch header.
    Requesters now have depth and close gadgets.
    Faster decrunching in 'pp...DecrHdr' code.
    
RELEASE 1.0 (Library Version 33.1)

    First release.
