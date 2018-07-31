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
c The following Parameters control static dimensioning within sasim3d:
c
c   MAXX      maximum nodes in X
c   MAXY      maximum nodes in Y
c   MAXZ      maximum nodes in Z
c   MAXDAT    maximum number of cutoffs/data to define CDF
c   MAXHST    maximum number of classes in histogram objective function
c   MAXCUT    maximum number of indicator cutoffs
c   MAXLAG    maximum number of lags in variogram calculation
c   MAXNST    maximum number of structures for variogram model
c
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter (MAXX   =  100, MAXY   =  100, MAXZ   =       50,
     +           MAXHST =10000, MAXCUT =    5, MAXDAT =    10000, 
     +           MAXLAG =  250, MAXNST =    2, MAXROT = MAXNST+1,
     +           MAXPRI =   15, MAXSEC =   15)
c
c Fixed Parameters:
c
      parameter(EPSLON=1.0e-10,BIGNUM=1.0e10,MAXOBJ=8,VERSION=2.000)
c
c ACORN parameters:
c
      parameter(KORDEI=12,MAXOP1=KORDEI+1,MAXINT=2**30)
c
c Variable declaration:
c
      integer   report,ixl(MAXLAG),iyl(MAXLAG),izl(MAXLAG),
     +          kasas,ksas,num,maxswap,ndat(0:MAXSEC+1)
      real      secdat(MAXDAT),pridat(MAXDAT),wtdat(MAXDAT),
     +          varavg(MAXX,MAXY),
     +          var(MAXX,MAXY,MAXZ),secvar(MAXX,MAXY,MAXZ),
     +          varnew(MAXLAG),varmod(MAXLAG),varact(MAXLAG),
     +          vardiv(MAXLAG),ivaract(MAXCUT,MAXLAG),
     +          ivarnew(MAXCUT,MAXLAG),ivarmod(MAXCUT,MAXLAG),
     +          icut(MAXCUT),iprop(MAXCUT),userfac(MAXOBJ),
     +          tmin,tmax,omin,omin2,reltol,redfac
      real*8    objscl(MAXOBJ),rotmat(MAXROT,3,3)

      integer   nhist,ndhist
      real      hzval(MAXHST),hqact(MAXHST),hqnew(MAXHST),thnum,
     +          histdat(MAXDAT),histwt(MAXDAT)

      integer   npricut,nseccut
      real      pricut(MAXSEC+1,MAXPRI+1),seccut(MAXSEC+1)
      real      refpdf(MAXSEC+1,MAXPRI+1),actpdf(MAXSEC+1,MAXPRI+1),
     +          trypdf(MAXSEC+1,MAXPRI+1)

      real      tnum
      real*8    sumsx,sumsy,sumsxx,sumsyy,sumsxy,sumtx,sumtxx,sumtxy

      integer   nst(1),it(MAXNST)
      real      c0(1),cc(MAXNST),aa(MAXNST),ang1(MAXNST),ang2(MAXNST),
     +          ang3(MAXNST),anis1(MAXNST),anis2(MAXNST)

      integer   inst(MAXCUT),iit(MAXCUT,MAXNST)
      real      ic0(MAXCUT),icc(MAXCUT,MAXNST),iaa(MAXCUT,MAXNST),
     +          iang1(MAXCUT,MAXNST),iang2(MAXCUT,MAXNST),
     +          iang3(MAXCUT,MAXNST),ianis1(MAXCUT,MAXNST),
     +          ianis2(MAXCUT,MAXNST)

      character outfl*40
      logical   cond(MAXX,MAXY,MAXZ),
     +          testhist,testvarg,testivar,testcorr,testcpdf,vertavg
c
c Common blocks:
c

      common /iaco/   ixv(MAXOP1)

      common /grid3d/ nx,ny,nz,xsiz,ysiz,zsiz,xmn,ymn,zmn

      common /genral/ lout,idbg,report,ldbg,nsim,var,varavg,t0,redfac,
     +                kasas,ksas,num,omin,omin2,reltol,maxswap,
     +                userfac,objscl,tmin,tmax,ilog,maxnochange

      common /secstf/ sumsx,sumsy,sumsxx,sumsyy,sumsxy,
     +                sumtx,sumtxx,sumtxy,secvar,corr,tnum

      common /indics/ nicut,icut,iprop,ivaract,ivarnew,ivarmod,
     +                inst,ic0,icc,iaa,iang1,iang2,iang3,ianis1,
     +                ianis2,iit

      common /inimod/ isill,secdat,pridat,ndata,ndat,zmin,zmax

      common /variog/ nlag,ixl,iyl,izl,varnew,varact,vardiv,varmod

      common /histcm/ nhist,ndhist,histdat,histwt,
     +                hzval,hqact,hqnew,thnum

      common /cova3d/ nst,c0,it,cmax,cc,aa,ang1,ang2,ang3,anis1,anis2
 
      common /cpdf/   npricut,nseccut,pricut,seccut,refpdf,actpdf,trypdf

      common /charac/ outfl

      common /logics/ cond,testhist,testvarg,testivar,testcorr,testcpdf,
     +                vertavg
