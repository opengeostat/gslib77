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
c                          Plot Variograms
c                          ***************
c
c New version requires a parameter file (created if none given the first
c time the program is run)
c
c
c
c-----------------------------------------------------------------------
      parameter (MAXLAG=250, BIGNUM=1.0e21,EPSLON=.00001, MAXVAR=10,
     +           MAXCAT=24,VERSION=2.000)

      integer    redint(MAXCAT),grnint(MAXCAT),bluint(MAXCAT)

      character  datafl(MAXVAR)*80,outfl*80,title*40,str*80
      real       ar1(MAXVAR,MAXLAG),ar2(MAXVAR,MAXLAG),
     +           xx(MAXLAG),yy(MAXLAG)
      integer    ivar(MAXVAR),idash(MAXVAR),ipts(MAXVAR),iline(MAXVAR),
     +           nlag(MAXVAR),iclr(MAXVAR)
      logical    testfl
c
c Common variables for Postscript Plotting:
c
      common /psdata/ lpsout,pscl,pxmin,pxmax,pymin,pymax,xmin,
     +                xmax,ymin,ymax
      common /olddat/ xvmn,xvmx,yvmn,yvmx
c
c Hardcoded categorical colors:
c
      data       redint/255,255,255,127,  0,  0,  0,255,255,0,127,170,
     +                  255,  0,200,9*255/,
     +           grnint/  0,127,255,255,255,255,  0,  0,255,0,  0, 85,
     +                   85,255,200,9*  0/,
     +           bluint/  0,  0,  0,  0,  0,255,255,255,255,0,255,  0,
     +                  170,127,200,9*255/
c
c Hardwire many of the plot parameters:
c
      data lin/1/,lpsout/2/,pscl/0.24/,pxmin/0.0/,pxmax/288.0/
     +     pymin/0.0/,pymax/216.0/,xmin/-10.0/,xmax/60.0/,
     +     ymin/-10.0/,ymax/60.0/,vpxmin/1.0/,vpxmax/59.5/,
     +     vpymin/0.0/,vpymax/58.0/
c
c Note VERSION number:
c
      write(*,9999) VERSION
 9999 format(/' VARGPLT Version: ',f5.3/)
c
c Get the name of the parameter file - try the default name if no input:
c
      write(*,*) 'Which parameter file do you want to use?'
      read (*,'(a20)') str(1:20)
      if(str(1:1).eq.' ') str(1:20) = 'vargnew.par         '
      inquire(file=str(1:20),exist=testfl)
      if(.not.testfl) then
            write(*,*) 'ERROR - the parameter file does not exist,'
            write(*,*) '        check for the file and try again  '
            write(*,*)
            if(str(1:20).eq.'vargnew.par         ') then
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
      read(lin,'(a40)',err=98) outfl
      call chknam(outfl,40)
      write(*,*) ' data file = ',outfl

      read(lin,*,err=98) nvar
      write(*,*) ' number of variograms = ',nvar
      if(nvar.gt.10) then
            write(*,*) ' Too many variograms (only allow 10 right now)'
            stop
      end if

      read(lin,*,err=98) dmin,dmax
      write(*,*) ' distance limits = ',dmin,dmax

      read(lin,*,err=98) gmin,gmax
      write(*,*) ' variogram limits = ',gmin,gmax

      read(lin,'(a40)') title
      call chktitle(title,40)
      write(*,*) ' title = ',title

      do i=1,nvar
            read(lin,'(a40)') datafl(i)
            call chknam(datafl(i),40)
            write(*,*) ' data file ',i,' = ',datafl(i)
            read(lin,*) ivar(i),idash(i),ipts(i),iline(i),iclr(i)
            write(*,*) ' parameters ',
     +                  ivar(i),idash(i),ipts(i),iline(i),iclr(i)
      end do
      close(lin)
c
c Read the variograms:
c
      do iii=1,nvar
c
c Find the right variogram:
c
      open(lin,file=datafl(iii))
      read(lin,'(a80)') str
      do jjj=1,ivar(iii)-1
            do jj3=1,1000
                  read(lin,'(a80)') str
                  if(str(1:1).ne.' ') go to 333
            end do
 333        continue
      end do
