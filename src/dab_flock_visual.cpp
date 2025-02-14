/* \file dab_flock_visual.cpp
*/

#include "dab_flock_visual.h"
#include "dab_geom_poly_line.h"
#include "dab_flock_simulation.h"
#include "dab_flock_swarm.h"
#include "dab_flock_agent.h"
#include "dab_flock_visual_swarm.h"
#include "dab_flock_visual_agent_shape.h"
#include "dab_flock_visual_agent_trail.h"
#include "dab_flock_visual_neighbor_space.h"
#include "dab_flock_visual_grid_space.h"

using namespace dab;
using namespace dab::flock;

unsigned int FlockVisuals::sMaxTrailLength = 200;
std::shared_ptr<geom::GeometryGroup> FlockVisuals::sAgentPyramidGeom( nullptr );
std::shared_ptr<geom::GeometryGroup> FlockVisuals::sAgentSphereGeom( nullptr );
std::string FlockVisuals::sAgentVertexShaderFileName("shaders/agent_shader.vert");
std::string FlockVisuals::sAgentFragmentShaderFileName("shaders/agent_shader.frag");
std::string FlockVisuals::sTrailVertexShaderFileName("shaders/trail_shader.vert");
std::string FlockVisuals::sTrailFragmentShaderFileName("shaders/trail_shader.frag");
std::string FlockVisuals::sGridVertexShaderFileName("shaders/grid_shader.vert");
std::string FlockVisuals::sGridFragmentShaderFileName("shaders/grid_shader.frag");
std::string FlockVisuals::sNeighborVertexShaderFileName("shaders/neighbor_shader.vert");
std::string FlockVisuals::sNeighborFragmentShaderFileName("shaders/neighbor_shader.frag");

FlockVisuals::FlockVisuals()
: mBackgroundColor( {1.0, 1.0, 1.0, 1.0} )
, mViewSize(7.0, 7.0, 7.0)
, mViewResetPos(0.0, 0.0, -15.0)
, mViewPos(0.0, 0.0, -15.0)
, mRotQuat(1.0, 0.0, 0.0, 0.0)
, mZoomVal(50.0) // 0.5
, mExportImage(false)
, mSimDispLock( false )
, mToggleFullScreen( false )
, mWindowSettings( nullptr )
, mInitialised(false)
, mSimUpdated(false)
, mLock(false)
{
    // create default agent geometry
    if(sAgentPyramidGeom == nullptr)
    {
        sAgentPyramidGeom = std::shared_ptr<geom::GeometryGroup>(new geom::GeometryGroup());
        geom::PolyLine* polyLine = new geom::PolyLine();
        polyLine->append( ofVec3f(0.0, -1.0, 1.0) );
        polyLine->append( ofVec3f(0.0, -1.0, -1.0) );
        polyLine->append( ofVec3f(0.0, 1.0, -1.0) );
        polyLine->append( ofVec3f(0.0, 1.0, 1.0) );
        polyLine->append( ofVec3f(0.0, -1.0, 1.0) );
        polyLine->append( ofVec3f(2.0, 0.0, 0.0) );
        polyLine->append( ofVec3f(0.0, -1.0, -1.0) );
        polyLine->append( ofVec3f(2.0, 0.0, 0.0) );
        polyLine->append( ofVec3f(0.0, 1.0, -1.0) );
        polyLine->append( ofVec3f(2.0, 0.0, 0.0) );
        polyLine->append( ofVec3f(0.0, 1.0, 1.0) );
        polyLine->append( ofVec3f(2.0, 0.0, 0.0) );
        
        sAgentPyramidGeom->addGeometry(polyLine);
    }
    
    if(sAgentSphereGeom == nullptr)
    {
        sAgentSphereGeom = std::shared_ptr<geom::GeometryGroup>(new geom::GeometryGroup());
        
        int vertexCount = 10;
        
        geom::PolyLine* polyLine1 = new geom::PolyLine();
        geom::PolyLine* polyLine2 = new geom::PolyLine();
        geom::PolyLine* polyLine3 = new geom::PolyLine();
        
        for(int vI=0; vI<vertexCount; ++vI)
        {
            float angle = 2.0 * PI / static_cast<float>(vertexCount - 1) * static_cast<float>(vI);
            polyLine1->append(ofVec3f(cos(angle), sin(angle), 0.0));
            polyLine2->append(ofVec3f(cos(angle), 0.0, sin(angle)));
            polyLine3->append(ofVec3f(0.0, cos(angle), sin(angle)));
        }
        
        sAgentSphereGeom->addGeometry(polyLine1);
        sAgentSphereGeom->addGeometry(polyLine2);
        sAgentSphereGeom->addGeometry(polyLine3);
    }
    
    mSelf = std::shared_ptr<FlockVisuals>(this);
}

