      program main
C%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
C                                                                      %
C Copyright (C) 1996, The Board of Trustees of the Leland Stanford     %
C Junior University.  All rights reserved.                             %
C                                                                      %
C The programs in GSLIB are distributed in the hope that they will be  %
C useful, but WITHOUT ANY WARRANTY.  No author or distributor accepts  %
C responsibility to anyone for the consequences of using them or for   %
C whether they serve any particular purpose or work at all, unless he  %
C says so in writing.  Everyone is granted permission to copy, modify  %
C and redistribute the programs in GSLIB, but only under the condition %
C that this notice and the above copyright notice remain intact.       %
C                                                                      %
C%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
c-----------------------------------------------------------------------
c
c             Add Coordinates to a GSLIB Gridded File
c             ***************************************

c Takes a 3-D realization that is stored in the order implicit to GSLIB
c grid files and adds coordinates (for transfer to other programs, error
c checking, and so on)
c
c
c
c-----------------------------------------------------------------------
      parameter (MAXLEN=132,MAXFIL=40,VERSION=2.000)
      character str*132,datafl*40,outfl*40,fmt*36,label*20
      logical   testfl
      data      lin/1/,lout/2/
c
c Note VERSION number:
c
      write(*,9999) VERSION
 9999 format(/' ADDCOORD Version: ',f5.3/)
c
c Get the name of the parameter file - try the default name if no input:
c
      write(*,*) 'Which parameter file do you want to use?'
      read (*,'(a20)') str(1:20)
      if(str(1:1).eq.' ') str(1:20) = 'addcoord.par        '
      inquire(file=str(1:20),exist=testfl)
      if(.not.testfl) then
            write(*,*) 'ERROR - the parameter file does not exist,'
            write(*,*) '        check for the file and try again  '
            write(*,*)
            if(str(1:20).eq.'addcoord.par        ') then
                  write(*,*) '        creating a blank parameter file'
                  call makepar
                  write(*,*)
            end if
            stop
      endif
      open(lin,file=str(1:20),status='OLD')
c
c Find Start of Parameters:
c
 1    read(lin,'(a4)',end=98) str(1:4)
      if(str(1:4).ne.'STAR') go to 1
c
c Read Input Parameters:
c
      read(lin,'(a40)',err=98) datafl
      call chknam(datafl,MAXFIL)
      write(*,*) 'Data File = ',datafl

      read(lin,'(a40)',err=98) outfl
      call chknam(outfl,MAXFIL)
      write(*,*) 'Output File = ',outfl

      read(lin,*,err=98) isim
      write(*,*) 'Realization number = ',isim

      read(lin,*,err=98) nx,xmn,xsiz
      write(*,*) 'X grid size = ',nx,xmn,xsiz

      read(lin,*,err=98) ny,ymn,ysiz
      write(*,*) 'Y grid size = ',ny,ymn,ysiz

      read(lin,*,err=98) nz,zmn,zsiz
      write(*,*) 'Z grid size = ',nz,zmn,zsiz

      write(*,*)
      close(lin)
c
c Format?
c
      ix = 7
      xbig = max(abs(xmn),abs(xmn+real(nx-1)*xsiz))
      if(xbig.gt.1) ix = 6
      if(xbig.gt.10) ix = 5
      if(xbig.gt.100) ix = 4
      if(xbig.gt.1000) ix = 3
      if(xbig.gt.10000) ix = 2
      if(xbig.gt.100000) ix = 1
      if(xbig.gt.1000000) ix = 0
      iy = 7
      ybig = max(abs(ymn),abs(ymn+real(ny-1)*ysiz))
      if(ybig.gt.1) iy = 6
      if(ybig.gt.10) iy = 5
      if(ybig.gt.100) iy = 4
      if(ybig.gt.1000) iy = 3
      if(ybig.gt.10000) iy = 2
      if(ybig.gt.100000) iy = 1
      if(ybig.gt.1000000) iy = 0
      iz = 7
      zbig = max(abs(zmn),abs(zmn+real(nz-1)*zsiz))
      if(zbig.gt.1) iz = 6
      if(zbig.gt.10) iz = 5
      if(zbig.gt.100) iz = 4
      if(zbig.gt.1000) iz = 3
      if(zbig.gt.10000) iz = 2
      if(zbig.gt.100000) iz = 1
      if(zbig.gt.1000000) iz = 0
      write(fmt,101) ix,iy,iz
 101  format('(f10.',i1,',1x,f10.',i1,',1x,f10.',i1,',1x,a)')
      write(*,*) 'Format: ',fmt
c
c Read in the data (if the file exists): 
c
      inquire(file=datafl,exist=testfl)
      if(.not.testfl) then
            write(*,*) 'ERROR - the data file does not exist,'
            write(*,*) '        check for the file and try again  '
            stop
      endif
      open(lout,file=outfl,status='UNKNOWN')
      open(lin,file=datafl,status='OLD')
      read(lin,'(a)',err=99) str
      read(lin,*,err=99)     nvari
      write(lout,200) nvari+3
 200  format('With Coordinates',/,i3,/,'X',/,'Y',/,'Z')
      do i=1,nvari
            read(lin,'(a20)',err=99) label
            write(lout,'(a20)') label
      end do
c
c Get input/output files ready:
c
c
c Get to the right realization:
c
      do i=1,isim-1
            do iz=1,nz
                  do iy=1,ny
                        do ix=1,nx
                              read(lin,*)
                        end do
                  end do
            end do
      end do
c
c Loop over grid
c
      do iz=1,nz
            zz = zmn + real(iz-1)*zsiz
            do iy=1,ny
                  yy = ymn + real(iy-1)*ysiz
                  do ix=1,nx
                        xx = xmn + real(ix-1)*xsiz
                        read(lin,'(a80)') str(1:80)
                        call strlen(str,80,lostr)
                        write(lout,fmt) xx,yy,zz,str(1:lostr)
                  end do
            end do
      end do
c
c Finished:
c
      close(lin)
      close(lout)
      write(*,9998) VERSION
 9998 format(/' ADDCOORD Version: ',f5.3, ' Finished'/)
      stop
 98   stop ' ERROR in parameter file'
 99   stop ' ERROR in data file'
      end



      subroutine makepar
c-----------------------------------------------------------------------
c
c                      Write a Parameter File
c                      **********************
c
c
c
c-----------------------------------------------------------------------
      lun = 99
      open(lun,file='addcoord.par',status='UNKNOWN')
      write(lun,10)
 10   format('                  Parameters for ADDCOORD',/,
     +       '                  ***********************',/,/,
     +       'START OF PARAMETERS:')

      write(lun,11)
 11   format('addcoord.dat                      ',
     +       '-file with data')
      write(lun,12)
 12   format('addcoord.out                      ',
     +       '-file for output')
      write(lun,13)
 13   format('1                                 ',
     +       '-realization number')
      write(lun,14)
 14   format('4   5.0    10.0                   ',
     +       '-nx,xmn,xsiz')
      write(lun,15)
 15   format('4   2.5     5.0                   ',
     +       '-ny,ymn,ysiz')
      write(lun,16)
 16   format('2   0.5     1.0                   ',
     +       '-nz,zmn,zsiz')

      close(lun)
      return
      end
