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
c                           LU Simulation
c                           *************
c The following Parameters control static dimensioning within lusim:
c
c   MAXX      maximum nodes in X
c   MAXY      maximum nodes in Y
c   MAXZ      maximum nodes in Z
c   MAXDAT    maximum number of data
c   MAXNST    maximum number of nested structures for variogram
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXX   =  4, MAXY   =  4, MAXZ   = 4,
     +          MAXDAT = 50, MAXNST =  4)
c
c Fixed parameters:
c
      parameter(MAXXY=MAXX*MAXY*MAXZ,PMX=999.,UNEST=-1.0e21,
     +          EPSLON=1.0e-6,VERSION=2.000)
c
c ACORN parameters:
c
      parameter(KORDEI=12,MAXOP1=KORDEI+1,MAXINT=2**30)
c
c Variable Declaration:
c
      character datafl*40,outfl*40,dbgfl*40
      real      c0(1),cc(MAXNST),aa(MAXNST),ang1(MAXNST),
     +          ang2(MAXNST),ang3(MAXNST),anis1(MAXNST),anis2(MAXNST),
     +          x(MAXDAT),y(MAXDAT),z(MAXDAT),vr(MAXDAT)
      integer   nst(1),it(MAXNST)
c
c Common Blocks:
c
      common /iaco/   ixv(MAXOP1)

      common/genral/ idbg,ldbg,lout,nsim,nx,xmn,xsiz,ny,ymn,ysiz,
     +               nz,zmn,zsiz

      common/vargar/ nst,it,aa,c0,cc,ang1,ang2,ang3,anis1,anis2


      common/charar/ datafl,outfl,dbgfl

      common/dataar/ nd,x,y,z,vr,tmin,tmax
