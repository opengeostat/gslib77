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
c                Variogram of Data on a Regular Grid
c                ***********************************
c
c The following Parameters control static dimensioning within gam3:
c
c   MAXXYZ    maximum number of nodes in the 3-D grid
c   MAXVAR    maximum number of variables
c
c   MAXDIR    maximum number of directions possible at one time
c   MAXLAG    maximum number of lags at one time
c   MXVARG    maximum number of variograms possible at one time
c
c   MXDLV     MAXDIR*MAXLAG*MXVARG - used for dimensioning
c   EPSLON    a small number to avoid dividing by zero
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXXYZ = 22500,  
     +          MAXVAR =   5,
     +          MAXDIR =   5, 
     +          MAXLAG = 200,
     +          MXVARG =   5  )
c
c Fixed Parameters:
c
      parameter(MAXLG=MAXLAG+2,MXDLV=MAXDIR*MAXLG*MXVARG,
     +          EPSLON=1.0e-20,VERSION=2.000)
c
c Variable Declaration:
c
      real      xsiz,ysiz,zsiz,tmin,tmax,vr(MAXXYZ*MAXVAR)
      real*8    gam(MXDLV),hm(MXDLV),tm(MXDLV),hv(MXDLV),tv(MXDLV),
     +          np(MXDLV),sills(MAXVAR)
      integer   nlag,nx,ny,nz,nxy,nxyz,ndir,ixd(MAXDIR),iyd(MAXDIR),
     +          izd(MAXDIR),ivtail(MXVARG),ivhead(MXVARG),
     +          ivtype(MXVARG),isill,nvarg
      character outfl*40,names(MAXVAR)*12
c
c Common Blocks:
c
      common /datagv/ xsiz,ysiz,zsiz,tmin,tmax,vr,nlag,nx,ny,nz,nxy,
     +                nxyz,ndir,ixd,iyd,izd,ivtail,ivhead,ivtype,
     +                isill,nvarg

      common /parmdp/ gam,hm,tm,hv,tv,np,sills

      common /chargv/ outfl,names
