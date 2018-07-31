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
c                Compute Normal Scores of a Data Set
c                ***********************************
c
c PROGRAM NOTES:
c
c   1. The MAXDAT parameter controls the maximum number of values to use
c      in constructing the transformation table
c
c   2. Random Despiking (THIS IS WHY WE NEED A RANDOM NUMBER GENERATOR)
c
c
c
c-----------------------------------------------------------------------
      parameter(MAXDAT = 10000,MAXLEN=132,EPSLON=1.0e-6,VERSION=2.000)
c
c ACORN parameters:
c
      parameter(KORDEI=12,MAXOP1=KORDEI+1,MAXINT=2**30)
c
c Dimensioning:
c
      real*8    vr(MAXDAT),wt_ns(MAXDAT),twt,wtfac,w,cp,oldcp,vrg,vrr,
     +          dpowint,doubone,acorni
      real      var(50)
      character datafl*40,outfl*40,smthfl*40,transfl*40,tmpfl*40,str*132
      logical   testfl,getrank

      common /iaco/   ixv(MAXOP1)

      data      lin/1/,lout/2/,doubone/1.0/
c
c Note VERSION number before anything else:
c
      write(*,9999) VERSION
 9999 format(/' NSCORE Version: ',f5.3/)
c
c Get the name of the parameter file - try the default name if no input:
c
      write(*,*) 'Which parameter file do you want to use?'
      read (*,'(a40)') str(1:40)
      if(str(1:1).eq.' ')str='nscore.par                              '
      inquire(file=str(1:40),exist=testfl)
      if(.not.testfl) then
            write(*,*) 'ERROR - the parameter file does not exist,'
            write(*,*) '        check for the file and try again  '
            write(*,*)
            if(str(1:20).eq.'nscore.par          ') then
                  write(*,*) '        creating a blank parameter file'
                  call makepar
                  write(*,*)
            end if
            stop
      endif
      open(lin,file=str(1:40),status='OLD')
c
c Find Start of Parameters:
c
 1    read(lin,'(a40)',end=98) str(1:40)
      if(str(1:4).ne.'STAR') go to 1
c
c Read Input Parameters:
c
      read(lin,'(a40)',err=98) datafl
      call chknam(datafl,40)
      write(*,*) ' data file = ',datafl

      getrank = .false.
      read(lin,*,err=98) ivr,iwt
      if(ivr.lt.0) then
            getrank = .true.
            ivr     = -ivr
      end if
      write(*,*) ' columns = ',ivr,iwt

      read(lin,*,err=98) tmin,tmax
      write(*,*) ' trimming limits = ',tmin,tmax

      read(lin,*,err=98) ismooth
      write(*,*) ' consider a different ref. dist. (1=yes) = ',ismooth

      read(lin,'(a40)',err=98) smthfl
      call chknam(smthfl,40)
      write(*,*) ' file with reference distribution = ',smthfl

      read(lin,*,err=98) isvr,iswt
      write(*,*) ' columns = ',isvr,iswt

      read(lin,'(a40)',err=98) outfl
      call chknam(outfl,40)
      write(*,*) ' file for output = ',outfl

      read(lin,'(a40)',err=98) transfl
      call chknam(transfl,40)
      write(*,*) ' file for transformation table = ',transfl

      close(lin)
c
c Decide which file to use for establishing the transformation table:
c
      if(ismooth.eq.1) then
            tmpfl  = smthfl
            icolvr = isvr
            icolwt = iswt
      else
            tmpfl  = datafl
            icolvr = ivr
            icolwt = iwt
      end if
c
c Make sure the file exists:
c
      inquire(file=tmpfl,exist=testfl)
      if(.not.testfl) then
            write(*,*) 'ERROR: ',tmpfl,' does not exist'
            write(*,*) '       this file is needed! '
            stop
      endif
c
c Open up the file with reference distribution:
c
      open(lin,file=tmpfl,status='UNKNOWN')
      do i=1,MAXOP1
            ixv(i) = 0.0
      end do
      ixv(1) = 69069
      do i=1,1000
            p = real(acorni(idum))
      end do
c
c Read the header off the data file and prepare the output files:
c
      read(lin,'(a40)',err=98) str(1:40)
      read(lin,*,err=99) nvari
      do i=1,nvari
            read(lin,*,err=98)
      end do
c
c Now, read in the actual data:
c
      nt     = 0
      nd     = 0
      twt    = 0.0
 3    read(lin,*,end=4,err=99) (var(j),j=1,nvari)
c
c Trim this data?
c
      if(var(icolvr).lt.tmin.or.var(icolvr).ge.tmax) then
            nt = nt + 1
            go to 3
      endif
      nd = nd + 1