c
c Read the variogram:
c
      np = 0
      do jjj=1,1000
            read(lin,'(a80)',end=334) str
            if(str(1:1).ne.' ') go to 334
            backspace lin
            np = np + 1
            read(lin,*,err=334,end=334) iiii,ar1(iii,np),ar2(iii,np)
            if(ar1(iii,np).lt.0.0001) np = np - 1
      end do
 334  continue
      nlag(iii) = np
      close(lin)
c
c Finish reading all variograms:
c
      end do
c
c Establish data-based limits to distance and variogram:
c
      dbdmin =  1.0e21
      dbdmax = -1.0e21
      dbvmin =  1.0e21
      dbvmax = -1.0e21
      do i=1,nvar
            do j=1,nlag(i)
                  if(ar1(i,j).lt.dbdmin) dbdmin = ar1(i,j)
                  if(ar1(i,j).gt.dbdmax) dbdmax = ar1(i,j)
                  if(ar2(i,j).lt.dbvmin) dbvmin = ar2(i,j)
                  if(ar2(i,j).gt.dbvmax) dbvmax = ar2(i,j)
            end do
      end do
      if(dmax.le.dmin) then
            dmin = 0.0
            dmax = dbdmax
      end if
      if(gmax.le.gmin) then
            gmin = dbvmin
            gmax = dbvmax
      end if
      xvmin = dmin
      xvmax = dmax
      yvmin = gmin
      yvmax = gmax
c
c Write the variograms:
c
      open(lpsout,file=outfl,status='UNKNOWN')
      write(lpsout,998)
 998  format('%!PS                                 %    Remove     ',
     +    /, '90 234 translate 1.5 1.5 scale       %  these lines  ',
     +    /, '                                     % for EPSF file ',
     +    /, '%!PS-Adobe-3.0 EPSF-3.0',
     +    /, '%%BoundingBox: 0 0 288 216',
     +    /, '%%Creator: GSLIB',
     +    /, '%%Title:   ',
     +    /, '%%CreationDate: ',
     +    /, '%%EndComments',/,/,/,'%',/,'%',/,'%',/,
     +    /, '/m {moveto} def /l {lineto} def /r {rlineto} def',
     +    /, '/s {stroke} def /n {newpath} def /c {closepath} def',
     +    /, '/rtext{ dup stringwidth pop -1 div 0 rmoveto show } def',
     +    /, '/ctext{ dup stringwidth pop -2 div 0 rmoveto show } def',
     +    /, '/ltext{show} def /gr{grestore} def /gs{gsave} def',
     +    /, '/tr{translate} def /setc{setrgbcolor} def',
     +    /, '/bullet{ 8 0 360 arc c fill } def',/,/,
     +    /, '%72 72 translate',/,/,
     +    /, '0.240000 0.240000 scale')
c
c Draw and scale axes:
c
      ts   = 7.5
      xvmn = xvmin
      xvmx = xvmax
      yvmn = yvmin
      yvmx = yvmax
      write(lpsout,100)
 100  format('gsave /Symbol findfont 100 scalefont setfont ',
     +       '0 501 m (g) rtext grestore')
      xrange = vpxmax - vpxmin
      yrange = vpymax - vpymin
      xloc = vpxmin + 0.50*xrange
      yloc = vpymin - 0.15*yrange
      call pstext(xloc,yloc,8,'Distance',ts,1,0.0,1)
      xloc = vpxmin
      yloc = vpymax + 0.01*(vpxmax-vpxmin)
      call pstext(xloc,yloc,40,title,9.0,3,0.0,0)
      idsh = -1
      ilog = 0
      i45  = 0
      call scal(xvmin,xvmax,yvmin,yvmax,vpxmin,vpxmax,vpymin,vpymax,
     +          ilog,i45)
c
c Write variogram bullets and lines:
c
      do iii=1,nvar
