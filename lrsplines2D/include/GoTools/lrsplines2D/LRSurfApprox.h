/*
 * Copyright (C) 1998, 2000-2007, 2010, 2011, 2012, 2013 SINTEF ICT,
 * Applied Mathematics, Norway.
 *
 * Contact information: E-mail: tor.dokken@sintef.no                      
 * SINTEF ICT, Department of Applied Mathematics,                         
 * P.O. Box 124 Blindern,                                                 
 * 0314 Oslo, Norway.                                                     
 *
 * This file is part of GoTools.
 *
 * GoTools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version. 
 *
 * GoTools is distributed in the hope that it will be useful,        
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with GoTools. If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * In accordance with Section 7(b) of the GNU Affero General Public
 * License, a covered work must retain the producer line in every data
 * file that is created or manipulated using GoTools.
 *
 * Other Usage
 * You can be released from the requirements of the license by purchasing
 * a commercial license. Buying such a license is mandatory as soon as you
 * develop commercial activities involving the GoTools library without
 * disclosing the source code of your own applications.
 *
 * This file may be used in accordance with the terms contained in a
 * written agreement between you and SINTEF ICT. 
 */

#ifndef _LRSURFAPPROX_H_
#define _LRSURFAPPROX_H_

#include "GoTools/geometry/SplineSurface.h"
#include "GoTools/lrsplines2D/LRSplineSurface.h"
#include "GoTools/lrsplines2D/LRSurfSmoothLS.h"
#include <vector>



//class LRSurfSmoothLS;

namespace Go
{
/// This class can generate a LR B-spline surface that approximates
/// a set of points for a given accuracy.
class LRSurfApprox
{
 public:

  /// Constructor given a parameterized point set
  /// \param points Parameterized point set given as (u1,v1,x1,y1,z1, u2, v2, ...)
  ///               The length of the array is (2+dim)x(the number of points)
  /// \param dim    The dimension of the geometry space
  /// \param epsge  Requested approximation accuracy
  /// \param closest_dist Check accuracy in closest point or in corresponding 
  ///                     parameter value
  /// \param repar Perform reparameterization during iterations
  LRSurfApprox(std::vector<double>& points, 
	       int dim, double epsge, bool closest_dist=true,
	       bool repar=false);

  /// Constructor given a parameterized point set and an initial spline surface
  /// \param srf    Given spline surface
  /// \param points Parameterized point set given as (u1,v1,x1,y1,z1, u2, v2, ...)
  ///               The length of the array is (2+dim)x(the number of points)
  ///               Note that the sequence of the points can be changed and this class
  ///               references the initial points (no copy)
  /// \param dim    The dimension of the geometry space
  /// \param epsge  Requested approximation accuracy
  /// \param closest_dist Check accuracy in closest point or in corresponding 
  ///                     parameter value
  /// \param repar Perform reparameterization during iterations
  LRSurfApprox(shared_ptr<SplineSurface>& srf,
	       std::vector<double>& points, 
	       double epsge, bool closest_dist=true,
	       bool repar=false);

  /// Constructor given a parameterized point set and an initial LR B-spline surface
  /// \param srf    Given LR B-spline surface
  /// \param points Parameterized point set given as (u1,v1,x1,y1,z1, u2, v2, ...)
  ///               The length of the array is (2+dim)x(the number of points)
  ///               Note that the sequence of the points can be changed and this class
  ///               references the initial points (no copy)
  /// \param dim    The dimension of the geometry space
  /// \param epsge  Requested approximation accuracy
  /// \param closest_dist Check accuracy in closest point or in corresponding 
  ///                     parameter value
  /// \param repar Perform reparameterization during iterations
  LRSurfApprox(shared_ptr<LRSplineSurface>& srf,
	       std::vector<double>& points, 
	       double epsge, bool closest_dist=true,
	       bool repar=false);

  /// Constructor given a parameterized point set and the size of an initial
  /// spline space
  /// \param srf    Given LR B-spline surface
  /// \param points Parameterized point set given as (u1,v1,x1,y1,z1, u2, v2, ...)
  ///               The length of the array is (2+dim)x(the number of points)
  ///               Note that the sequence of the points can be changed and this class
  ///               references the initial points (no copy)
  /// \param dim    The dimension of the geometry space
  /// \param epsge  Requested approximation accuracy
  /// \param closest_dist Check accuracy in closest point or in corresponding 
  ///                     parameter value
  /// \param repar Perform reparameterization during iterations
  LRSurfApprox(int ncoef_u, int order_u, int ncoef_v, int order_v,
	       std::vector<double>& points, 
	       int dim, double epsge, bool closest_dist=true,
	       bool repar=false);

