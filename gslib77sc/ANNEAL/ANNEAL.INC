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
c The following Parameters control static dimensioning within anneal:
c
c   MAXX      maximum nodes in X
c   MAXY      maximum nodes in Y
c   MAXZ      maximum nodes in Z
c   MAXLAG    maximum number of lags in variogram calculation
c   MAXDIR    maximum number of directions for variogram calculation
c   MAXRC     maximum number of rock codes
c
c
c-----------------------------------------------------------------------
c
c General parameters:
c
	parameter(MAXX   = 150, MAXY   = 150, MAXZ   =   1,
     +            MAXLAG =  24, MAXDIR =   4, MAXRC  =  10,
     +            MAXROC = MAXRC+1, VERSION=2.000)
c
c ACORN parameters:
c
	parameter   (KORDEI=12,MAXOP1=KORDEI+1,MAXINT=2**30)
c
c Dimensioning:
c
	real      prop(MAXROC)
	integer   var(MAXX,MAXY,MAXZ),
     +            ntrain(MAXROC,MAXROC,MAXDIR,MAXLAG),
     +            nact(MAXROC,MAXROC,MAXDIR,MAXLAG),
     +            ntry(MAXRC,MAXROC,MAXROC,MAXDIR,MAXLAG),
     +            num(MAXDIR,MAXLAG),seed,report,ixl(MAXDIR),
     +            iyl(MAXDIR),izl(MAXDIR),nlag(MAXDIR),icross(MAXDIR),
     +            icat(MAXROC)
	character datafl*40,trainfl*40,outfl*40,dbgfl*40

        common/iaco/ ixv(MAXOP1)

	common /genral/ nsim,nx,ny,nz,nr,np,nv,lout,idbg,report,
     +                  ldbg,seed,maxit,tol,var,num,ndir,nlag,mlag,
     +                  ixl,iyl,izl,icross,ntry,ntrain,nact,prop,ncat,
     +                  icat

	common /charac/ datafl,trainfl,outfl,dbgfl