c
c Color?
c
      if(iclr(iii).gt.0) then
            jclr = iclr(iii)
            irr  = redint(jclr)
            igg  = grnint(jclr)
            ibb  = bluint(jclr)
            write(lpsout,301) irr,igg,ibb
 301        format(3i4,' setrgbcolor')
      end if
c
c Mark bullets:
c
      if(ipts(iii).eq.1) then
            do i=1,nlag(iii)
                  xloc = resc(xvmn,xvmx,vpxmin,vpxmax,ar1(iii,i))
                  yloc = resc(yvmn,yvmx,vpymin,vpymax,ar2(iii,i))
                  if(ar1(iii,i).ge.dmin.and.ar1(iii,i).le.dmax.and.
     +               ar2(iii,i).ge.gmin.and.ar2(iii,i).le.gmax) then
                  ix = int((resc(xmin,xmax,pxmin,pxmax,xloc))/pscl)
                  iy = int((resc(ymin,ymax,pymin,pymax,yloc))/pscl)
                  write(lpsout,32) ix,iy
                  end if
            end do
 32         format('n ',i4.4,1x,i4.4,' bullet')
      end if
c
c Draw a line:
c
      if(iline(iii).eq.1) then
            np = 0
            do i=1,nlag(iii)
                  xx(i) = resc(xvmn,xvmx,vpxmin,vpxmax,ar1(iii,i))
                  yy(i) = resc(yvmn,yvmx,vpymin,vpymax,ar2(iii,i))
                  if(ar1(iii,i).lt.dmin.or.ar1(iii,i).gt.dmax.or.
     +               ar2(iii,i).lt.gmin.or.ar2(iii,i).gt.gmax) then
                        if(np.eq.0) np = i-1
                  end if
            end do
            idsh = idash(iii)
            np   = nlag(iii)
            call psline(np,xx,yy,0.5,idsh)
      end if
      write(lpsout,302)
 302  format('0 0 0 setrgbcolor')
c
c Finish loop over all annotation:
c
      end do
c
c Close data file:
c
      write(lpsout,999)
 999  format('%END OF POSTSCRIPT FILE',/,'4.166667 4.166667 scale',/,/,
     +       '%%EOF',/,'showpage')
      close(lpsout)
c
c Finished:
c
 9997 write(*,9998) VERSION
 9998 format(/' VARGPLT Version: ',f5.3, ' Finished'/)
      stop
 98   stop 'Error in parameter file'
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
      open(lun,file='vargnew.par',status='UNKNOWN')
      write(lun,10)
 10   format('                  Parameters for VARGPLT',/,
     +       '                  **********************',/,/,
     +       'START OF PARAMETERS:')

      write(lun,11)
 11   format('vargplt.ps                   ',
     +       '-file for PostScript output')
      write(lun,12)
 12   format('2                            ',
     +       '-number of variograms to plot')
      write(lun,13)
 13   format('0.0   20.0                   ',
     +       '-distance  limits (from data if max<min)')
      write(lun,14)
 14   format('0.0    1.2                   ',
     +       '-variogram limits (from data if max<min)')
      write(lun,15)
 15   format('Normal Scores Semivariogram  ',
     +       '-Title for variogram')
      write(lun,16)
 16   format('gamv.out                     ',
     +       '-1 file with variogram data')
      write(lun,17)
 17   format('1    1   1   1    1          ',
     +       '-  variogram #, dash #, pts?, line?, color')
      write(lun,18)
 18   format('vmodel.var                   ',
     +       '-2 file with variogram data')
      write(lun,19)
 19   format('1    3   0   1   10          ',
     +       '-  variogram #, dash #, pts?, line?, color')
      write(lun,20)
 20   format(//,'Color Codes for Variogram Lines/Points:')
      write(lun,21)
 21   format('      1=red, 2=orange, 3=yellow, 4=light green,',
     +       ' 5=green, 6=light blue,',/,
     +       '      7=dark blue, 8=violet, 9=white,',
     +       ' 10=black, 11=purple, 12=brown,',/,
     +       '      13=pink, 14=intermediate green, 15=gray')

      close(lun)
      return
      end
