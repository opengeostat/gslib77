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
c             Ordinary CoKriging of a 3-D Rectangular Grid
c             ********************************************
c
c The following Parameters control static dimensioning within cokb3d:
c
c   MAXSBX    maximum super block nodes in X
c   MAXSBY    maximum super block nodes in Y
c   MAXSBZ    maximum super block nodes in Z
c   MAXDAT    maximum number of data points
c   MAXVAR    maximum number of variables (including primary!)
c   MAXSAM    maximum number of data points
c   MAXCOK    maximum number of data points in kriging system
c   MAXDIS    maximum number of discretization points per block
c   MAXNST    maximum number of nested structures
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXSBX =    21, MAXSBY =  21, MAXSBZ =   1,
     +          MAXDAT = 10000, MAXVAR =   3, MAXSAM =  20,
     +          MAXDIS =    64, MAXNST =   4)
c
c Fixed Parameters:
c
      parameter(UNEST=-999.0,EPSLON=0.000001,MXVARG=MAXVAR*MAXVAR,
     +          MAXCOK=(MAXSAM*MAXVAR+MAXVAR),MAXROT=MAXNST+1,
     +          MAXSB=MAXSBX*MAXSBY*MAXSBZ,VERSION=2.000)
c
c Static Array Dimensioning:
c
      integer nst(MXVARG),it(MXVARG*MAXNST),iva(MAXCOK),nisb(MAXSB),
     +        ixsbtosr(8*MAXSB),iysbtosr(8*MAXSB),izsbtosr(8*MAXSB)
      real    x(MAXDAT),y(MAXDAT),z(MAXDAT),vr(MAXDAT),close(MAXDAT),
     +        sec1(MAXDAT),sec2(MAXDAT),sec3(MAXDAT),tmp(MAXDAT),
     +        xa(MAXCOK),ya(MAXCOK),za(MAXCOK),vra(MAXCOK),
     +        xdb(MAXDIS),ydb(MAXDIS),zdb(MAXDIS),
     +        vmean(MAXVAR),c0(MXVARG),cc(MXVARG*MAXNST),
     +        aa(MXVARG*MAXNST),ang1(MXVARG*MAXNST),
     +        ang2(MXVARG*MAXNST),ang3(MXVARG*MAXNST),
     +        anis1(MXVARG*MAXNST),anis2(MXVARG*MAXNST)
      real*8  r(MAXCOK),rr(MAXCOK),s(MAXCOK),a(MAXCOK*MAXCOK),unbias,
     +        rotmat(MAXROT,3,3)
c
c The data and other input variables:
c
      common /datcom/ nd,x,y,z,vr,sec1,sec2,sec3,ktype,nvr,vmean,tmin,
     +                tmax,nx,ny,nz,xmn,ymn,zmn,xsiz,ysiz,zsiz,
     +                idbg,ldbg,lout
c
c Kriging parameters:
c
      common /krigcm/ ndmin,ndmaxp,ndmaxs,radiusp,radiuss,
     +                nxdis,nydis,nzdis
c
c Variogram Parameters:
c
      common /vargdt/ nst,c0,it,cc,aa,ang1,ang2,ang3,anis1,anis2
c
c Search variables and data for kriging:
c
      common /srccom/ sang1,sang2,sang3,sanisp1,sanisp2,isrot,saniss1,
     +                saniss2,radsqdp,radsqds,na,xa,ya,za,iva,
     +                vra,xas,yas,zas,vras,xdb,ydb,zdb,ndb
c
c Kriging systems (double precision arrays):
c
      common /krgsys/ r,rr,s,a,unbias,rotmat
