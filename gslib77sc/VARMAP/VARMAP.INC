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
c                 Variogram Map/Volume Calculation
c                 ********************************
c
c The following Parameters control static dimensioning within varmap:
c
c   MAXX      maximum number of nodes in the X direction
c   MAXY      maximum number of nodes in the Y direction
c   MAXZ      maximum number of nodes in the Z direction
c   MAXDAT    maximum number of data (irregular or scattered data)
c   MAXVAR    maximum number of variables
c
c   MXVARG    maximum number of variograms possible at one time
c
c   MXLAGX    maximum number of lags in the X direction
c   MXLAGY    maximum number of lags in the Y direction
c   MXLAGZ    maximum number of lags in the Z direction
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXX   =  150, MAXY   =  150, MAXZ   =    1,
     +          MXLAGX =   50, MXLAGY =   50, MXLAGZ =   25,
     +          MAXDAT =10000, MAXVAR =    1, MXVARG =    1  )
c
c Fixed Parameters:
c
      parameter(MAXDIM=MAXX*MAXY*MAXZ,EPSLON=1.0e-20,VERSION=2.000)
c
c Variable Declaration:
c
      real    xsiz,ysiz,zsiz,tmin,tmax,dxlag,dylag,dzlag,
     +        x(MAXDAT),y(MAXDAT),z(MAXDAT),
     +        vr(MAXDIM*MAXVAR),sills(MAXVAR),
     +        np (-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG),
     +        gam(-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG),
     +        hm (-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG),
     +        tm (-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG),
     +        hv (-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG),
     +        tv (-MXLAGX:MXLAGX,-MXLAGY:MXLAGY,-MXLAGZ:MXLAGZ,MXVARG)
      integer igrid,nx,ny,nz,nxlag,nylag,nzlag,nd,isill,nvarg,minnp,
     +        ivtail(MXVARG),ivhead(MXVARG),ivtype(MXVARG)
      character outfl*40,names(MAXVAR)*12
c
c Common Blocks:
c
      common /datagv/ xsiz,ysiz,zsiz,tmin,tmax,dxlag,dylag,dzlag,
     +                vr,igrid,nx,ny,nz,isill,nvarg,minnp,
     +                nxlag,nylag,nzlag,ivtail,ivhead,ivtype

      common /parmdp/ gam,hm,tm,hv,tv,np,sills

      common /databl/ nd,x,y,z

      common /chargv/ outfl,names
