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
c The following Parameters control static dimensioning for SISIM:
c
c   MAXX      maximum nodes in X
c   MAXY      maximum nodes in Y
c   MAXZ      maximum nodes in Z
c   MAXDAT    maximum number of data
c   MAXZCUT   maximum number of primary cutoffs
c   MAXVCUT   maximum number of secondary cutoffs
c   MAXTAB    maximum number of data in tabulated values for detail
c   MAXSAM    maximum number of data for one simulation
c   MAXNOD    maximum number of previously simulated nodes to use
c   MAXNST    maximum number of nested structures for variogram
c   MAXROT    maximum number of rotation matrices to store
c   MAXCTX    maximum X points in covariance table (ODD number)
c   MAXCTY    maximum Y points in covariance table (ODD number)
c   MAXCTZ    maximum Z points in covariance table (ODD number)
c   MAXSBX    maximum super block nodes in X direction
c   MAXSBY    maximum super block nodes in Y direction
c   MAXSBZ    maximum super block nodes in Z direction
c
c-----------------------------------------------------------------------
c
c User Adjustable Parameters:
c
      parameter(MAXX=     100, MAXY=     100, MAXZ=     1,
     +          MAXCTX=    71, MAXCTY=    71, MAXCTZ=   1,
     +          MAXSBX=    21, MAXSBY=    21, MAXSBZ=   1,
     +          MAXDAT= 10000, MAXTAB= 1000, MAXZCUT=  11,
     +          MAXNOD=    25, MAXSAM=    32, MAXNST=   4,
     +          MAXVCUT = 11)
c
c Fixed Parameters:
c
      parameter(MXYZ=MAXX*MAXY*MAXZ,MAXKR1=2*MAXNOD+2*MAXSAM+1,
     +          MAXKR2=MAXKR1*MAXKR1,MAXROT=MAXZCUT*MAXNST+1,
     +          MAXCXY=MAXCTX*MAXCTY,MAXXYZ=MAXCTX*MAXCTY*MAXCTZ,
     +          MAXSB=MAXSBX*MAXSBY*MAXSBZ,MXSXY=4*MAXSBX*MAXSBY,
     +          MXSX=2*MAXSBX,MXZCUT=MAXZCUT+1,EPSLON=1.0e-20,
     +          UNEST=-99.0,VERSION=2.000)
c
c ACORN parameters:
c
      parameter(KORDEI=12,MAXOP1=KORDEI+1,MAXINT=2**30)
c
c Static Array Dimensioning:
c
      integer ixnode(MAXXYZ),iynode(MAXXYZ),iznode(MAXXYZ),nisb(MAXSB),
     +        inoct(8),icnode(MAXNOD),
     +        ixsbtosr(8*MAXSB),iysbtosr(8*MAXSB),izsbtosr(8*MAXSB),
     +        it(MAXZCUT*MAXNST),nst(MAXZCUT),ltail,middle,utail,sstrat,
     +        mults,nmult,nviol(MAXZCUT),ivtype,imbsim

      real    ltpar,mpar,utpar,x(MAXDAT),y(MAXDAT),z(MAXDAT),
     +        vr(MAXDAT,MXZCUT),close(MAXDAT),actloc(MAXDAT),
     +        tmpdat(MAXDAT),zcut(MAXZCUT),calib(MAXZCUT,MAXVCUT),
     +        sim(MXYZ),order(MXYZ),tmp(MXYZ),gcut(MAXTAB),gcdf(MAXTAB),
     +        vcut(MAXVCUT),cdf(MAXZCUT),ccdf(MAXZCUT),
     +        beez(MAXZCUT),c0(MAXZCUT),cmax(MAXZCUT),ccdfo(MAXZCUT),
     +        cc(MAXNST*MAXZCUT),aa(MAXNST*MAXZCUT),
     +        ang1(MAXNST*MAXZCUT),ang2(MAXNST*MAXZCUT),
     +        ang3(MAXNST*MAXZCUT),anis1(MAXNST*MAXZCUT),
     +        anis2(MAXNST*MAXZCUT),aviol(MAXZCUT),xviol(MAXZCUT),
     +        covtab(MAXCTX,MAXCTY,MAXCTZ,MAXZCUT),cnodex(MAXNOD),
     +        cnodey(MAXNOD),cnodez(MAXNOD),cnodev(MAXNOD),
     +        cnodet(MAXNOD),vra(MAXKR1),
     +        sec(MXYZ),xold(MAXDAT),yold(MAXDAT),zold(MAXDAT)
      real*8  rotmat(MAXROT,3,3),r(MAXKR1),rr(MAXKR1),s(MAXKR1),
     +        a(MAXKR2)
      logical atnode(MAXDAT),softdat(MAXKR1)
c
c Common Blocks:
c
      common /iaco/   ixv(MAXOP1)

      common /grid3d/ nx,ny,nz,nxy,nxyz,xsiz,ysiz,zsiz,xmn,ymn,zmn

      common /data3d/ ivtype,nd,x,y,z,vr,zmin,zmax,ltail,middle,
     +                utail,ltpar,mpar,utpar,tmin,tmax,xold,yold,zold

      common /simula/ nsim,ktype,nviol,aviol,xviol,lin,lout,idbg,ldbg,
     +                sim,tmp,vcut,ncutv,sec,tmpp

      common /srch3d/ radius,radsqd,sang1,sang2,sang3,sanis1,sanis2,
     +                noct,nclose,close,actloc,ndmax,mik,sstrat,
     +                infoct,inoct,mults,nmult,maxsec

      common /indict/ ncutz,zcut,cdf,ccdf,ccdfo,beez,ng,gcut,gcdf

      common /cova3d/ nst,c0,it,cmax,cc,aa,ang1,ang2,ang3,anis1,
     +                anis2,isrot,ivrot,vra,cbb

      common /clooku/ covtab,nctx,ncty,nctz,nlooku,ixnode,iynode,iznode,
     +                ncnode,icnode,cnodex,cnodey,cnodez,cnodev,cnodet,
     +                nodmax

      common /krigev/ r,rr,s,a,rotmat,calib

      common /logick/ atnode,softdat