c
c Exceeded available storage?
c
      if(nd.gt.MAXDAT) then
            write(*,*) ' ERROR: not enough room for data ',MAXDAT
            stop
      endif
c
c Keep this data: Assign the data value and coordinate location:
c
      vr(nd) = dble(var(icolvr))+acorni(idum)*dble(EPSLON)
      if(icolwt.le.0) then
            wt_ns(nd) = 1.0
      else
            wt_ns(nd) = dble(var(icolwt))
      endif
      if(wt_ns(nd).le.1.0e-10) then
            nd = nd - 1
            nt = nt + 1
            go to 3
      end if
      twt = twt + wt_ns(nd)
c
c Go back for another datum:
c
      go to 3
 4    close(lin)
      if(nd.le.1.or.real(twt).le.EPSLON) then
            write(*,*) 'ERROR: too few data'
            stop
      endif
c
c Write transformation table:
c
      open(lout,file=transfl,status='UNKNOWN')
c
c Sort data by value:
c
      istart = 1
      iend   = nd
      call dsortem(istart,iend,vr,1,wt_ns,c,d,e,f,g,h)
c
c Compute the cumulative probabilities and write transformation table
c
      wtfac = 1.0/twt
      oldcp = 0.0
      cp    = 0.0
      do j=istart,iend
            w     =  wtfac*wt_ns(j)
            cp    =  cp + w
            wt_ns(j) = (cp + oldcp)/2.0
            call gauinv(wt_ns(j),vrrg,ierr)
            vrg = dble(vrrg)
            write(lout,201) vr(j),vrrg
 201        format(f12.5,1x,f12.5)
            oldcp =  cp
c
c Now, reset the weight to the normal scores value:
c
            wt_ns(j) = vrg
      end do
      close(lout)
c
c Now, write the output file with the normal score transform:
c
      open(lin,file=datafl, status='OLD')
      do i=1,MAXOP1
            ixv(i) = 0.0
      end do
      ixv(1) = 69069
      do i=1,1000
            p = real(acorni(idum))
      end do
      open(lout,file=outfl, status='UNKNOWN')
      read(lin,'(a40)',err=99) str(1:40)
      write(lout,100)          str(1:40)
      read(lin,*,err=99)       nvari
      write(lout,'(i2)')       nvari+1
      do i=1,nvari
            read(lin,'(a40)',err=99) str(1:40)
            write(lout,'(a40)')      str(1:40)
      end do
      write(lout,101)
 100  format('Normal Score Transform:',a40)
 101  format('Normal Score value')
c
c Read and write all the data until the end of the file:
c
      id = 0
 7    read(lin,*,end=9) (var(i),i=1,nvari)
c
c Normal Scores Transform:
c
      if(var(ivr).lt.tmin.or.var(ivr).ge.tmax) then
            vrg = -999.
      else
            vrr = dble(var(ivr))+acorni(idum)*dble(EPSLON)
c
c Now, get the normal scores value for "vrr" 
c
            call dlocate(vr,nd,1,nd,vrr,j)
            j   = min(max(1,j),(nd-1))
            vrg = dpowint(vr(j),vr(j+1),wt_ns(j),wt_ns(j+1),vrr,doubone)
      endif
c
c Write the rank order instead of the rank?
c
      if(getrank) then
            pp  = real(vrg)
            yy  = gcum(pp)
            vrg = dble(yy)
      end if
c
c Write out the results:
c
      backspace lin
      read(lin,'(a)') str
      call strlen(str,MAXLEN,lostr)
      write(lout,'(a,1x,f12.5)') str(1:lostr),real(vrg)
      go to 7
 9    continue
c
c Finished:
c
      close(lin)
      close(lout)
      write(*,9998) VERSION
 9998 format(/' NSCORE Version: ',f5.3, ' Finished'/)
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
      open(lun,file='nscore.par',status='UNKNOWN')
      write(lun,10)
 10   format('                  Parameters for NSCORE',/,
     +       '                  *********************',/,/,
     +       'START OF PARAMETERS:')

      write(lun,11)
 11   format('../data/cluster.dat      ',
     +       '-file with data')
      write(lun,12)
 12   format('3   5                    ',
     +       '-  columns for variable and weight')
      write(lun,13)
 13   format('-1.0e21   1.0e21         ',
     +       '-  trimming limits')
      write(lun,14)
 14   format('0                        ',
     +       '-1=transform according to specified ref. dist.')
      write(lun,15)
 15   format('../histsmth/histsmth.out ',
     +       '-  file with reference dist.')
      write(lun,16)
 16   format('1   2                    ',
     +       '-  columns for variable and weight')
      write(lun,17)
 17   format('nscore.out               ',
     +       '-file for output')
      write(lun,18)
 18   format('nscore.trn               ',
     +       '-file for output transformation table')

      close(lun)
      return
      end