FlockVisuals::~FlockVisuals()
{
 // TODO
}

void
FlockVisuals::clear()
{
    Simulation::get().removeListener(mSelf);
    mSelf.reset();
}

event::EventManager&
FlockVisuals::event()
{
    return mEventManager;
}
    
const std::vector< VisSwarm* >&
FlockVisuals::visualSwarms() const
{
    return mVisualSwarms;
}

const std::vector< VisNeighborSpace* >&
FlockVisuals::visualNeighbors() const
{
    return mVisualNeighborSpaces;
}

const std::vector< VisGridSpace* >&
FlockVisuals::visualGrids() const
{
    return mVisualGridSpaces;
}

const ofVec3f&
FlockVisuals::displayPosition() const
{
    return mViewPos;
}

const ofQuaternion&
FlockVisuals::displayOrientation() const
{
    return mRotQuat;
}

const ofQuaternion&
FlockVisuals::displayOrientationChange() const
{
    return mRotQuat2;
}

float
FlockVisuals::displayZoom() const
{
    return mZoomVal;
}

const std::array<float,4>&
FlockVisuals::displayColor() const
{
    return mBackgroundColor;
}

void
FlockVisuals::setDisplayPosition(const ofVec3f& pPosition)
{
    mViewPos = pPosition;

	//std::cout << "setDisplayPosition " << mViewPos[0] << " " << mViewPos[1] << " " << mViewPos[2] << "\n";
}

void
FlockVisuals::setDisplayOrientation(const ofQuaternion& pOrientation)
{
    mRotQuat = pOrientation;

	//std::cout << "setDisplayOrientation " << mRotQuat[0] << " " << mRotQuat[1] << " " << mRotQuat[2] << " " << mRotQuat[3] << "\n";
}

void
FlockVisuals::setDisplayOrientationChange(const ofQuaternion& pOrientationChange)
{
    mRotQuat2 = pOrientationChange;
}

void
FlockVisuals::setDisplayZoom(float pZoom)
{
    mZoomVal = pZoom;

	//std::cout << "setDisplayZoom " << mZoomVal << "\n";
}

void
FlockVisuals::setDisplayColor(const std::array<float, 4>& pColor)
{
    mBackgroundColor = pColor;
}

VisSwarm*
FlockVisuals::visualSwarm( const std::string& pSwarmName )
{
    int visSwarmCount = mVisualSwarms.size();
    for(int sI=0; sI<visSwarmCount; ++sI)
    {
        if(mVisualSwarms[sI]->swarmName() == pSwarmName) return mVisualSwarms[sI];
    }
    return nullptr;
}

void
FlockVisuals::showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName )
{
    showSwarm(pSwarmName, pPosParName, pVelParName, sMaxTrailLength);
}

void
FlockVisuals::showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, unsigned int pMaxTrailLength )
{
    if(pVelParName.empty() == true)
    {
        showSwarm(pSwarmName, pPosParName, pVelParName, sAgentSphereGeom, { false, false, false }, pMaxTrailLength);
    }
    else
    {
        showSwarm(pSwarmName, pPosParName, pVelParName, sAgentPyramidGeom, { false }, pMaxTrailLength);
    }
}

