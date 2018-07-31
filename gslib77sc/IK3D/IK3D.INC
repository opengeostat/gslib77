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
c              Indicator Kriging on a 3-D Rectangular Grid
c              *******************************************
c
c This file controls both the static dimensioning and allocates most of
c the storage in common blocks.
c
c   MAXSBX    maximum super block nodes in X direction
c   MAXSBY    maximum super block nodes in Y direction
c   MAXSBZ    maximum super block nodes in Z direction
c   MAXDAT    maximum number of data points
c   MAXSAM    maximum number of data points to use in one kriging system
c   MAXCUT    maximum number of IK cutoffs
c   MAXNST    maximum number of nested structures
c   MAXROT    maximum number of rotation matrices (MAXNST+1)
c   MAXCUT    maximum number of cutoffs to define a variogram model
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXDAT =  10000,
     +          MAXSAM =     64,
     +          MAXCUT =     11,
     +          MAXNST =      4,
     +          MAXSBX =     21, MAXSBY =  21, MAXSBZ = 21)
c
c Fixed Parameters:
c
      parameter(MAXSB=MAXSBX*MAXSBY*MAXSBZ,EPSLON=0.000001,
     +          MAXROT=MAXNST*MAXCUT+1,MAXEQ=MAXSAM+1,VERSION=2.000)
c
c Static Array Dimensioning:
c
      integer nisb(MAXSB),it(MAXCUT*MAXNST),nst(MAXCUT),nviol(MAXCUT),
     +        ixsbtosr(8*MAXSB),iysbtosr(8*MAXSB),izsbtosr(8*MAXSB)
      real    x(MAXDAT),y(MAXDAT),z(MAXDAT),vr(MAXDAT,MAXCUT+1),
     +        sb(MAXDAT),tmp(MAXDAT),xa(MAXSAM),ya(MAXSAM),za(MAXSAM),
     +        vra(MAXSAM),sdis(MAXDAT),close(MAXDAT),actloc(MAXDAT),
     +        gcdf(MAXCUT),ccdf(MAXCUT),ccdfo(MAXCUT),
     +        aviol(MAXCUT),xviol(MAXCUT),
     +        thres(MAXCUT),c0(MAXCUT),cc(MAXCUT*MAXNST),
     +        aa(MAXCUT*MAXNST),ang1(MAXCUT*MAXNST),
     +        ang2(MAXCUT*MAXNST),ang3(MAXCUT*MAXNST),
     +        anis1(MAXCUT*MAXNST),anis2(MAXCUT*MAXNST)
      real*8  r(MAXEQ),s(MAXEQ),a(MAXEQ*MAXEQ),rotmat(MAXROT,3,3)
c
c The data and other input variables:
c
      common /datcom/ nd,x,y,z,vr,nx,ny,nz,xmn,ymn,zmn,xsiz,ysiz,zsiz,
     +                ndmax,ndmin,radius,sang1,sang2,sang3,sanis1,
     +                sanis2,isrot,ivrot,mik,ktype,noct,idbg,
     +                ldbg,iout,lout,tmin,tmax,gcdf,ccdf,ccdfo,
     +                ivtype,nviol,aviol,xviol,koption
c
c Search variables and data for kriging:
c
      common /srccom/ na,xa,ya,za,vra,sdis,close,actloc,
     +                ljack,ixlj,iylj,izlj,ivrlj,nvarij
c
c Double Precision kriging arrays:
c
      common /krigmt/ r,s,a,rotmat
c
c Variogram Parameters:
c
      common /vargdt/ ncut,thres,nst,c0,it,cc,aa,ang1,ang2,ang3,
     +                anis1,anis2
