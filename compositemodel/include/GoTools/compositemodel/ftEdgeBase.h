//===========================================================================
//                                                                           
// File: ftEdgeBase.h                                                        
//                                                                           
// Created: Mon Jul  8 15:19:56 2002                                         
//                                                                           
// Author: Sverre Briseid <Sverre.Briseid@math.sintef.no>
//                                                                           
// Revision: $Id: ftEdgeBase.h,v 1.8 2009-01-30 09:54:51 vsk Exp $
//                                                                           
// Description:
//                                                                           
//===========================================================================

#ifndef _FTEDGEBASE_H
#define _FTEDGEBASE_H

#include "GoTools/compositemodel/ftFaceBase.h"
#include "GoTools/utils/BoundingBox.h"
#include "GoTools/topology/tpJointType.h"
#include "GoTools/topology/FaceConnectivity.h"
#include <vector>
#include <memory>

namespace Go
{

class ftEdge;
class ftFaceBase;

/** ftEdgeBase -  Short description.
 * Detailed description.
 */
class ftEdgeBase //: public ftEdgeBase
{
public:
    /// Constructor
    ftEdgeBase();
    /// Destructor
    virtual ~ftEdgeBase();

    ftEdgeBase* next();
    ftEdgeBase* prev();
    ftEdgeBase* twin();

    virtual double tMin() const = 0;
    virtual double tMax() const = 0;
    //virtual void turnOrientation() = 0;
    //virtual void setOrientation() = 0;
    //virtual bool isTurned() = 0;

    /// Set a flag to indicate that the orientation of the edge is
    /// reversed with respect to the parametrization of the underlying
    /// ParamCurve.
    /// \param is_reversed value of the orientation flag
    virtual void setReversed(bool is_reversed) = 0;
    /// Get the value of the flag indicating that the orientation of
    /// the edge is reversed with respect to the parametrization of
    /// the underlying ParamCurve.
    /// \return \a true if orientation is reversed, \a false otherwise
    virtual bool isReversed() = 0;

    virtual ftFaceBase* face() = 0;
    // Set face pointer
    virtual void setFace(ftFaceBase* face) = 0;

    virtual Go::BoundingBox boundingBox() = 0;
    virtual int entryId() = 0;
    virtual void setEntryId(int id) = 0;

    virtual ftEdgeBase* split(double t) = 0;

    virtual Go::Point point(double t) const = 0;
    virtual Go::Point tangent(double t) const = 0;
    virtual Go::Point normal(double t) const = 0;
    virtual Go::Point normal(double t, Go::Point& face_par_pt, double* face_seed) const = 0;
    virtual void closestPoint(const Go::Point& pt, double& clo_t,
			      Go::Point& clo_pt, double& clo_dist,
			      double const *seed = 0) const = 0;

    virtual void connectAfter(ftEdgeBase* edge);
    virtual void closeLoop(ftEdgeBase* last);
    virtual void disconnectThis();
    virtual void connectTwin(ftEdgeBase* twin, int& status);
    virtual void disconnectTwin();
    bool onBoundary() { return (twin_) ? false : true; }
    /// Consider the 'node' or 'corner' implied by the startpoint
    /// (if at_start_of_edge is true) or endpoint (if it is false).
    /// The vector adjacent is filled with all edges with that 'node'
    /// as a startpoint (indicated by a true at the corresponding place
    /// in the at_start vector) or endpoint (false in at_start).
    /// The edge originally asked for is not included.
    void adjacentEdges(bool at_start_of_edge,
		       std::vector<ftEdgeBase*>& adjacent,
		       std::vector<bool>& at_start);

    tpJointType checkContinuity(ftEdgeBase* nextedge,
				double neighbour, double gap,
				double bend, double kink) const;

    /// Compare orientation of curve and edge
    virtual bool orientationOK() const;

    /// Return edge pointer
    virtual ftEdge* geomEdge() = 0; // The only new functionalty.

    bool checkOverlap(ftEdgeBase *other, double tol, int nmbsample,
		      double& t1, double& t2, double& t3, 
		      double& t4, bool& same_dir,
		      bool no_snap=true) const;

    bool hasConnectivityInfo()
    {
	return connectivity_info_.get() ? true : false;
    }

    std::shared_ptr<FaceConnectivity<ftEdgeBase> > getConnectivityInfo()
      {
	return connectivity_info_;
      }

    void setConnectivityInfo(std::shared_ptr<FaceConnectivity<ftEdgeBase> > info)
    {
      connectivity_info_ = info;
    }

    void resetConnectivityInfo()
    {
      connectivity_info_.reset();
    }


protected:
    ftEdgeBase* next_;
    ftEdgeBase* prev_;
    ftEdgeBase* twin_;
    std::shared_ptr<FaceConnectivity<ftEdgeBase> > connectivity_info_;

};

} // namespace Go

#endif // _FTEDGEBASE_H
