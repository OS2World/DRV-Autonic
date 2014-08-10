in config sys insert a line like this
device=x:\autonic.sys   x = boot drive
replace the line of your nic card by this one
device=x:\ibmcom\macs\auto.os2 x = boot drive

on your hardisk or cdrom create directories
d:\mptn\auto\xxxx.yyyy where xxxx = pci vendor id and yyyy = pci device id
into the directory xxxx.yyyy copy protocol.ini and driver zzzz.os2 for the card

copy pcisnif.exe to x:\os2 x = boot drive

what do autonic.sys
===================

at boot time autnic during the init phase DosExecPgm a program called
snifpci.exe yes you can start a program from a device driver at boot time
because during init phase of your device driver you are at ring 3
the only problem is that your program must be a 16 bits OS/2 program
(no problem with watcom 11C)

The program snifpci scan the pci env using the OEMHLP$ driver so put the driver
autonic.sys after the load of the OEMHLP$ driver.
When snifpci found a vendor id,device id it looks at x:\ibmcom\auto\ for
a directory matching.
If matching it copies the x:\ibmcom\auto\xxxx.yyyy\*.ini to x:\ibmcom\protocol.ini
and x:\ibmcom\xxxx.yyyy\*.os to x:\ibmcom\macs\auto.os2


x:\ibmcom\8086.7200\protocol.ini
x:\ibmcom\8086.7200\ibmfe.os2
x:\ibmcom\8086.7300\protocol.ini
x:\ibmcom\8086.7300\ibmcndis.os2

etc...

TODO:

1. When booting from cdrom not copy the driver and protocol.ini to the bootdrive
(it is in read only) but put it on the ram disk
Easy to do because we must just change a 16 bit ring 3 program

2. When no match read the directory \ibmcom\macs\*.nif
display the text string for all the card available in the directory and
let the user choose the card



EMAIL : serge.sterck@fmsb.be
