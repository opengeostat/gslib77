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
c                Variogram of Irregular Spaced 3-D Data
c                **************************************
c
c The following Parameters control static dimensioning within gamv3:
c
c   MAXDAT    maximum number of data points
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
      parameter(MAXDAT = 10000, MAXVAR =    5, MAXDIR =    5,
     +          MAXLAG =   200, MXVARG =    5  )
c
c Fixed Parameters:
c
      parameter(MAXLG=MAXLAG+2,MXDLV=MAXDIR*MAXLG*MXVARG,
     +          EPSLON=1.0e-20,VERSION=2.000)
c
c Variable Declaration:
c
      real      x(MAXDAT),y(MAXDAT),z(MAXDAT),vr(MAXDAT,MAXVAR),
     +          azm(MAXDIR),atol(MAXDIR),bandwh(MAXDIR),dip(MAXDIR),
     +          dtol(MAXDIR),bandwd(MAXDIR),xlag,xltol,tmin,tmax
      real*8    dis(MXDLV),gam(MXDLV),hm(MXDLV),tm(MXDLV),hv(MXDLV),
     +          tv(MXDLV),np(MXDLV),sills(MAXVAR)
      integer   nd,nlag,ndir,nvarg,isill,
     +          ivtail(MXVARG),ivhead(MXVARG),ivtype(MXVARG)
      character outfl*40,names(MAXVAR)*12
c
c Common Blocks:
c
      common /datagv/ nd,x,y,z,vr,nlag,xlag,xltol,ndir,nvarg,isill,
     +                azm,atol,
     +                bandwh,dip,dtol,bandwd,ivtail,ivhead,ivtype,
     +                tmin,tmax

      common /parmdp/ dis,gam,hm,tm,hv,tv,np,sills

      common /chargv/ outfl,names
