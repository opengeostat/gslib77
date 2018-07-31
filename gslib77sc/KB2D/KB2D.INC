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
c                       2-D Kriging Program
c                       *******************
c
c The following Parameters control static dimensioning within okb2d:
c
c   MAXX      maximum nodes in X
c   MAXY      maximum nodes in Y
c   MAXDAT    maximum number of data points
c   MAXSAM    maximum number of data points to use in one kriging system
c   MAXDIS    maximum number of discretization points per block
c   MAXNST    maximum number of nested structures
c
c-----------------------------------------------------------------------
c
c User Adjustable:
c
      parameter(MAXDAT =10000,
     +          MAXSAM =  120,
     +          MAXDIS =   64,
     +          MAXNST =    4)
c
c Fixed
c
      parameter(MAXKD=MAXSAM+1,MAXKRG=MAXKD*MAXKD,UNEST=-999.,
     +          EPSLON=0.0000001,VERSION=2.000)
c
c Variable Declaration:
c
      real      x(MAXDAT),y(MAXDAT),vr(MAXDAT),aa(MAXNST),cc(MAXNST),
     +          ang(MAXNST),anis(MAXNST)
      integer   it(MAXNST)
c
c Common Blocks:
c
      common /datagv/ nd,x,y,vr

      common /parmfk/ nx,ny,xmn,ymn,xsiz,ysiz,nxdis,nydis,ndmin,
     +                ndmax,radius,nst,c0,it,aa,cc,ang,anis,ktype,
     +                skmean,idbg,lout,ldbg
