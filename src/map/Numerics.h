/**
 * ====================================================================================================
 *                              Numerics.h
 * ====================================================================================================
 *	     
 * Copyright Sept. 2012
 * 
 * Author: Marco D. Masciola, 
 * National Renewable Energy Laboratory, Golden, Colorado, USA
 *
 * This file is part of the Mooring Analysis Program (MAP).
 *
 * MAP is free software: you can redistribute it and/or modify it under the terms 
 * of the GNU General Public License as published by the Free Software Foundation, 
 * either version 3 of the License, or (at your option) any later version.
 *
 * MAP is distributed in the hope that it will be useful, but WITHOUT ANY 
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along 
 * with MAP. If not, see:
 * 
 * < http://www.gnu.org/licenses/>
 * ====================================================================================================
 */


#ifndef _NUMERICS_H
#define _NUMERICS_H


class MAP_OtherStateType_class;


/**
 * This is the guts of the numerics solver. It is dependent on the PETSc library. Current, MPI is not 
 * supported, and the PETSc uniprocessor case functions just fine for now (because the size of the 
 * unknowns is small).
 *
 * @property   SNES                      snes                  nonlinear solver context 
 * @property   KSP                       ksp;                  Krylov sup-space context
 * @property   PC                        pc;                   preconditioner context
 * @property   Vec                       x;                    solution vector
 * @property   Vec                       r;                    residual vectors
 * @property   Mat                       J;                    Jacobian matrix
 * @property   PetscErrorCode            ierr;                 PETSc error context
 * @property   PetscInt                  its;                  number of iterations to convergence
 * @property   PetscMPIInt               size;                 Not needed (for MPI)
 * @property   PetscMPIInit              rank;                 Not needed (for MPI)
 * @property   PetscScalar               *X;                         
 * @property   PetscBool                 help_flag;            Help flag. If '-help' is in the input file, 
 *                                                             this is true
 * @property   SNESConvergedReason       reason;               Reason the non-inear solver converged
 * @property   PetscBool                 msqs_fd_jacobian;     is the finite-differenced Jacobian used?
 * @property   PetscBool                 msqs_default_setting  are we using the default solver options?
 * @property   PetscReal                 msqs_tol;             set the msqs residual function tolerance
 * @property   std::vector<std::string>  options_string;       PETSc and MAP(MSQS) run-time options
 */
class 
Numerics 
{
private:
  SNES                     snes;         // nonlinear solver context 
  KSP                      ksp;          // linear solver context
  PC                       pc;           // preconditioner context
  Vec                      x;            // solution
  Vec                      r;            // residual vectors
  Mat                      J;            // Jacobian matrix
  PetscErrorCode           ierr;
  PetscInt                 its;
  PetscMPIInt              size;
  PetscMPIInt              rank;
  PetscScalar              *X;
  PetscBool                help_flag;
  SNESConvergedReason      reason;

  // option built into MAP
  PetscBool                msqs_fd_jacobian;  
  PetscBool                msqs_default_setting;
  PetscReal                msqs_tol;

  // PETSc run-time options listed in the MAP input file
  std::vector<std::string> options_string;  

public:
  
Numerics( ) : 
  X           ( NULL       ) ,   // make sure X points to nothing
  help_flag   ( PETSC_TRUE ) ,   // default for the help flag. This is false if '-help' is defined in input 
  msqs_tol    (1e-2        ) {}  // maximum value of all residuals to guarantee convergence
  
  ~Numerics( ) { /*ierr = PetscFinalize()*/ }
        
  int  InitializeSolver( MAP_OtherStateType_class &other ,
                         MAP_InitInputType_class  &init  ,
                         MAP_ErrStat              &err   ,  
                         MAP_Message              &msg   );
  void setNumericsOptionsString( const std::string &optionStr );

  int  PetscSolve           ( MAP_OtherStateType_class &other ,
                              MAP_ErrStat        &Error , 
                              MAP_Message        &Msg );
  void PetscConvergeReason ( MAP_ErrStat &Error, MAP_Message &Msg );
  int  PetscEnd       ( MAP_ErrStat &err, MAP_Message &msg );    

  PetscBool GetHelpFlag( ) const { return help_flag;      }
  PetscReal GetMSQSTol ( ) const { return this->msqs_tol; }
};

#endif // _NUMERICS_H