  /// Destructor
  ~LRSurfApprox();

    /// Sets the smoothing weight to something other than the default (1e-9).
    /// The value should lie in the unit interval, typically close to 0.
    /// \param smooth the new smoothing weight.
    void setSmoothingWeight(double smooth)
	{
	    ASSERT(smoothweight_ >= 0.0 && smoothweight_ <= 1.0);
	    smoothweight_ = smooth;
	}

    /// Decide whether or not the total bondary of the surface should be kept fixed
    /// (i.e. unchanged by approximation process).   Default is true.  Cross derivatives
    /// will not be kept fixed.  (If you want to keep cross derivatives fixed, use
    /// the edgeFix() member function instead).
    /// \param fix_boundary if 'true' the boundary of the surface will not be modified,
    ///                     if 'false' it will be open to modification.
    void setFixBoundary(bool fix_boundary)
    {
      int fix = (fix_boundary) ? 1 : 0;
      edge_derivs_[0] = edge_derivs_[1] = edge_derivs_[2] = edge_derivs_[3] = fix;
      setCoefKnown();
    }

    /// Decide whether specific edges of the surface's boundary should be kept fixed
    /// (i.e. unchanged by approximation process), as well as a certain number of cross-
    /// derivatives across these curves.
    /// \param edge_fix pointer to an array of four integers specifying to which extent
    ///                 each surface edge should be kept fixed during the approximation 
    ///                 process.
    ///                 A value of 0 means that it will not be kept fixed, 1 means that
    ///                 its position will be kept fixed, derivatives at the boundary
    ///                 are currently NOT kept fixed
    ///                 The integers are associated with the surface edges starting 
    ///                 with the edge 'v=vmin' and moving counterclockwise.
    void edgeFix(int edge_fix[])  // CCV
    {
      for (int ki=0; ki<4; ki++)
	edge_derivs_[ki] = std::min(1, edge_fix[ki]);
      setCoefKnown();
    }

    /// When everything else is set, this function can be used to run the 
    /// approximation process and fetch the approximated surface.
    /// \retval maxdist report the maximum distance between the approximated 
    ///                 surface and the data points
    /// \retval avdist report the average distance between the approximated 
    ///                surface and the datapoints
    /// \param nmb_out_eps report the number of data points that were found to
    ///                    lie outside the geometric tolerance (as specified by 
    ///                    the 'aepsge' argument to the ApproxSurf constructor.
    /// \param max_iter maximum number of iterations
    /// \return a shared pointer to the generated SplineCurve, approximating 
    ///         the points as specified.
    shared_ptr<LRSplineSurface> getApproxSurf(double& maxdist, 
					      double& avdist,
					      int& nmb_out_eps, 
					      int max_iter=4);

 private:
    shared_ptr<LRSplineSurface> srf_;
    std::vector<double>& points_;  // Reference to input points and parameter values
    std::vector<int> coef_known_;
    
    int edge_derivs_[4];
    double maxdist_;
    double avdist_;
    int outsideeps_;
    double aepsge_;
    double smoothweight_;
    bool repar_;
    bool check_close_;

    /// Define free and fixed coefficients
    void setCoefKnown();
    void updateCoefKnown();
    void setCoefKnown(Direction2D d, Direction2D d2, bool atstart, int fixcoef);

    /// Perform least squares approximation with a smoothing term
    void performSmooth(LRSurfSmoothLS *LSapprox);

    void computeAccuracy();

    /// Refine surface
    void refineSurf();

    /// Create initial LR B-spline surface
    void makeInitSurf(int dim);
    void makeInitSurf(shared_ptr<SplineSurface> surf);
    void makeInitSurf(int dim, int ncoef_u, int order_u, int ncoef_v, int order_v); 
    void makeInitSurf(int dim, int ncoef_u, int order_u, int ncoef_v, 
		      int order_v, double *knots_u, double *knots_v);

    /// Parameter domain surrounding the parameter values of all data points
    void computeParDomain(int dim, double& umin, double& umax, double& vmin, double& vmax);
};

}  // namespace Go

#endif
