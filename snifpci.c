#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "direct.h"
#define INCL_DOS
#define INCL_DOSDEVIOCTL
#include "os2.h"
#define APIRET unsigned short
#define OEMHLP_PCI                         0x000B
#define IOCTL_OEMHLP                       0x0080

UCHAR lookup_bios(HFILE filehandle,USHORT deviceid,USHORT func,USHORT bus,USHORT index)
{
  APIRET rc;
  struct _READPCI_CONF
   {
      UCHAR sub_func;
      UCHAR bus_number;
      UCHAR dev_funcnum;
      UCHAR conf_reg;
      UCHAR size;
   } rpci_conf;
  struct _READPCI_CONF_RESP
   {
     UCHAR ret_code;
     ULONG data;
   } rpci_conf_resp;
  rpci_conf.sub_func    = 3;
  rpci_conf.bus_number  = bus;
  rpci_conf.dev_funcnum = deviceid * 8 + func;
  rpci_conf.conf_reg    = index;
  rpci_conf.size        = sizeof(UCHAR);
  rc = DosDevIOCtl( (PVOID) &rpci_conf_resp,(PVOID)&rpci_conf,OEMHLP_PCI,IOCTL_OEMHLP,filehandle);
  if (rc != 0)
    return 0xff;
  return rpci_conf_resp.data;
}
int main(int argc,char **argv)
{
  UCHAR  info_tbl[255];
  APIRET rc;
  HFILE  filehandle;
  USHORT action;
  UCHAR  sub_func;
  struct _QUERY_PCI
   {
      UCHAR ret_code;
      UCHAR hard_mecha;
      UCHAR major_ver;
      UCHAR minor_ver;
      UCHAR last_bus;
   } qpci;
  USHORT bus;
  USHORT deviceid;
  USHORT func;
  USHORT index;
  UCHAR  nic[256];
  UCHAR  tmp[256];
  HDIR   dir_hwnd;
  USHORT nbr_match;
  FILEFINDBUF dir_buf;

  rc = DosOpen("OEMHLP$",
               &filehandle,
               &action,
               0L,
               0,
               1,
               0x40,
               0L);
  if (rc != 0)
    return rc;
  sub_func = 0;
  rc = DosDevIOCtl( (PVOID)&qpci,(PVOID) &sub_func,OEMHLP_PCI,IOCTL_OEMHLP,filehandle);
  if (rc != 0)
    return rc;

  for(bus=0;bus <= qpci.last_bus;bus++)
   {
    for(deviceid=0;deviceid <= 0x1f; deviceid++)
     {
       for(func=0;func <= 7;func++)
        {
           for(index=0;index<255;index++)
            {
              info_tbl[index] = lookup_bios(filehandle,deviceid,func,bus,index);
              if (index == 1 && info_tbl[0] == 0xff && info_tbl[1] == 0xff)
                index = 255;
            }
           if (info_tbl[0] != 0xff && info_tbl[1] != 0xff)
            {
              sprintf(nic,"\\IBMCOM\\AUTO\\%02X%02X.%02X%02X\\*.*",info_tbl[1],info_tbl[0],info_tbl[3],info_tbl[2]);
              dir_hwnd  = HDIR_SYSTEM;
              nbr_match = 1;
              rc = DosFindFirst( nic ,
                                 &dir_hwnd,
                                 FILE_NORMAL,
                                 &dir_buf,
                                 sizeof(dir_buf),
                                 &nbr_match,
                                 0);
              nic[ strlen(  nic ) -3] = '\0';
              while(rc == 0)
               {
                 if (dir_buf.achName[0] != '.')
                   {
                     if (toupper(dir_buf.achName[ strlen(dir_buf.achName) -3 ]) == 'O' &&
                         toupper(dir_buf.achName[ strlen(dir_buf.achName) -2 ]) == 'S' &&
                         toupper(dir_buf.achName[ strlen(dir_buf.achName) -1 ]) == '2')
                       {
                          sprintf(tmp,"%s\%s",nic,dir_buf.achName);
                          rc = DosCopy(tmp,"\\IBMCOM\\MACS\\AUTO.OS2",1,0);
                          printf("%d COPY %s\%s to \\IBMCOM\\MACS\\AUTO.OS2\n\r",rc,nic,dir_buf.achName);
                       }
                     if (toupper(dir_buf.achName[ strlen(dir_buf.achName) -3 ]) == 'I' &&
                         toupper(dir_buf.achName[ strlen(dir_buf.achName) -2 ]) == 'N' &&
                         toupper(dir_buf.achName[ strlen(dir_buf.achName) -1 ]) == 'I')
                       {
                          sprintf(tmp,"%s\%s",nic,dir_buf.achName);
                          rc = DosCopy(tmp,"\\IBMCOM\\PROTOCOL.INI",1,0);
                          printf("%d COPY %s\%s to \\IBMCOM\\MACS\\PROTOCOL.INI\n\r",rc,nic,dir_buf.achName,rc);
                       }
                   }
                 rc = DosFindNext( dir_hwnd,
                                   &dir_buf,
                                   sizeof(dir_buf),
                                   &nbr_match);
               }
            }
/******************************************************************************/
/*                                                                            */
/* If not multi-device device, then don't test for func 1-7 as some cards     */
/* incorrectly answer back on all 8 function numbers!!!                       */
/* S3 trio64, for example - stupid!                                           */
/*                                                                            */
/******************************************************************************/
           if ((func==0) && ( (info_tbl[0xe] && 0x80)==0))
             func = 7;

        }
     }
   }
  if (rc != 0)
    return rc;
  rc = DosClose( filehandle );
  return 0;
}