void
FlockVisuals::showSwarm( const std::string& pSwarmName, const std::string& pPosParName, const std::string& pVelParName, std::shared_ptr<geom::GeometryGroup> pAgentShape, const std::vector<bool>& pAgentFilled, unsigned int pMaxTrailLength )
{
    while(mLock == true)  std::this_thread::sleep_for(std::chrono::microseconds(10));
    mLock = true;
    
    VisSwarm* _visSwarm = visualSwarm(pSwarmName);
    
    if(_visSwarm == nullptr)
    {
        _visSwarm = new VisSwarm(pSwarmName, pPosParName, pVelParName);
        _visSwarm->createAgentShape(pAgentShape, pAgentFilled);
        _visSwarm->createAgentTrail(pMaxTrailLength);
        
        mVisualSwarms.push_back(_visSwarm);
    }
    
    mLock = false;
}

void
FlockVisuals::hideSwarm( const std::string& pSwarmName )
{
	while(mLock == true) std::this_thread::sleep_for(std::chrono::microseconds(10));
    mLock = true;

	VisSwarm* visSwarm;
	int visSwarmCount = mVisualSwarms.size();
    
    Simulation& simulation = Simulation::get();
	
	for(int sI = visSwarmCount - 1; sI >= 0; --sI)
	{
		visSwarm = mVisualSwarms[sI];
		
		if( visSwarm->swarmName() == pSwarmName )
		{
            mVisualSwarms.erase( mVisualSwarms.begin() + sI );
            delete visSwarm;
            mLock = false;
            return;
		}
	}
    
	mLock = false;
}

void
FlockVisuals::hideSwarms()
{
	while(mLock == true) std::this_thread::sleep_for(std::chrono::microseconds(10));
    mLock = true;

	VisSwarm* visSwarm;
	int visSwarmCount = mVisualSwarms.size();
    
    Simulation& simulation = Simulation::get();
	
	for(int sI = visSwarmCount - 1; sI >= 0; --sI)
	{
		visSwarm = mVisualSwarms[sI];
        mVisualSwarms.erase( mVisualSwarms.begin() + sI );
        delete visSwarm;
	}
    
	mLock = false;
}

void
FlockVisuals::setAgentColor( const std::string& pSwarmName, const std::array<float, 4>& pAgentColor )
{
    VisSwarm* _visSwarm = visualSwarm(pSwarmName);
    
    if(_visSwarm == nullptr) return;
    
    _visSwarm->setAgentColor(pAgentColor);
}

void
FlockVisuals::setAgentScale( const std::string& pSwarmName, float pScale )
{
    VisSwarm* _visSwarm = visualSwarm(pSwarmName);
    
    if(_visSwarm == nullptr) return;
    
    _visSwarm->setAgentScale(ofVec3f(pScale,pScale,pScale));
}

void
FlockVisuals::setAgentScale( const std::string& pSwarmName, const ofVec3f& pAgentScale )
{
    VisSwarm* _visSwarm = visualSwarm(pSwarmName);
    
    if(_visSwarm == nullptr) return;
    
    _visSwarm->setAgentScale(pAgentScale);
}

void
FlockVisuals::setAgentLineWidth( const std::string& pSwarmName, float pLineWidth )
{
    VisSwarm* _visSwarm = visualSwarm(pSwarmName);
    
    if(_visSwarm == nullptr) return;
    
    _visSwarm->setAgentLineWidth(pLineWidth);
}

void
FlockVisuals::setTrailColor(const std::string& pSwarmName, const std::array<float, 4>& pTrailColor)
{
	VisSwarm* _visSwarm = visualSwarm(pSwarmName);

	if (_visSwarm == nullptr) return;

	_visSwarm->setTrailColor(pTrailColor);
}


