/* \file dab_flock_visual.cpp
 */

#ifndef _dab_flock_visual_h_
#define _dab_flock_visual_h_

#include <array>
#include <Eigen/Dense>
#include "ofShader.h"
#include "ofVbo.h"
#include "ofAppBaseWindow.h"
#include "dab_geom_geometry_group.h"
#include "dab_flock_visual.h"
#include "dab_listener.h"
#include "dab_singleton.h"
#include "dab_event_manager.h"

namespace dab
{
    
namespace flock
{
    
class VisSwarm;
class VisAgentShape;
class VisAgentTrail;
class VisNeighborSpace;
class VisGridSpace;
        
class FlockVisuals : public Singleton<FlockVisuals>, public UpdateListener
{
public:
    FlockVisuals();
    ~FlockVisuals();
    
    void clear();
    
    event::EventManager& event();
    
    const std::vector< VisSwarm* >& visualSwarms() const;
	const std::vector< VisNeighborSpace* >& visualNeighbors() const;
	const std::vector< VisGridSpace* >& visualGrids() const;
	const ofVec3f& displayPosition() const;
	const ofQuaternion& displayOrientation() const;
	const ofQuaternion& displayOrientationChange() const;
	float displayZoom() const;
	const std::array<float,4>& displayColor() const;
    
    void setDisplayPosition(const ofVec3f& pPosition);
    void setDisplayOrientation(const ofQuaternion& pOrientation);
    void setDisplayOrientationChange(const ofQuaternion& pOrientationChange);
    void setDisplayZoom(float pZoom);
    void setDisplayColor(const std::array<float, 4>& pColor);
    
    VisSwarm* visualSwarm( const std::string& pSwarmName );

	void showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName );
	void showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, unsigned int pMaxTrailLength );
    void showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, std::shared_ptr<geom::GeometryGroup> pAgentShape, const std::vector<bool>& pAgentFilled, unsigned int pMaxTrailLength );
	void hideSwarm( const std::string& pSwarmName );
	void hideSwarms();
    
    void setAgentColor( const std::string& pSwarmName, const std::array<float, 4>& pAgentColor );
	void setAgentScale( const std::string& pSwarmName, float pScale );
	void setAgentScale( const std::string& pSwarmName, const ofVec3f& pAgentScale );
	void setAgentLineWidth( const std::string& pSwarmName, float pLineWidth );

	void setTrailColor(const std::string& pSwarmName, const std::array<float, 4>& pTrailColor);
    
    VisGridSpace* visualGridSpace( const std::string& pSpaceName );
    VisNeighborSpace* visualNeighborSpace( const std::string& pSpaceName );
    
    void showSpace( const std::string& pSpaceName );
	void showSpace( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor, float pValueScale = 1.0 );
	void hideSpace( const std::string& pSpaceName );
	void hideSpaces();
    
    void setSpaceColor( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor );
	void setSpaceLineWidth( const std::string& pSpaceName, float pLineWidth );
	void setSpaceValueScale( const std::string& pSpaceName, float pValueScale );
    
    virtual void init();
    virtual void resize(int pWidth, int pHeight);
    virtual void update();
    virtual void notifyUpdate();
    virtual void display();
    
protected:
    static unsigned int sMaxTrailLength;
    static std::shared_ptr<geom::GeometryGroup> sAgentPyramidGeom;
    static std::shared_ptr<geom::GeometryGroup> sAgentSphereGeom;
    static std::string sAgentVertexShaderFileName;
    static std::string sAgentFragmentShaderFileName;
    static std::string sTrailVertexShaderFileName;
    static std::string sTrailFragmentShaderFileName;
    static std::string sGridVertexShaderFileName;
    static std::string sGridFragmentShaderFileName;
    static std::string sNeighborVertexShaderFileName;
    static std::string sNeighborFragmentShaderFileName;
    
    ofShader mAgentShader;
    ofShader mTrailShader;
    ofShader mGridShader;
    ofShader mNeighborShader;
    
    std::array<float,4> mBackgroundColor;
	ofVec3f mViewSize;
	ofVec3f mViewPos;
	ofVec3f mViewResetPos;
	float mZoomVal;
	ofQuaternion mRotQuat;
	ofQuaternion mRotQuat2;
    
    ofMatrix4x4 mProjectionMatrix;
    ofMatrix4x4 mViewMatrix;
    ofMatrix4x4 mModelMatrix;

    std::vector< VisSwarm* > mVisualSwarms;
	std::vector< VisNeighborSpace* > mVisualNeighborSpaces;
	std::vector< VisGridSpace* > mVisualGridSpaces;
    
    ofGLWindowSettings* mWindowSettings;
    
    event::EventManager mEventManager; /// \brief event manager
    
    bool mInitialised;
    bool mSimUpdated;
    bool mLock;
    bool mToggleFullScreen;
    bool mSimDispLock;
    bool mExportImage;
    int mExportImageCount;
    int mCurrentExportImageCount;
    int mImageFileIndex;
    std::string mImageFileName;

    std::shared_ptr<FlockVisuals> mSelf;
    
    void removeVisSwarms();
    void updateVisSwarms();
    
    void removeVisSpaces();
    void updateVisSpaces();
    
    void removeVisNeighborSpaces();
    void removeVisGridSpaces();
    void updateVisNeighborSpaces();
    
    virtual void displayAgents();
    virtual void displayAgentShapes();
    virtual void displayAgentTrails();
    virtual void displaySpaces();
    virtual void displaySpaceNeighbors();
    virtual void displaySpaceGrids();
};
    
};
    
};

#endif