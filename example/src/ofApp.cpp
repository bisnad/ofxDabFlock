#include "ofApp.h"
#include "dab_flock_parameter.h"
#include "dab_flock_euler_integration.h"
#include "dab_flock_agent.h"
#include "dab_flock_swarm.h"
#include "dab_flock_simulation.h"
#include "dab_flock_com.h"
#include "dab_flock_osc_control.h"
#include "dab_space_includes.h"
#include "dab_flock_behavior_includes.h"
#include "dab_flock_visual.h"
#include "dab_flock_com.h"
#include "dab_math_roesseler_field_algorithm.h"
#include "dab_geom_line.h"
#include "ofTrueTypeFont.h"
#include "dab_flock_text_tools.h"
#include "dab_flock_serialize.h"
using namespace dab;
using namespace dab::flock;

//--------------------------------------------------------------
void ofApp::setup()
{
	//ofTrueTypeFont testFont;
	//testFont.load("Batang.ttf", 160, true, true, true);
	//std::string string = "A";

	//std::vector<ofPath> textChars = testFont.getStringAsPoints(string, false, false);

	//std::cout << "textCharCount " << textChars.size() << "\n";
	//for (int cI = 0; cI < textChars.size(); ++cI)
	//{
	//	ofPath& testCharContour = textChars[cI];

	//	std::vector<ofPath::Command> pathCommands = testCharContour.getCommands();
	//	std::cout << "pathCommands " << pathCommands.size() << "\n";
	//	const std::vector<ofPolyline>& charOutlines = testCharContour.getOutline();
	//	int outlineCount = charOutlines.size();
	//	std::cout << "outlineCount " << outlineCount << "\n";
	//	for (int oI = 0; oI < outlineCount; ++oI)
	//	{
	//		std::cout << "oI " << oI << " s " << charOutlines[oI].size() << "\n";
	//	}
	//}

	//char letter = '$';
	//bool vflip = true;
	//bool filled = false;
	//ofPath testCharContour = testFont.getCharacterAsPoints(letter, vflip, filled);

	//ofPath testCharContour = testFont.getCharacterAsPoints(letter, vflip, filled);

	//std::vector<ofPath::Command> pathCommands = testCharContour.getCommands();
	//std::cout << "pathCommands " << pathCommands.size() << "\n";
	//const std::vector<ofPolyline>& charOutlines = testCharContour.getOutline();
	//int outlineCount = charOutlines.size();
	//std::cout << "outlineCount " << outlineCount << "\n";
	//for (int oI = 0; oI < outlineCount; ++oI)
	//{
	//	std::cout << "oI " << oI << " s " << charOutlines[oI].size() << "\n";
	//}


	SerializeTools& serializeTools = SerializeTools::get();

	//    // debug
	//    ofTrueTypeFont font;
	//    bool success;
	//    success = font.load("/Library/Fonts/InaiMathi.ttf", 2, true, true, true, 0.3, 0);
	//    font.setLineHeight( font.getLineHeight() * 2.0 );
	//    font.setLetterSpacing(font.getLetterSpacing() * 2.0);
	//    font.setSpaceSize(20.0);
	//
	//
	//    std::cout << "success " << success << "\n";
	//
	//    std::vector<ofTTFCharacter> ttfChars = font.getStringAsPoints("h h\nh h");
	//    int charCount = ttfChars.size();
	//
	//    std::cout << "charCount " << charCount << "\n";
	//
	//    ofVec3f textMinPos(FLT_MAX, FLT_MAX, FLT_MAX);
	//    ofVec3f textMaxPos(-FLT_MIN, -FLT_MIN, -FLT_MIN);
	//    ofVec3f charMinPos(FLT_MAX, FLT_MAX, FLT_MAX);
	//    ofVec3f charMaxPos(-FLT_MIN, -FLT_MIN, -FLT_MIN);
	//
	//    for(int cI=0; cI<charCount; ++cI)
	//    {
	//        ofTTFCharacter& ttfChar = ttfChars[cI];
	//
	//        const vector<ofPolyline>& ttfOulines = ttfChar.getOutline();
	//
	//        int outlineCount = ttfOulines.size();
	//
	//        std::cout << "cI " << cI << " outlineCount " << outlineCount << "\n";
	//
	//        ofVec3f charMinPos = ofVec3f(FLT_MAX, FLT_MAX, FLT_MAX);
	//        ofVec3f charMaxPos = ofVec3f(-FLT_MIN, -FLT_MIN, -FLT_MIN);
	//
	//        for(int oI=0; oI<outlineCount; ++oI)
	//        {
	//            const ofPolyline& outline = ttfOulines[oI];
	//
	//            int pointCount = outline.size();
	//
	//            std::cout << "cI " << cI << " oI " << oI << " pointCount " << pointCount << "\n";
	//
	//            for(int pI=0; pI<pointCount; ++pI)
	//            {
	//                const ofPoint& point = outline[pI];
	//
	//                //std::cout << "cI " << cI << " oI " << oI << " pI " << pI << " pos " << point.x << " " << point.y << " " << point.z << "\n";
	//
	//                if(point.x < textMinPos[0]) textMinPos[0] = point.x;
	//                if(point.y < textMinPos[1]) textMinPos[1] = point.y;
	//                if(point.z < textMinPos[2]) textMinPos[2] = point.z;
	//                if(point.x > textMaxPos[0]) textMaxPos[0] = point.x;
	//                if(point.y > textMaxPos[1]) textMaxPos[1] = point.y;
	//                if(point.z > textMaxPos[2]) textMaxPos[2] = point.z;
	//
	//                if(point.x < charMinPos[0]) charMinPos[0] = point.x;
	//                if(point.y < charMinPos[1]) charMinPos[1] = point.y;
	//                if(point.z < charMinPos[2]) charMinPos[2] = point.z;
	//                if(point.x > charMaxPos[0]) charMaxPos[0] = point.x;
	//                if(point.y > charMaxPos[1]) charMaxPos[1] = point.y;
	//                if(point.z > charMaxPos[2]) charMaxPos[2] = point.z;
	//            }
	//        }
	//
	//        std::cout << "char " << cI << " Min " << charMinPos[0] << " " << charMinPos[1] << " " << charMinPos[2] << "\n";
	//        std::cout << "char " << cI << " Max " << charMaxPos[0] << " " << charMaxPos[1] << " " << charMaxPos[2] << "\n";
	//    }
	//
	//    std::cout << "text Min " << textMinPos[0] << " " << textMinPos[1] << " " << textMinPos[2] << "\n";
	//    std::cout << "text Max " << textMaxPos[0] << " " << textMaxPos[1] << " " << textMaxPos[2] << "\n";
	//
	//    std::exit(0);
	//
	//    // debug done

	try
	{
		Simulation& simulation = Simulation::get();
		simulation.setUpdateInterval(1.0);

		simulation.com().createOscControl(7400, "127.0.0.1", 7800);
		//simulation.com().createSender("FlockSender", "127.0.0.1", 7500, false);
		simulation.com().createSender("FlockSender", "127.0.0.1", 7500, true);

		// create agent space
		//simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("agent_position", new space::ANNAlg(3))));
		simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("agent_position", new space::KDTreeAlg(3))));
		//simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("agent_position", new space::NTreeAlg(3))));

		// create grid space
		space::SpaceGrid* forceGrid = new space::SpaceGrid(3, Array<unsigned int>{ 20, 20, 5 }, Eigen::Vector3f(-5.0, -5.0, -5.0), Eigen::Vector3f(5.0, 5.0, 5.0));
		simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("forcegrid", new space::GridAlg(forceGrid, space::GridAlg::AvgLocationMode, space::GridAlg::NoUpdateMode))));

		// create shape space and line object
		space::RTreeAlg* shapeSpaceAlg = new space::RTreeAlg(Eigen::Vector3f(-5.0, -5.0, -5.0), Eigen::Vector3f(5.0, 5.0, 5.0));
		//shapeSpaceAlg->setClosestPointType(space::ClosestPointShape);
		simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("shapespace", shapeSpaceAlg)));
		std::shared_ptr<geom::GeometryGroup> lineGeomGroup(new geom::GeometryGroup());
		lineGeomGroup->addGeometry(new geom::Line(glm::vec3(-1.0, -1.0, 0.0), glm::vec3(1.0, -1.0, 0.0)));
		lineGeomGroup->addGeometry(new geom::Line(glm::vec3(1.0, -1.0, 0.0), glm::vec3(0.0, 1.0, 0.0)));
		lineGeomGroup->addGeometry(new geom::Line(glm::vec3(0.0, 1.0, 0.0), glm::vec3(-1.0, -1.0, 0.0)));
		//        lineGeomGroup->addGeometry(new geom::Line(glm::vec3(0.1875, -0.1875, 0.0), glm::vec3( 0.1875, 0.0, 0.0)));
		//        lineGeomGroup->addGeometry(new geom::Line(glm::vec3(0.1875, 0.0, 0.0), glm::vec3( 0.09375, 0.0, 0.0)));
		//        lineGeomGroup->addGeometry(new geom::Line(glm::vec3(0.09375, 0.0, 0.0), glm::vec3( 0.09375, -0.875, 0.0)));
		//        lineGeomGroup->addGeometry(new geom::Line(glm::vec3(0.09375, -0.875, 0.0), glm::vec3( 0.1875, -0.1875, 0.0)));
		space::SpaceShape* lineShape = new space::SpaceShape(lineGeomGroup);
		simulation.space().addObject("shapespace", lineShape, true);

		// todo: getting text outlines doesn't work
		// create text space and text object
		space::RTreeAlg* textSpaceAlg = new space::RTreeAlg(Eigen::Vector3f(-5.0, -5.0, -5.0), Eigen::Vector3f(5.0, 5.0, 5.0));
		simulation.space().addSpace(std::shared_ptr<space::Space>(new space::Space("textspace", textSpaceAlg)));
		TextTools& textTools = TextTools::get();
		textTools.createFont("defaultfont", "C:/Windows/Fonts/Ebrima.ttf");
		textTools.createText("char0", "A", "defaultfont");
		space::SpaceShape* textShape = textTools.text("char0");
		textShape->setScale(Eigen::Vector3f(5.0, 5.0, 5.0));
		textShape->setPosition(Eigen::Vector3f(0.0, 2.0, 0.0));
		simulation.space().addObject("textspace", textShape, true);

		math::VectorField<float>& vectorField = forceGrid->vectorField();
		math::RoesselerFieldAlgorithm fieldAlg;
		fieldAlg.setVectorField(&vectorField);
		fieldAlg.update();

		Swarm* swarm = new Swarm("swarm");
		swarm->addParameter("position", { 0.0, 0.0, 0.0 });
		swarm->assignNeighbors("position", "agent_position", true, new space::NeighborGroupAlg(3.0, 8, true));
		swarm->assignNeighbors("position", "forcegrid", false, new space::NeighborGroupAlg(2.5, 1, true));
		swarm->assignNeighbors("position", "shapespace", false, new space::NeighborGroupAlg(60.0, 1, true));
		swarm->assignNeighbors("position", "textspace", false, new space::NeighborGroupAlg(60.0, 1, true));
		swarm->addParameter("velocity", { 0.0, 0.0, 0.0 });
		swarm->addParameter("acceleration", { 0.0, 0.0, 0.0 });
		swarm->addParameter("force", { 0.0, 0.0, 0.0 });
		swarm->addParameter("mass", { 0.1f });

		swarm->addBehavior("resetForce", ResetBehavior("", "force"));

		swarm->addBehavior("randomize", RandomizeBehavior("", "force"));
		swarm->set("randomize_range", { 0.0001f, 0.0001f, 0.0001f });

		swarm->addBehavior("damping", DampingBehavior("velocity", "force"));
		swarm->set("damping_prefVelocity", { 0.5 });
		swarm->set("damping_amount", { 0.1f });

		swarm->addBehavior("targetPos", TargetParameterBehavior("position", "force"));
		swarm->set("targetPos_target", { 0.0, 0.0, 0.0 });
		swarm->set("targetPos_adapt", { 1.0, 1.0, 1.0 });
		swarm->set("targetPos_amount", 0.0); // 0.52 // 0.0
		swarm->set("targetPos_absolute", 0.0);

		swarm->addBehavior("targetVel", TargetParameterBehavior("velocity", "force"));
		swarm->set("targetVel_target", { 1.0, 0.0, 0.0 });
		swarm->set("targetVel_adapt", { 1.0, 1.0, 1.0 });
		swarm->set("targetVel_amount", 0.0); // 0.52 // 0.0
		swarm->set("targetVel_absolute", 0.0);

		swarm->addBehavior("circular", CircularBehavior("position velocity", "force"));
		swarm->set("circular_centerPosition", { 0.0, 0.0, 0.0 });
		swarm->set("circular_innerRadius", 2.0);
		swarm->set("circular_outerRadius", 2.0);
		swarm->set("circular_ortAmount", 0.0);
		swarm->set("circular_tanAmount", 0.0);

		//swarm->addBehavior("shapeFollow", LineFollowBehavior("position@shapespace", "force"));
		//swarm->set("shapeFollow_minDist", 0.0);
		//swarm->set("shapeFollow_maxDist", 60.0);
		//swarm->set("shapeFollow_contourMaintainDist", 5.0);
		//swarm->set("shapeFollow_tanAmount", 0.02);
		//swarm->set("shapeFollow_ortAmount", 2.0);
		//swarm->set("shapeFollow_amount", 1.0);

		swarm->addBehavior( "textFollow", LineFollowBehavior("position@textspace","force"));
		swarm->set("textFollow_minDist", 0.0);
		swarm->set("textFollow_maxDist", 60.0);
		swarm->set("textFollow_contourMaintainDist", 5.0);
		swarm->set("textFollow_tanAmount", 0.02);
		swarm->set("textFollow_ortAmount", 2.0);
		swarm->set("textFollow_amount", 1.0);

		//swarm->addBehavior( "forcegrid", GridAvgBehavior( "position@forcegrid", "force" ) );
		//swarm->set("forcegrid_amount", { 0.01 } );

		//swarm->addBehavior("cohesion", CohesionBehavior("position@agent_position", "force"));
		//swarm->set("cohesion_minDist", { 0.0 });
		//swarm->set("cohesion_maxDist", { 3.0 });
		//swarm->set("cohesion_amount", { 0.05f });

		//swarm->addBehavior("alignment", AlignmentBehavior("position@agent_position:velocity", "force"));
		//swarm->set("alignment_minDist", { 0.0 });
		//swarm->set("alignment_maxDist", { 3.0 });
		//swarm->set("alignment_amount", { 0.05f });

		//swarm->addBehavior("evasion", EvasionBehavior("position@agent_position", "force"));
		//swarm->set("evasion_maxDist", { 0.2f });
		//swarm->set("evasion_amount", { 0.2f });

		swarm->addBehavior("boundaryWrap", BoundaryWrapBehavior("position", "position"));
		swarm->set("boundaryWrap_lowerBoundary", { -5.0, -5.0, -5.0 });
		swarm->set("boundaryWrap_upperBoundary", { 5.0, 5.0, 5.0 });

		//        swarm->addBehavior( "boundaryRepulsion", BoundaryRepulsionBehavior( "position", "force" ) );
		//		swarm->set( "boundaryRepulsion_lowerBoundary", { -5.0, -5.0, -5.0 } );
		//		swarm->set( "boundaryRepulsion_upperBoundary", { 5.0, 5.0, 5.0 } );
		//		swarm->set( "boundaryRepulsion_maxDist", { 2.5 } );
		//		swarm->set( "boundaryRepulsion_amount", { 0.13 } );

		swarm->addBehavior("boundaryMirror", BoundaryMirrorBehavior("position velocity force", "velocity force"));
		swarm->set("boundaryMirror_lowerBoundary", { -5.0, -5.0, -5.0 });
		swarm->set("boundaryMirror_upperBoundary", { 5.0, 5.0, 5.0 });
		swarm->set("boundaryMirror_active", 0.0);

		swarm->addBehavior("acceleration", AccelerationBehavior("mass velocity force", "acceleration"));
		swarm->set("acceleration_maxAngularAcceleration", { 1.0, 1.0, 1.0 });

		swarm->addBehavior("integration", EulerIntegration("position velocity acceleration", "position velocity"));
		swarm->set("integration_timestep", { 0.1f });

		//swarm->addBehavior( "print", ParameterPrintBehavior("position", "") );

		swarm->addAgents(100);
		swarm->randomize("position", { -1.0, -1.0, -1.0 }, { 1.0, 1.0, 1.0 });

		//std::cout << "swarm\n" << swarm->info(2) << "\n";

		//simulation.com().registerParameter("FlockSender", "swarm", "position" );
		//simulation.com().registerParameter("FlockSender", "swarm", "position", Eigen::Vector3f(-5.0, -5.0, -5.0), Eigen::Vector3f(5.0, 5.0, 5.0) );
		//simulation.com().registerParameter("FlockSender", "swarm", "position", std::array<int, 2>( { 5, 10 } ) );
		//simulation.com().registerParameter("FlockSender", "swarm", "position", swarm->agentCount() );
		simulation.com().registerParameter("FlockSender", "swarm", "position", swarm->agentCount(), Eigen::Vector3f(-5.0, -5.0, -5.0), Eigen::Vector3f(5.0, 5.0, 5.0));
		//simulation.com().registerParameter("FlockSender", "swarm", "velocity", swarm->agentCount());

		FlockVisuals& visuals = FlockVisuals::get();
		visuals.showSwarm("swarm", "position", "velocity", 10);

		//visuals.showSwarm("swarm", "position", "");
		visuals.setAgentScale("swarm", 0.05);
		//        visuals.showSpace("agent_position");
		//        visuals.showSpace("forcegrid");
		//        visuals.setSpaceColor("forcegrid", std::array<float,4>( { 0.0, 0.0, 0.0, 0.2 } ));
		//        visuals.setSpaceValueScale("forcegrid", 0.1);

		simulation.start();
		simulation.setUpdateInterval(20.0);
	}
	catch (dab::Exception& e)
	{
		std::cout << e << "\n";
	}

	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update()
{
	//std::cout << "ofApp::update() begin\n";

	//Simulation::get().update();
	FlockVisuals::get().update();

	//std::cout << "ofApp::update() end\n";
}

//--------------------------------------------------------------
void ofApp::draw()
{
	//std::cout << "ofApp::draw() begin\n";

	//FlockVisuals::get().update();
	FlockVisuals::get().display();

	//std::cout << "ofApp::draw() end\n";
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == 'f')
	{
		ofGetMainLoop()->getCurrentWindow()->toggleFullscreen();
		//ofGetMainLoop()->getCurrentWindow()->setWindowShape(windowSize[0], windowSize[1]);
		//ofGetMainLoop()->getCurrentWindow()->setWindowPosition(windowPos[0], windowPos[1]);
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