VisGridSpace*
FlockVisuals::visualGridSpace( const std::string& pSpaceName )
{
    int visGridSpaceCount = mVisualGridSpaces.size();
    for(int vgI=0; vgI<visGridSpaceCount; ++vgI)
    {
        if(mVisualGridSpaces[vgI]->spaceName() == pSpaceName) return mVisualGridSpaces[vgI];
    }
    
    return nullptr;
}

VisNeighborSpace*
FlockVisuals::visualNeighborSpace( const std::string& pSpaceName )
{
    int visNeighborSpaceCount = mVisualNeighborSpaces.size();
    for(int vnI=0; vnI<visNeighborSpaceCount; ++vnI)
    {
        if(mVisualNeighborSpaces[vnI]->spaceName() == pSpaceName) return mVisualNeighborSpaces[vnI];
    }
    
    return nullptr;
}

void
FlockVisuals::showSpace( const std::string& pSpaceName )
{
    while(mLock == true) std::this_thread::sleep_for(std::chrono::microseconds(10));
    mLock = true;
    
    // test if space is already registered
    if( visualNeighborSpace(pSpaceName) != nullptr || visualGridSpace(pSpaceName) != nullptr ) return;
	
	// register space
	if( Simulation::get().space().checkSpace(pSpaceName) )
	{
		space::SpaceAlg* spaceAlg = Simulation::get().space().space(pSpaceName)->spaceAlg();
		
		if(dynamic_cast<space::GridAlg*>(spaceAlg) != nullptr) // register as grid space
		{
			VisGridSpace* visualSpace = new VisGridSpace(pSpaceName);
			mVisualGridSpaces.push_back( visualSpace );
		}
		else // register as neighbor space
		{
			VisNeighborSpace* visualSpace = new VisNeighborSpace(pSpaceName);
			mVisualNeighborSpaces.push_back( visualSpace );
		}
	}
    
    mLock = false;
}

void
FlockVisuals::showSpace( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor, float pValueScale )
{
    // TODO
}

void
FlockVisuals::hideSpace( const std::string& pSpaceName )
{
    // TODO
}

void
FlockVisuals::hideSpaces()
{
    // TODO
}

void
FlockVisuals::setSpaceColor( const std::string& pSpaceName, const std::array<float, 4>& pSpaceColor )
{
    VisNeighborSpace* _visNeighborSpace = visualNeighborSpace(pSpaceName);
    VisGridSpace* _visGridSpace = visualGridSpace(pSpaceName);
    
    if(_visNeighborSpace != nullptr)
    {
         _visNeighborSpace->setColor(pSpaceColor);
    }
    
    if(_visGridSpace != nullptr)
    {
        _visGridSpace->setColor(pSpaceColor);
    }
}

void
FlockVisuals::setSpaceLineWidth( const std::string& pSpaceName, float pLineWidth )
{
    VisNeighborSpace* _visNeighborSpace = visualNeighborSpace(pSpaceName);
    VisGridSpace* _visGridSpace = visualGridSpace(pSpaceName);
    
    if(_visNeighborSpace != nullptr)
    {
        _visNeighborSpace->setLineWidth(pLineWidth);
    }
    
    if(_visGridSpace != nullptr)
    {
        _visGridSpace->setLineWidth(pLineWidth);
    }
}

void
FlockVisuals::setSpaceValueScale( const std::string& pSpaceName, float pValueScale )
{
    VisGridSpace* _visGridSpace = visualGridSpace(pSpaceName);
    
    if(_visGridSpace != nullptr)
    {
        _visGridSpace->setValueScale(pValueScale);
    }
}

void
FlockVisuals::notifyUpdate()
{
    mSimUpdated = true;
}

void
FlockVisuals::init()
{
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
    
    // create projection matrices
    ofPoint screenSize = ofGetMainLoop()->getCurrentWindow()->getWindowSize();
    mProjectionMatrix.makePerspectiveMatrix(mZoomVal * 80, mViewSize.x / mViewSize.y, 1.0, 5000.0);
    //mProjectionMatrix.makeOrthoMatrix(-1.0, 1.0, -1.0, 1.0, 0.0, 1000.0);
    
    mViewMatrix.makeIdentityMatrix();
    mViewMatrix.rotate(mRotQuat);
    mViewMatrix.translate(mViewPos);

    mModelMatrix.makeIdentityMatrix();
    
    // create shaders
    bool success = mAgentShader.load(sAgentVertexShaderFileName, sAgentFragmentShaderFileName);
    if(success == false)
    {
        std::cout << "failed to load agent shader\n";
        std::exit(1);
    }
    
    success = mTrailShader.load(sTrailVertexShaderFileName, sTrailFragmentShaderFileName);
    if(success == false)
    {
        std::cout << "failed to load trail shader\n";
        std::exit(1);
    }
    
    success = mGridShader.load(sGridVertexShaderFileName, sGridFragmentShaderFileName);
    if(success == false)
    {
        std::cout << "failed to load grid shader\n";
        std::exit(1);
    }
    
    success = mNeighborShader.load(sNeighborVertexShaderFileName, sNeighborFragmentShaderFileName);
    if(success == false)
    {
        std::cout << "failed to load neighbor shader\n";
        std::exit(1);
    }
    
    mAgentShader.begin();
    mAgentShader.setUniformMatrix4f("ProjectionMatrix", mProjectionMatrix);
    mAgentShader.setUniformMatrix4f("ViewMatrix", mViewMatrix);
    mAgentShader.end();
    
    mTrailShader.begin();
    mTrailShader.setUniformMatrix4f("ProjectionMatrix", mProjectionMatrix);
    mTrailShader.setUniformMatrix4f("ViewMatrix", mViewMatrix);
    mTrailShader.end();
    
    mGridShader.begin();
    mGridShader.setUniformMatrix4f("ProjectionMatrix", mProjectionMatrix);
    mGridShader.setUniformMatrix4f("ViewMatrix", mViewMatrix);
    mGridShader.end();
    
    mNeighborShader.begin();
    mNeighborShader.setUniformMatrix4f("ProjectionMatrix", mProjectionMatrix);
    mNeighborShader.setUniformMatrix4f("ViewMatrix", mViewMatrix);
    mNeighborShader.end();
    
    glEnable(GL_DEPTH_TEST);
    
    Simulation::get().addListener(mSelf);
}

void
FlockVisuals::resize(int pWidth, int pHeight)
{
    mViewSize.x = pWidth;
    mViewSize.y = pHeight;
    
    mProjectionMatrix.makePerspectiveMatrix(mZoomVal * 80, mViewSize.x / mViewSize.y, 1.0, 5000.0);
}

void
FlockVisuals::update()
{
    Simulation& simulation = Simulation::get();
    
    if(mSimUpdated == true)
    {
        if( simulation.paused() == false ) simulation.switchPaused();
        
        mEventManager.update();
        
        removeVisSwarms();
        updateVisSwarms();
        
        removeVisSpaces();
        updateVisSpaces();
        
        if( simulation.paused() == true ) simulation.switchPaused();
        
        mSimUpdated = false;
    }
	   
    if(simulation.paused() == false)
    {
        mRotQuat = mRotQuat2 * mRotQuat;
        mRotQuat.normalize();
    }
    
    display();
}

void
FlockVisuals::display()
{
    if(mInitialised == false) init();
    
    glClearColor(mBackgroundColor[0], mBackgroundColor[1], mBackgroundColor[2], mBackgroundColor[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    displayAgents();
    displaySpaces();
}

void
FlockVisuals::removeVisSwarms()
{
	VisSwarm* visSwarm;
	int visSwarmCount = mVisualSwarms.size();
    
    Simulation& simulation = Simulation::get();
	
	for(int sI = visSwarmCount - 1; sI >= 0; --sI)
	{
		visSwarm = mVisualSwarms[sI];
        
		if( simulation.checkSwarm( visSwarm->swarmName() ) == false )
		{
            mVisualSwarms.erase( mVisualSwarms.begin() + sI );
            delete visSwarm;
		}
	}
}

void
FlockVisuals::updateVisSwarms()
{
	int visSwarmCount = mVisualSwarms.size();
	
	for(int sI = visSwarmCount - 1; sI >= 0; --sI)
	{
		mVisualSwarms[sI]->update();
	}
}

void
FlockVisuals::removeVisSpaces()
{
	removeVisNeighborSpaces();
	removeVisGridSpaces();
}

void
FlockVisuals::updateVisSpaces()
{
    updateVisNeighborSpaces();
}

void
FlockVisuals::removeVisNeighborSpaces()
{
	VisNeighborSpace* visualNeighborSpace;
	int visNeighborSpaceCount = mVisualNeighborSpaces.size();
	
	for(int sI = visNeighborSpaceCount - 1; sI >= 0; --sI)
	{
		visualNeighborSpace = mVisualNeighborSpaces[sI];
		
		if( Simulation::get().space().checkSpace( visualNeighborSpace->spaceName() ) == false )
		{
			mVisualNeighborSpaces.erase( mVisualNeighborSpaces.begin() + sI );
			delete visualNeighborSpace;
		}
	}
}

void
FlockVisuals::removeVisGridSpaces()
{
	VisGridSpace* visualGridSpace;
	int visGridSpaceCount = mVisualGridSpaces.size();
	
	for(int sI = visGridSpaceCount - 1; sI >= 0; --sI)
	{
		visualGridSpace = mVisualGridSpaces[sI];
		
		if( Simulation::get().space().checkSpace( visualGridSpace->spaceName() ) == false )
		{
			mVisualGridSpaces.erase( mVisualGridSpaces.begin() + sI );
			delete visualGridSpace;
		}
	}
}

void
FlockVisuals::updateVisNeighborSpaces()
{
	VisNeighborSpace* visualNeighborSpace;
	int visNeighborSpaceCount = mVisualNeighborSpaces.size();
    
    for(int sI = 0; sI < visNeighborSpaceCount; ++sI)
    {
        visualNeighborSpace = mVisualNeighborSpaces[sI];
        visualNeighborSpace->create();
    }
    
}

void
FlockVisuals::displayAgents()
{
	displayAgentShapes();
	displayAgentTrails();
}

void
FlockVisuals::displayAgentShapes()
{
    int visSwarmCount = mVisualSwarms.size();
    
    mAgentShader.begin();
    
    for(int vsI=0; vsI<visSwarmCount; ++vsI)
    {
        mVisualSwarms[vsI]->displayAgents(mAgentShader);
    }
    
    mAgentShader.end();
}

void
FlockVisuals::displayAgentTrails()
{
    int visSwarmCount = mVisualSwarms.size();
    
    mTrailShader.begin();
    
    for(int vsI=0; vsI<visSwarmCount; ++vsI)
    {
        mVisualSwarms[vsI]->displayTrails(mTrailShader);
    }
    
    mTrailShader.end();
}

void
FlockVisuals::displaySpaces()
{
	displaySpaceNeighbors();
	displaySpaceGrids();
}

void
FlockVisuals::displaySpaceNeighbors()
{
    int visNeighborCount = mVisualNeighborSpaces.size();
    
    mNeighborShader.begin();
    
    for(int vnI=0; vnI<visNeighborCount; ++vnI)
    {
        mVisualNeighborSpaces[vnI]->display(mNeighborShader);
    }
    
    mNeighborShader.end();
}

void
FlockVisuals::displaySpaceGrids()
{
    int visGridCount = mVisualGridSpaces.size();
    
    mGridShader.begin();
    
    for(int vgI=0; vgI<visGridCount; ++vgI)
    {
        mVisualGridSpaces[vgI]->display(mGridShader);
    }
    
    mGridShader.end();
}
